
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "log4stdc.h"

#define LF "\n"

static const char configstring[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" LF
    "<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
    " debug=\"1\">" LF "	<appender "
                       "class=\"org.apache.log4j.SocketAppender\" "
                       "name=\"testlog\">" LF
    "		<param name=\"RemoteHost\" value=\"localhost\"/>" LF
    "		<param name=\"Port\" value=\"%u\"/>" LF
    "		<layout class=\"org.apache.log4j.XmlLayout\"/>" LF
    "	</appender>" LF
    "	<logger additivity=\"false\" name=\"testlogger\">" LF
    "		<level value=\"DEBUG\"/>" LF
    "		<appender-ref ref=\"testlog\"/>" LF "	</logger>" LF
    "</log4j:configuration>" LF;

/*
 * Expected output stream
 */
static const char refstream[] =
    "<Event xmlns=\"http://logging.apache.org/log4j/2.0/events\""
    " timeMillis=\"?????????????\" thread=\"????\""
    " level=\"INFO\" loggerName=\"testlogger\" endOfBatch=\"false\""
    " loggerFqcn=\"org.apache.logging.log4j.spi.AbstractLogger\">\n"
    "  <Message>ABC</Message>\n"
    "  <Source class=\"\""
    " method=\"testfunction\" file=\"sourcefile\" line=\"100\"/>\n"
    "</Event>\r\n"
    "<Event xmlns=\"http://logging.apache.org/log4j/2.0/events\""
    " timeMillis=\"?????????????\" thread=\"????\""
    " level=\"INFO\" loggerName=\"testlogger\" endOfBatch=\"false\""
    " loggerFqcn=\"org.apache.logging.log4j.spi.AbstractLogger\">\n"
    "  <Message>x &lt; 2 &amp;&amp; l\303\244nger als 5\"</Message>\n"
    "  <Source class=\"\""
    " method=\"testfunction\" file=\"sourcefile\" line=\"101\"/>\n"
    "</Event>\r\n";

int
main(int argc, char *argv[])
{
#ifdef HAVE_SYS_SOCKET_H
    l4sc_logger_ptr_t logger;
    int rc, sock;
    socklen_t alen;
    char buf[800];
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    ((unsigned char *)&addr.sin_addr)[0] = 127;
    ((unsigned char *)&addr.sin_addr)[3] = 1;
    rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(rc >= 0);

    alen = sizeof(addr);
    rc = getsockname(sock, (struct sockaddr *)&addr, &alen);
    assert(rc >= 0);
    assert(alen > 0);
    assert(addr.sin_port != 0);

    rc = l4sc_snprintf(buf, sizeof(buf), configstring, ntohs(addr.sin_port));
    assert(rc > 0);
    assert(rc < (int)sizeof(buf));

    rc = l4sc_configure_from_xml_string(buf, rc);
    assert(rc >= 0);

    rc = fork();
    assert(rc != -1);
    if (rc == 0) {
        /* child */
        close(sock);
        sleep(1);
        logger = l4sc_get_logger("testlogger", 0);
        assert(logger != NULL);

        l4sc_logprintf(logger, INFO_LEVEL, "sourcefile", 100, "testfunction",
                       "ABC");
        l4sc_logprintf(logger, INFO_LEVEL, "sourcefile", 101, "testfunction",
                       "x < 2 && l\303\244nger als 5\"");
        return (0);
    } else {
        /* parent */
        unsigned char *cp;
        int i, conn, status, bytes = 0;

        rc = listen(sock, 1);
        assert(rc >= 0);

        alen = sizeof(addr);
        conn = accept(sock, (struct sockaddr *)&addr, &alen);
        assert(conn >= 0);

        while (bytes < sizeof(buf)) {
            rc = read(conn, buf + bytes, sizeof(buf) - bytes);
            if (rc > 0) {
                int i;
                for (i = 0; i < rc; i++) {
                    unsigned char c = ((unsigned char *)buf)[bytes + i];
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
        cp = (unsigned char *)buf;
        for (i = 0; i < sizeof(refstream) - 1; i++, cp++) {
            unsigned char x = ((unsigned char *)refstream)[i];
            printf("#%03u: 0x%02X, expect 0x%02X '%c'\n", i, *cp, x,
                   ((x >= ' ') && (x < 0x7F)) ? x : '.');
            assert((*cp == x) || (x == '?'));
            if ((*cp != x) && (x == '?') && (refstream[i - 1] == '"')) {
                /* skip remainder of string */
                while (*cp && (*cp != '"'))
                    cp++;
                while (refstream[i] && (refstream[i] != '"'))
                    i++;
            }
        }
        printf("%d bytes\n", bytes);

        wait(&status);
    }

    close(sock);
#endif /* HAVE_SYS_SOCKET_H */
    return (0);
}
