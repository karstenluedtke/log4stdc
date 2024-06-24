
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "log4stdc.h"
#include "logobjs.h"

#define LF "\n"

/* clang-format off */
static const char configstring[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" LF
"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
							" debug=\"1\">" LF
"	<appender class=\"org.apache.log4j.SocketAppender\""
"                 name=\"testlog\">" LF
"		<param name=\"RemoteHost\" value=\"localhost\"/>" LF
"		<param name=\"Port\" value=\"0\"/>" LF
"		<layout class=\"log4stdc.JsonWsLayout\"/>" LF
"	</appender>" LF
"	<logger additivity=\"false\" name=\"testlogger\">" LF
"		<level value=\"DEBUG\"/>" LF
"		<appender-ref ref=\"testlog\"/>" LF
"	</logger>" LF
"</log4j:configuration>" LF;

/*
 * Output stream.
 */
static const char refstream1[] = 
"{ \"timeMillis\" : ?????????????, \"instant\" : {"
" \"epochSecond\" : ??????????, \"nanoOfSecond\" : 0 },"
" \"thread\" : \"?\","
   " \"level\" : \"INFO\", \"loggerName\" : \"testlogger\"," LF
"  \"message\" : \"ABC\"," LF
"  \"endOfBatch\" : false, \"loggerFqcn\" : \"org.apache.logging.log4j.spi.AbstractLogger\"," LF
"  \"source\" : { \"class\" : \"\", \"method\" : \"testfunction\", \"file\" : \"sourcefile\", \"line\" : 100 }" LF
"}" LF;

static const char refstream2[] = 
"{ \"timeMillis\" : ?????????????, \"instant\" : {"
" \"epochSecond\" : ??????????, \"nanoOfSecond\" : 456789000 },"
" \"thread\" : \"?\","
   " \"level\" : \"INFO\", \"loggerName\" : \"testlogger\"," LF
"  \"message\" : \"DEF\"," LF
"  \"endOfBatch\" : false, \"loggerFqcn\" : \"org.apache.logging.log4j.spi.AbstractLogger\"," LF
"  \"source\" : { \"class\" : \"\", \"method\" : \"testfunction\", \"file\" : \"sourcefile\", \"line\" : 101 }" LF
"}" LF;

static const char refstream3[] = 
"{ \"timeMillis\" : ?????????????, \"instant\" : {"
" \"epochSecond\" : ??????????, \"nanoOfSecond\" : 0 },"
" \"thread\" : \"?\","
   " \"level\" : \"INFO\", \"loggerName\" : \"testlogger\"," LF
"  \"message\" : \"????????????????\"," LF
"  \"endOfBatch\" : false, \"loggerFqcn\" : \"org.apache.logging.log4j.spi.AbstractLogger\"," LF
"  \"source\" : { \"class\" : \"\", \"method\" : \"testfunction\", \"file\" : \"sourcefile\", \"line\" : 102 }" LF
"}" LF;
/* clang-format on */

#define WEBSOCKET_F0_FIN 0x80
#define WEBSOCKET_F0_OPCODE 0x0F
#define WEBSOCKET_F0_CONT 0x00
#define WEBSOCKET_F0_TEXT 0x01
#define WEBSOCKET_F0_BINARY 0x02
#define WEBSOCKET_F0_CLOSE 0x08
#define WEBSOCKET_F0_PING 0x09
#define WEBSOCKET_F0_PONG 0x0A

#define WEBSOCKET_F1_MASK 0x80
#define WEBSOCKET_F1_LENGTH 0x7F
#define WEBSOCKET_F1_LEN2B 126
#define WEBSOCKET_F1_LEN8B 127

static int
check_formatted_message(char *buf, int size, const char *refstream)
{
    unsigned char *frame = (unsigned char *)buf;
    unsigned char *mask = frame + 4;
    unsigned char *cp = frame + 8;
    int i, bytes, reflen = strlen(refstream);

    assert(size >= 12);
    assert(frame[0] == (WEBSOCKET_F0_FIN | WEBSOCKET_F0_TEXT));

    if (frame[1] == (WEBSOCKET_F1_MASK | WEBSOCKET_F1_LEN8B)) {
        assert(size >= 65536 + 14);
        assert(frame[1] == (WEBSOCKET_F1_MASK | WEBSOCKET_F1_LEN8B));
        assert(frame[2] == 0);
        assert(frame[3] == 0);
        assert(frame[4] == 0);
        assert(frame[5] == 0);
        assert(frame[6] == 0);
        bytes = 65536 * frame[7] + 256 * frame[8] + frame[9];
        mask = frame + 10;
        cp = mask + 4;
    } else {
        assert(frame[1] == (WEBSOCKET_F1_MASK | WEBSOCKET_F1_LEN2B));
        bytes = 256 * frame[2] + frame[3];
    }

    assert(bytes >= 4);
    assert(size >= bytes + 8);

    for (i = 0; i < bytes; i++) {
        unsigned char c = cp[i] ^ mask[i & 3];
        if ((c >= ' ') && (c < 0x7F)) {
            printf("%c", c);
        } else if (c == '\n') {
            printf("\\n\n");
        } else {
            printf("\\%03o", c);
        }
        cp[i] = c;
    }
    printf("%d bytes\n", bytes);

    for (i = 0; i < reflen; i++, cp++) {
        unsigned char x = ((unsigned char *)refstream)[i];
        printf("#%03u/%u: 0x%02X, expect 0x%02X '%c'\n", i, reflen, *cp, x,
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

    return (0);
}

int
main(int argc, char *argv[])
{
    l4sc_logger_ptr_t logger;
    int rc, bytes;
    struct l4sc_appender *appender;
    struct l4sc_layout *layout;
    char buf[1500];
    l4sc_logmessage_t msg[2];
    static char bigbuf1[100000];
    static char bigbuf2[200000];

    rc = l4sc_configure_from_xml_string(configstring, strlen(configstring));
    assert(rc >= 0);

    logger = l4sc_get_logger("testlogger", 0);
    assert(logger != NULL);

    appender = l4sc_get_appender("testlog", 7, NULL, 0);
    assert(appender != NULL);

    layout = l4sc_get_appender_layout(appender);
    assert(layout != NULL);

    l4sc_init_logmessage(msg, sizeof(msg), logger, INFO_LEVEL, "ABC", 3,
                         "sourcefile", 100, "testfunction");
    msg->time.tv_usec = 0;
    RETVAR_METHCALL(bytes, l4sc_layout_class_ptr_t, layout, format,
                    (layout, msg, buf, sizeof(buf)), -1);

    check_formatted_message(buf, bytes, refstream1);

    l4sc_init_logmessage(msg, sizeof(msg), logger, INFO_LEVEL, "DEF", 3,
                         "sourcefile", 101, "testfunction");
    msg->time.tv_usec = 456789;
    RETVAR_METHCALL(bytes, l4sc_layout_class_ptr_t, layout, format,
                    (layout, msg, buf, sizeof(buf)), -1);

    check_formatted_message(buf, bytes, refstream2);

    memset(bigbuf1, 'A', sizeof(bigbuf1));
    l4sc_init_logmessage(msg, sizeof(msg), logger, INFO_LEVEL, bigbuf1,
                         sizeof(bigbuf1), "sourcefile", 102, "testfunction");
    msg->time.tv_usec = 0;
    RETVAR_METHCALL(bytes, l4sc_layout_class_ptr_t, layout, format,
                    (layout, msg, bigbuf2, sizeof(bigbuf2)), -1);

    check_formatted_message(bigbuf2, bytes, refstream3);

    return 0;
}
