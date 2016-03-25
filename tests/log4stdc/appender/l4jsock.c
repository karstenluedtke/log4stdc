
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "log4stdc.h"

#define LF "\n"

static const char configstring[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" LF
"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
							" debug=\"1\">" LF
"	<appender class=\"org.apache.log4j.SocketAppender\" name=\"testlog\">" LF
"		<param name=\"RemoteHost\" value=\"localhost\"/>" LF
"		<param name=\"Port\" value=\"%u\"/>" LF
"	</appender>" LF
"	<logger additivity=\"false\" name=\"testlogger\">" LF
"		<level value=\"DEBUG\"/>" LF
"		<appender-ref ref=\"testlog\"/>" LF
"	</logger>" LF
"</log4j:configuration>" LF;

int
main (int argc, char *argv[])
{
	l4sc_logger_ptr_t logger;
	int rc, sock;
	int alen;
	char buf[800];
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	assert (sock != -1);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	((unsigned char *)&addr.sin_addr)[0] = 127;
	((unsigned char *)&addr.sin_addr)[3] =   1;
	rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	assert (rc >= 0);

	alen = sizeof(addr);
	rc = getsockname(sock, (struct sockaddr *)&addr, &alen);
	assert (rc >= 0);
	assert (alen > 0);
	assert (addr.sin_port != 0);

	rc = l4sc_snprintf(buf,sizeof(buf),configstring,ntohs(addr.sin_port));
	assert (rc > 0);
	assert (rc < (int) sizeof(buf));

	rc = l4sc_configure_from_xml_string(buf, rc);
	assert (rc >= 0);
	
	rc = fork();
	assert (rc != -1);
	if (rc == 0) {
		/* child */
		close(sock);
		sleep(1);
		logger = l4sc_get_logger("testlogger", 0);
		assert (logger != NULL);

		l4sc_logprintf(logger, INFO_LEVEL,
				"sourcefile", 100, "testfunction",
				"ABC");
		l4sc_logprintf(logger, INFO_LEVEL,
				"sourcefile", 101, "testfunction",
				"DEF");
		return(0);
	} else {
		/* parent */
		int conn;
		int status;
		int bytes = 0;
		rc = listen(sock, 1);
		assert (rc >= 0);

		alen = sizeof(addr);
		conn = accept(sock, (struct sockaddr *)&addr, &alen);
		assert (conn >= 0);

		while (bytes < sizeof(buf)) {
			rc = read(conn, buf+bytes, sizeof(buf)-bytes);
			if (rc > 0) {
				int i;
				for (i=0; i < rc; i++) {
					unsigned char c =
					    ((unsigned char *)buf)[bytes+i];
					if ((c >= ' ') && (c < 0x7F)) {
						printf("%c", c);
					} else {
						printf("\\%03o", c);
					}
				}
				printf("\n");
				bytes += rc;
			} else {
				break;
			}
		}
		close(conn);
		assert(bytes >= 4);
		assert(((unsigned char *)buf)[0] == 0xAC);
		assert(((unsigned char *)buf)[1] == 0xED);
		assert(((unsigned char *)buf)[2] == 0x00);
		assert(((unsigned char *)buf)[3] == 0x05);

		assert(bytes > 4);

		wait(&status);
	}

	close(sock);
	return (0);
}

