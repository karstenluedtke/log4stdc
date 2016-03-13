
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#if defined(HAVE_WCHAR_H)
#include <wchar.h>
#endif
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_WINDOWS_FILES 1
#define L4SC_WINDOWS_LOCKS 1
#include <windows.h>
#include <malloc.h>  /* for alloca */
#define getcwd _getcwd
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#endif
#if defined(HAVE_ALLOCA_H)
#include <alloca.h>
#endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(_MSC_VER)
#define strncasecmp strnicmp
#endif

#include "logobjs.h"
#include "bareftc/mempool.h"
#include "bareftc/mutex.h"

static int init_appender(void *, size_t, bfc_mempool_t );
static void destroy_appender(l4sc_appender_ptr_t appender);
static size_t get_appender_size(l4sc_appender_cptr_t obj);

static int  set_appender_option(l4sc_appender_ptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
static int  get_appender_option(l4sc_appender_cptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
static void apply_appender_options(l4sc_appender_ptr_t obj);
static void append_to_output(l4sc_appender_ptr_t appender,
			     l4sc_logmessage_cptr_t msg);
static bfc_mutex_ptr_t lock_appender(l4sc_appender_ptr_t appender);
static void unlock_appender(l4sc_appender_ptr_t appender,bfc_mutex_ptr_t lock);
static void open_appender(l4sc_appender_ptr_t appender);
static void close_appender(l4sc_appender_ptr_t appender);
static int  start_rollover(l4sc_appender_ptr_t appender);
static void complete_rollover(l4sc_appender_ptr_t appender);

static int merge_path(char *buf, int bufsize,
		      const char *dirpath, const char *relpath, int rellen);

#define is_open(appender) ((appender)->fu.fh != NULL)

#define need_rollover(appender,len) \
	(((appender)->maxfilesize > 0) && \
	 ((appender)->maxfilesize < (appender)->filesize + (len)))

extern const struct l4sc_appender_class l4sc_sysout_appender_class;

const struct l4sc_appender_class l4sc_file_appender_class = {
	/* .super 	*/ &l4sc_sysout_appender_class,
	/* .name 	*/ "fileappender",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_appender,
	/* .initrefc 	*/ (void *) l4sc_default_init_refcount,
	/* .incrrefc 	*/ (void *) l4sc_default_incr_refcount,
	/* .decrrefc 	*/ (void *) l4sc_default_decr_refcount,
	/* .destroy 	*/ destroy_appender,
	/* .clone 	*/ (void *) l4sc_default_clone_object,
	/* .clonesize 	*/ get_appender_size,
	/* .hashcode 	*/ (void *) l4sc_default_get_object_hashcode,
	/* .equals 	*/ (void *) l4sc_default_is_equal_object,
	/* .length 	*/ (void *) l4sc_default_get_object_length,
	/* .tostring 	*/ (void *) l4sc_default_object_tostring,
	/* .dump 	*/ (void *) l4sc_default_dump_object,
	/* .set_name	*/ NULL, /* inherit */
	/* .set_opt	*/ set_appender_option,
	/* .get_opt	*/ get_appender_option,
	/* .apply	*/ apply_appender_options,
	/* .close	*/ close_appender,
	/* .append	*/ append_to_output,
	/* .set_layout	*/ NULL, /* inherit */
	/* .ref_layout	*/ NULL  /* inherit */
};

static char initial_working_directory[256] = { 0 };

static int
init_appender(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(l4sc_appender_class_ptr_t,
			  l4sc_appender_ptr_t, appender, buf, bufsize, pool,
			  &l4sc_file_appender_class);

	appender->name = "file appender";
#ifdef L4SC_WINDOWS_LOCKS
	l4sc_new_win32_mutex(&appender->lock, pool,
			__FILE__, __LINE__, __FUNCTION__);
#else
	l4sc_new_posix_mutex(&appender->lock, pool,
			__FILE__, __LINE__, __FUNCTION__);
#endif
	if (initial_working_directory[0] == 0) {
		if (getcwd(initial_working_directory,
				sizeof(initial_working_directory)) == NULL) {
			LOGERROR(("%s: cannot store initial working dir: %d",
				__FUNCTION__, (int) errno));
		}
	}
	return (BFC_SUCCESS);
}

static void
destroy_appender(l4sc_appender_ptr_t appender)
{
	bfc_mutex_ptr_t lock = appender->lock;
	close_appender(appender);
	if (lock) {
		bfc_mempool_t pool = lock->parent_pool;
		appender->lock = NULL;
		VOID_METHCALL(bfc_mutex_class_ptr_t, lock, destroy, (lock));
		if (pool != NULL) {
			mempool_free(pool, lock);
		}
	}
	BFC_DESTROY_EPILOGUE(appender, &l4sc_file_appender_class);
}

static size_t
get_appender_size(l4sc_appender_cptr_t obj)
{
	return (sizeof(struct l4sc_appender));
}

static int
set_appender_option(l4sc_appender_ptr_t obj, const char *name, size_t namelen,
				     const char *value, size_t vallen)
{
	LOGINFO(("%s: %.*s=\"%.*s\"",__FUNCTION__,
		(int) namelen, name, (int) vallen, value));

	if ((namelen == 4) && (strncasecmp(name, "File", 4) == 0)) {
		int n = merge_path(obj->pathbuf, sizeof(obj->pathbuf),
				   initial_working_directory, value, vallen);
		if ((n > 0) && (n < sizeof(obj->pathbuf))) {
			obj->filename = obj->pathbuf;
			LOGINFO(("%s: File set to \"%s\"",
				__FUNCTION__, obj->filename));
		} else {
			bfc_mempool_t pool = obj->parent_pool?
							obj->parent_pool:
							get_default_mempool();
			char *p = mempool_alloc(pool, n+20);
			if (p != NULL) {
				merge_path(p, n+20, initial_working_directory, 
								value, vallen);
				obj->filename = p;
				LOGINFO(("%s: File set to \"%s\" (malloc)",
					__FUNCTION__, obj->filename));
			} else {
				LOGERROR(("%s: no memory for File \"%.*s\"",
					__FUNCTION__, (int) vallen, value));
			}
		}
	} else if ((namelen == 11)
		&& (strncasecmp(name, "MaxFileSize", 11) == 0)) {
		char *unit = NULL;
		unsigned long maxsize = strtoul(value, &unit, 10);
		if (unit != NULL) {
			while ((unit < value+vallen) && (unit[0] == ' ')) {
				unit++;
			}
			if (unit < value+vallen) {
				switch (unit[0]) {
				case 'k':
				case 'K': maxsize <<= 10;
					  break;
				case 'm':
				case 'M': maxsize <<= 20;
					  break;
				case 'g':
				case 'G': maxsize <<= 30;
					  break;
				default:
					LOGERROR(("%s: unknown unit in"
						" %.*s=\"%.*s\"",__FUNCTION__,
						(int) namelen, name,
						(int) vallen, value));
				}
			}
		}
		if (maxsize < 50) {
			maxsize <<= 20;	/* assume size in MB */
		} else if (maxsize < 1000) {
			maxsize <<= 10;	/* assume size in kB */
		}
		obj->maxfilesize = maxsize;
		LOGINFO(("%s: MaxFileSize set to %lu",
			__FUNCTION__, obj->maxfilesize));
		
	} else if ((namelen == 14)
		&& (strncasecmp(name, "MaxBackupIndex", 14) == 0)) {
		obj->maxbackupindex = strtoul(value, NULL, 10);
		LOGINFO(("%s: MaxBackupIndex set to %u",
			__FUNCTION__, obj->maxbackupindex));
	}
	return (0);
}

static int
get_appender_option(l4sc_appender_cptr_t obj, const char *name, size_t namelen,
				     char *valbuf, size_t bufsize)
{
	return (0);
}

static void
append_to_output(l4sc_appender_ptr_t appender, l4sc_logmessage_cptr_t msg)
{
	if (msg && (msg->msglen > 0)) {
		l4sc_layout_cptr_t layout = &appender->layout;
		bfc_mempool_t pool = appender->parent_pool;
		const size_t bufsize = msg->msglen + 200;
		char *poolmem = ((bufsize > 2000) && pool)?
				bfc_mempool_alloc(pool, bufsize):
				NULL;
		char *buf = poolmem? poolmem: alloca(bufsize);
		const int len = l4sc_formatmsg(layout, msg, buf, bufsize);
		if (len > 0) {
			bfc_mutex_ptr_t lock = lock_appender(appender);
			int rolling = 0;

			if (!is_open(appender)) {
				open_appender(appender);
			}
			if (need_rollover(appender, len)) {
				rolling = start_rollover(appender);
			}
			if (is_open(appender)) {
#if defined(L4SC_WINDOWS_FILES)
				DWORD dwBytesWritten;
				WriteFile((HANDLE) appender->fu.fh,
					buf, len, &dwBytesWritten, NULL);
				appender->filesize += dwBytesWritten;
#else
				int rc;
				size_t written = 0;
				do {
					if ((rc = write(appender->fu.fd,
							buf + written,
							len - written)) > 0) {
						written += rc;
					} else if ((errno != EINTR)
						&& (errno != EAGAIN)) {
						break;
					}
				} while (written < len);
				appender->filesize += written;
#endif
			}

			unlock_appender(appender, lock);
		
			if (rolling) {
				complete_rollover(appender);
			}
		}
		if (poolmem) {
			bfc_mempool_free(pool, poolmem);
			poolmem = NULL;
		}
	}
}

static void
apply_appender_options(l4sc_appender_ptr_t obj)
{
}

static bfc_mutex_ptr_t
lock_appender(l4sc_appender_ptr_t appender)
{
	bfc_mutex_ptr_t lock = appender->lock;
	if (lock) {
		bfc_mutex_lock(lock);
		return (lock);
	}
	return (NULL);
}

static void
unlock_appender(l4sc_appender_ptr_t appender, bfc_mutex_ptr_t lock)
{
	if (lock) {
		bfc_mutex_unlock(lock);
	}
}

static void
open_appender(l4sc_appender_ptr_t appender)
{
#if defined(L4SC_WINDOWS_FILES)
	if (appender->filename != NULL) {
		HANDLE fh;
		wchar_t *wbuf;
		size_t wlen, clen = strlen(appender->filename);
		wbuf = (wchar_t *) alloca((clen+1)*sizeof(wchar_t));
		wlen = MultiByteToWideChar(CP_UTF8, 0,
			appender->filename, clen, wbuf, clen+1);
		wbuf[(wlen < clen)? wlen: clen] = 0;
		fh = CreateFileW(wbuf, FILE_APPEND_DATA,
			FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL /* FILE_FLAG_WRITE_THROUGH */,
			NULL);
		if (fh != INVALID_HANDLE_VALUE) {
			DWORD sizehigh=0;
			DWORD size = GetFileSize(fh, &sizehigh);
			if (size == INVALID_FILE_SIZE) {
				size = 0;
			}
			appender->fu.fh = (void *) fh;
			appender->filesize = size;
		}
	}
#else
	if (appender->filename != NULL) {
		int fd = open(appender->filename,
			      O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd != -1) {
			struct stat sb;
			if (fstat(fd, &sb) != 0) {
				memset(&sb, 0, sizeof(sb));
			}
			appender->fu.fd = fd;
			appender->filesize = sb.st_size;
		}
	}
#endif
}

static void
close_appender(l4sc_appender_ptr_t appender)
{
	bfc_mutex_ptr_t lock = lock_appender(appender);
#if defined(L4SC_WINDOWS_FILES)
	HANDLE fh = appender->fu.fh;
	appender->fu.fh = NULL;
	appender->fu.fd = 0;
	if (fh) {
		CloseHandle((HANDLE) fh);
	}
#else
	int fd = appender->fu.fd;
	appender->fu.fh = NULL;
	appender->fu.fd = 0;
	if (fd && (fd != -1)) {
		close(fd);
	}
#endif
	unlock_appender(appender, lock);
}

static int
start_rollover(l4sc_appender_ptr_t appender)
{
	int fnlen, bufsize;
	char *from, *to;
	if (appender->filename == NULL) {
		return (0);
	}
	fnlen = strlen(appender->filename);
	bufsize = fnlen + 10;
	from = alloca(bufsize);
	to   = alloca(bufsize);
	/*
	 * test.log > test.log.0
	 */
	memcpy(from, appender->filename, fnlen);
	from[fnlen] = '\0';
	memcpy(to,   appender->filename, fnlen);
	to[fnlen+0] = '.';
	to[fnlen+1] = '0';
	to[fnlen+2] = '\0';
#if defined(L4SC_WINDOWS_FILES)
	close_appender(appender);
	MoveFileExA(from, to, 0 /* !MOVEFILE_COPY_ALLOWED */);
#else
	rename(from, to);
	close_appender(appender);
#endif
	open_appender(appender);
	return (1);
}

static void
complete_rollover(l4sc_appender_ptr_t appender)
{
	int i, fnlen, bufsize;
	char *from, *to;
	if (appender->filename == NULL) {
		return;
	}
	fnlen = strlen(appender->filename);
	bufsize = fnlen + 10;
	from = alloca(bufsize);
	to   = alloca(bufsize);
	memcpy(from, appender->filename, fnlen);
	memcpy(to,   appender->filename, fnlen);
	to[fnlen] = from[fnlen] = '\0';

	/*
	 * Remove last backup file
	 */
	l4sc_snprintf(from+fnlen, 6, ".%u", appender->maxbackupindex);
#if defined(L4SC_WINDOWS_FILES)
	DeleteFileA(from);
#else
	unlink(from);
#endif
	/*
	 * test.log.8 -> test.log.9,
	 * test.log.7 -> test.log.8,
	 * ...
	 * test.log.0 -> test.log.1
	 */
	for (i = appender->maxbackupindex; i > 0; i--) {
		l4sc_snprintf(to+fnlen, 6,   ".%d", i);
		l4sc_snprintf(from+fnlen, 6, ".%d", i-1);
#if defined(L4SC_WINDOWS_FILES)
		MoveFileExA(from, to, 0 /* !MOVEFILE_COPY_ALLOWED */);
#else
		rename(from, to);
#endif
	}
}

static int
merge_path(char *buf, int bufsize,
	   const char *dirpath, const char *relpath, int rellen)
{
	char sep = '/';
	const int dirlen = strlen(dirpath);
	int i;

	if ((relpath == NULL) || (rellen <= 0)) {
		LOGERROR(("%s: no relative path", __FUNCTION__));
		return (-EFAULT);
	}
	if ((dirlen == 0)
	 || ((rellen > 0) && ((relpath[0] == '/') || (relpath[0] == '\\')))
	 || ((rellen > 2) && ( relpath[1] == ':')		/* "C:/" */
			  && ((relpath[2] == '/') || (relpath[2] == '\\')))) {
		if (buf && (bufsize > 0)) {
			l4sc_snprintf(buf, bufsize, "%.*s", rellen, relpath);
		}
		return (rellen);
	} else {
		for (i=0; i < dirlen; i++) {
			if ((dirpath[i] == '/') || (dirpath[i] == '\\')) {
				sep = dirpath[i];
				break;
			}
		}
		if (buf && (bufsize > 0)) {
			l4sc_snprintf(buf, bufsize, "%.*s%c%.*s",
				dirlen, dirpath, sep, rellen, relpath);
		}
	}
	return (dirlen + rellen + 1);
}

int
l4sc_merge_base_directory_path (char *buf, int bufsize,
	  			const char *relpath, int rellen)
{
	if (initial_working_directory[0] == 0) {
		if (getcwd(initial_working_directory,
				sizeof(initial_working_directory)) == NULL) {
			LOGERROR(("%s: cannot store initial working dir: %d",
				__FUNCTION__, (int) errno));
		}
	}
	return (merge_path(buf, bufsize,
			   initial_working_directory, relpath, rellen));
}

int
l4sc_set_base_directory_name(const char *path)
{
	const int len = strlen(path);

	if (len < (int) sizeof(initial_working_directory)) {
		memcpy(initial_working_directory, path, len);
		initial_working_directory[len] = 0;
	} else {
		LOGERROR(("%s: cannot store base directory %s: too long %d",
						__FUNCTION__, path, len));
		return (-ENOSPC);
	}
	return (BFC_SUCCESS);
}

