
#include <log4cxx/logstring.h>
#include <log4cxx/file.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/helpers/pool.h>

#include "log4stdc.h"
#include "barefootc/utf8.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

File::File() {
}

template<class S> 
static LogString decodeLS(const S* src) {
    LogString dst;
    if (src != 0) {
      Transcoder::decode(src, dst);
    }
    return dst;
}

template<class S> 
static LogString decodeLS(const std::basic_string<S>& src) {
    LogString dst;
    Transcoder::decode(src, dst);
    return dst;
}


File::File(const std::string& name1)
  : path(decodeLS(name1)) {
}

File::File(const char* name1)
  : path(decodeLS(name1)) {
}

#if LOG4CXX_WCHAR_T_API
File::File(const std::wstring& name1)
   : path(decodeLS(name1)) {
}

File::File(const wchar_t* name1)
   : path(decodeLS(name1)) {
}
#endif

#if LOG4CXX_UNICHAR_API
File::File(const std::basic_string<UniChar>& name1)
   : path(decodeLS(name1)) {
}

File::File(const UniChar* name1)
   : path(decodeLS(name1)) {
}
#endif

#if LOG4CXX_CFSTRING_API
File::File(const CFStringRef& name1)
   : path(decodeLS(name1)) {
}
#endif

File::File(const File& src)
  : path(src.path) {
}

File& File::operator=(const File& src) {
  if (this == &src) return *this;

  path.assign(src.path);

  return *this;
}


File::~File() {
}


LogString File::getPath() const {
    return path;
}

File& File::setPath(const LogString& newName) {
    path.assign(newName);
    return *this;
}

LogString File::getName() const {
    const logchar slashes[] = { 0x2F, 0x5C, 0 };
    size_t lastSlash = path.find_last_of(slashes);
    if (lastSlash != LogString::npos) {
        return path.substr(lastSlash+1);
    }
    return path;
}

#if 0
char* File::getPath(Pool& p) const {
    return 0;
}
#endif

log4cxx_status_t File::open(apr_file_t** file, int flags,
      int perm, Pool& p) const {
	return -1;
}



bool File::exists(Pool& p) const {
	return false;
}

char* File::convertBackSlashes(char* src) {
  for(char* c = src; *c != 0; c++) {
   if(*c == '\\') {
      *c = '/';
   }
  }
  return src;
}

bool File::deleteFile(Pool& p) const {
	return false;
}

bool File::renameTo(const File& dest, Pool& p) const {
	return false;
}


size_t File::length(Pool& pool) const {
	return 0;
}


log4cxx_time_t File::lastModified(Pool& pool) const {
	return 0;
}


std::vector<LogString> File::list(Pool& p) const {
	std::vector<LogString> filenames;
	return filenames;
}

LogString File::getParent(Pool&) const {
	LogString parent;
	return parent;
}

bool File::mkdirs(Pool& p) const {
	return false;
}

/*
 * Private extension for FMCC PalService
 */
namespace log4cxx {
	void setBaseDirectoryName(const char *path)
	{
		l4sc_set_base_directory_name(path);
	}

	void setBaseDirectoryName(std::string& path)
	{
		l4sc_set_base_directory_name(path.c_str());
	}

	void setBaseDirectoryName(std::wstring& path)
	{
		const size_t wlen = path.length();
		const size_t bufsize = 3*wlen + 20;
		char buf[bufsize];
		char *cp = buf;
		for (size_t pos=0; pos < wlen; pos++) {
			unsigned long unicode = path[pos];
			BFC_PUT_UTF8(cp, buf+sizeof(buf), unicode);
		}
		if (cp < buf + sizeof(buf)) {
			*cp = 0;
		} else {
			buf[sizeof(buf)-1] = 0;
		}
		l4sc_set_base_directory_name(buf);
	}
}

