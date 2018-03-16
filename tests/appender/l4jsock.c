
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
    "		<param name=\"Port\" value=\"%u\"/>" LF "	</appender>" LF
    "	<logger additivity=\"false\" name=\"testlogger\">" LF
    "		<level value=\"DEBUG\"/>" LF
    "		<appender-ref ref=\"testlog\"/>" LF "	</logger>" LF
    "</log4j:configuration>" LF;

/*
 * Output stream from log4cxx generated by l4cxsock.cpp.
 */
static const char refstream[] = {
    0xAC, 0xED, 0x00, 0x05, 's', 'r', 0x00, '!', 'o', 'r', 'g', '.', 'a', 'p',
    'a', 'c', 'h', 'e', '.', 'l', 'o', 'g', '4', 'j', '.', 's', 'p', 'i', '.',
    'L', 'o', 'g', 'g', 'i', 'n', 'g', 'E', 'v', 'e', 'n', 't', 0xF3, 0xF2,
    0xB9, '#', 't', 0x0B, 0xB5, '?', 0x03, 0x00, 0x0A, 'Z', 0x00, 0x15, 'm',
    'd', 'c', 'C', 'o', 'p', 'y', 'L', 'o', 'o', 'k', 'u', 'p', 'R', 'e', 'q',
    'u', 'i', 'r', 'e', 'd', 'Z', 0x00, 0x11, 'n', 'd', 'c', 'L', 'o', 'o',
    'k', 'u', 'p', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd', 'J', 0x00, 0x09,
    't', 'i', 'm', 'e', 'S', 't', 'a', 'm', 'p', 'L', 0x00, 0x0C, 'c', 'a',
    't', 'e', 'g', 'o', 'r', 'y', 'N', 'a', 'm', 'e', 't', 0x00, 0x12, 'L',
    'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n',
    'g', ';', 'L', 0x00, 0x0C, 'l', 'o', 'c', 'a', 't', 'i', 'o', 'n', 'I',
    'n', 'f', 'o', 't', 0x00, '#', 'L', 'o', 'r', 'g', '/', 'a', 'p', 'a', 'c',
    'h', 'e', '/', 'l', 'o', 'g', '4', 'j', '/', 's', 'p', 'i', '/', 'L', 'o',
    'c', 'a', 't', 'i', 'o', 'n', 'I', 'n', 'f', 'o', ';', 'L', 0x00, 0x07,
    'm', 'd', 'c', 'C', 'o', 'p', 'y', 't', 0x00, 0x15, 'L', 'j', 'a', 'v',
    'a', '/', 'u', 't', 'i', 'l', '/', 'H', 'a', 's', 'h', 't', 'a', 'b', 'l',
    'e', ';', 'L', 0x00, 0x03, 'n', 'd', 'c', 't', 0x00, 0x12, 'L', 'j', 'a',
    'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';',
    'L', 0x00, 0x0F, 'r', 'e', 'n', 'd', 'e', 'r', 'e', 'd', 'M', 'e', 's',
    's', 'a', 'g', 'e', 't', 0x00, 0x12, 'L', 'j', 'a', 'v', 'a', '/', 'l',
    'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';', 'L', 0x00, 0x0A,
    't', 'h', 'r', 'e', 'a', 'd', 'N', 'a', 'm', 'e', 't', 0x00, 0x12, 'L',
    'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n',
    'g', ';', 'L', 0x00, 0x0D, 't', 'h', 'r', 'o', 'w', 'a', 'b', 'l', 'e',
    'I', 'n', 'f', 'o', 't', 0x00, '+', 'L', 'o', 'r', 'g', '/', 'a', 'p', 'a',
    'c', 'h', 'e', '/', 'l', 'o', 'g', '4', 'j', '/', 's', 'p', 'i', '/', 'T',
    'h', 'r', 'o', 'w', 'a', 'b', 'l', 'e', 'I', 'n', 'f', 'o', 'r', 'm', 'a',
    't', 'i', 'o', 'n', ';', 'x', 'p', 0x00, /* mdc lookup required */
    0x00,                                    /* ndc lookup required */
    '?', '?', '?', '?', '?', '?', '?',
    '?', /* timestamp 0x00,0x00,0x01,'S',0xAD,0xAD,'O',0xD2, */
    't', 0x00, 0x0A, 't', 'e', 's', 't', 'l', 'o', 'g', 'g', 'e', 'r', 's',
    'r', 0x00, '!', 'o', 'r', 'g', '.', 'a', 'p', 'a', 'c', 'h', 'e', '.', 'l',
    'o', 'g', '4', 'j', '.', 's', 'p', 'i', '.', 'L', 'o', 'c', 'a', 't', 'i',
    'o', 'n', 'I', 'n', 'f', 'o', 0xED, 0x99, 0xBB, 0xE1, 'J', 0x91, 0xA5, '|',
    0x02, 0x00, 0x01, 'L', 0x00, 0x08, 'f', 'u', 'l', 'l', 'I', 'n', 'f', 'o',
    't', 0x00, 0x12, 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/',
    'S', 't', 'r', 'i', 'n', 'g', ';', 'x', 'p', 't', 0x00, 0x1D, '.', 't',
    'e', 's', 't', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', '(', 's', 'o', 'u',
    'r', 'c', 'e', 'f', 'i', 'l', 'e', ':', '1', '0', '0', ')', 'p', 'p', 't',
    0x00, 0x03, 'A', 'B', 'C', 't', '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    /* threadid '0','x','7','f','d','3','9','8','e','f','5','7','8','0',*/
    'p', 'w', 0x04, 0x00, 0x00, 0x4E, 0x20,           /* level */
    'p', 'x', 's', 'q', 0x00, 0x7E, 0x00, 0x00, 0x00, /* mdc lookup required */
    0x00,                                             /* ndc lookup required */
    '?', '?', '?', '?', '?', '?', '?',
    '?', /* timestamp 0x00,0x00,0x01,'S',0xAD,0xAD,'O',0xD3,*/
    't', 0x00, 0x0A, 't', 'e', 's', 't', 'l', 'o', 'g', 'g', 'e', 'r', 's',
    'q', 0x00, 0x7E, 0x00, 0x0A, 't', 0x00, 0x1D, '.', 't', 'e', 's', 't', 'f',
    'u', 'n', 'c', 't', 'i', 'o', 'n', '(', 's', 'o', 'u', 'r', 'c', 'e', 'f',
    'i', 'l', 'e', ':', '1', '0', '1', ')', 'p', 'p', 't', 0x00, 0x03, 'D',
    'E', 'F', 't', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?',
    /* threadid '0','x','7','f','d','3','9','8','e','f','5','7','8','0',*/
    'p', 'w', 0x04, 0x00, 0x00, 0x4E, 0x20, /* level */
    'p', 'x'};

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
                       "DEF");
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
        assert(cp[0] == 0xAC);
        assert(cp[1] == 0xED);
        assert(cp[2] == 0x00);
        assert(cp[3] == 0x05);

        assert(bytes > 4);
        for (i = 0; i < sizeof(refstream); i++, cp++) {
            unsigned char x = ((unsigned char *)refstream)[i];
            printf("#%03u: 0x%02X, expect 0x%02X '%c'\n", i, *cp, x,
                   ((x >= ' ') && (x < 0x7F)) ? x : '.');
            assert((*cp == x) || (x == '?'));
            if ((*cp != x) && (x == '?') && (refstream[i - 1] == 't')) {
                /* skip remainder of string */
                cp += cp[1] + 1;
                while (refstream[i + 1] == '?')
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
