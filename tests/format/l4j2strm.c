
#include "compat.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "log4stdc.h"
#include "logobjs.h"

#define LF "\n"

/* From java objectstream spec */
/* clang-format off */
#define TC_NULL 		0x70
#define TC_REFERENCE 		0x71
#define TC_CLASSDESC 		0x72
#define TC_OBJECT 		0x73
#define TC_STRING 		0x74
#define TC_ARRAY 		0x75
#define TC_CLASS 		0x76
#define TC_BLOCKDATA 		0x77
#define TC_ENDBLOCKDATA 	0x78
#define TC_RESET 		0x79
#define TC_BLOCKDATALONG 	0x7A
#define TC_EXCEPTION 		0x7B
#define TC_LONGSTRING 	 	0x7C
#define TC_PROXYCLASSDESC 	0x7D
#define TC_ENUM 	 	0x7E

#define SC_WRITE_METHOD 	0x01 /* if SC_SERIALIZABLE */
#define SC_BLOCK_DATA 		0x08 /* if SC_EXTERNALIZABLE */
#define SC_SERIALIZABLE 	0x02
#define SC_EXTERNALIZABLE 	0x04
#define SC_ENUM 		0x10

static const char expectation1[] = {
TC_OBJECT,
TC_CLASSDESC, 0x00, 0x3e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','c','o','r','e','.','i','m','p','l','.','L','o','g','4','j','L','o','g','E','v','e','n','t', 0x24, 'L','o','g','E','v','e','n','t','P','r','o','x','y',
 0x9c, 0xed, 0x0f, 0x5d, 'p', 0xda, 0x2a, 0x16, /* serialVersionUID */
 SC_SERIALIZABLE, 0x00, 0x0d, /* 13 fields will follow */
 'Z', 0x00, 0x0c, 'i','s','E','n','d','O','f','B','a','t','c','h',
 'Z', 0x00, 0x12, 'i','s','L','o','c','a','t','i','o','n','R','e','q','u','i','r','e','d',
 'J', 0x00, 0x0a, 't','i','m','e','M','i','l','l','i','s',
 'L', 0x00, 0x0a, 'c','o','n','t','e','x','t','M','a','p',
      TC_STRING, 0x00, 0x0f, 'L','j','a','v','a', 0x2f, 'u','t','i','l', 0x2f, 'M','a','p',';',
 'L', 0x00, 0x0c, 'c','o','n','t','e','x','t','S','t','a','c','k',
      TC_STRING, 0x00, 0x35, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 'T','h','r','e','a','d','C','o','n','t','e','x','t', 0x24, 'C','o','n','t','e','x','t','S','t','a','c','k',';',
 'L', 0x00, 0x05, 'l','e','v','e','l',
      TC_STRING, 0x00, 0x20, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 'L','e','v','e','l',';',
 'L', 0x00, 0x0a, 'l','o','g','g','e','r','F','Q','C','N',
      TC_STRING, 0x00, 0x12, 'L','j','a','v','a', 0x2f, 'l','a','n','g', 0x2f, 'S','t','r','i','n','g',';',
 'L', 0x00, 0x0a, 'l','o','g','g','e','r','N','a','m','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
 'L', 0x00, 0x06, 'm','a','r','k','e','r',
      TC_STRING, 0x00, 0x21, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 'M','a','r','k','e','r',';',
 'L', 0x00, 0x07, 'm','e','s','s','a','g','e',
      TC_STRING, 0x00, 0x2a, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 'm','e','s','s','a','g','e', 0x2f, 'M','e','s','s','a','g','e',';',
 'L', 0x00, 0x06, 's','o','u','r','c','e',
      TC_STRING, 0x00, 0x1d, 'L','j','a','v','a', 0x2f, 'l','a','n','g', 0x2f, 'S','t','a','c','k','T','r','a','c','e','E','l','e','m','e','n','t',';',
 'L', 0x00, 0x0a, 't','h','r','e','a','d','N','a','m','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
 'L', 0x00, 0x0b, 't','h','r','o','w','n','P','r','o','x','y',
      TC_STRING, 0x00, 0x33, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 'c','o','r','e', 0x2f, 'i','m','p','l', 0x2f, 'T','h','r','o','w','a','b','l','e','P','r','o','x','y',';',
TC_ENDBLOCKDATA,
TC_NULL, /* superClassDesc */
/* End of newClassDesc */
0x00, /* isEndOfBatch */
0x01, /* isLocationRequired */
0,0,0,0,0,0,0,0, /* timeMillis */
TC_OBJECT,
TC_CLASSDESC, 0x00, 0x1e, 'j','a','v','a','.','u','t','i','l','.','C','o','l','l','e','c','t','i','o','n','s', 0x24, 'E','m','p','t','y','M','a','p',
 'Y', 0x36, 0x14, 0x85, 'Z', 0xdc, 0xe7, 0xd0,
 SC_SERIALIZABLE, 0x00, 0x00,
TC_ENDBLOCKDATA,
TC_NULL,
TC_OBJECT,
TC_CLASSDESC, 0x00, 0x3e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','T','h','r','e','a','d','C','o','n','t','e','x','t', 0x24, 'E','m','p','t','y','T','h','r','e','a','d','C','o','n','t','e','x','t','S','t','a','c','k',
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
 SC_SERIALIZABLE, 0x00, 0x00,
TC_ENDBLOCKDATA,
TC_NULL,
TC_OBJECT, /* level */
TC_CLASSDESC, 0x00, 0x1e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','L','e','v','e','l',
 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x20, 0x1a,
 SC_SERIALIZABLE, 0x00, 0x03,
 'I', 0x00, 0x08, 'i','n','t','L','e','v','e','l',
 'L', 0x00, 0x04, 'n','a','m','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
 'L', 0x00, 0x0d, 's','t','a','n','d','a','r','d','L','e','v','e','l',
      TC_STRING, 0x00, 0x2c, 'L','o','r','g', 0x2f, 'a','p','a','c','h','e', 0x2f, 'l','o','g','g','i','n','g', 0x2f, 'l','o','g','4','j', 0x2f, 's','p','i', 0x2f, 'S','t','a','n','d','a','r','d','L','e','v','e','l',';',
TC_ENDBLOCKDATA,
TC_NULL,
0x00, 0x00, 0x01, 0x90,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_ENUM,
TC_CLASSDESC, 0x00, 0x2a, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','s','p','i','.','S','t','a','n','d','a','r','d','L','e','v','e','l',
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00,
TC_ENDBLOCKDATA,
TC_CLASSDESC, 0x00, 0x0e, 'j','a','v','a','.','l','a','n','g','.','E','n','u','m',
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00,
TC_ENDBLOCKDATA,
TC_NULL,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_STRING, 0x00, 0x2b, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','s','p','i','.','A','b','s','t','r','a','c','t','L','o','g','g','e','r',
TC_STRING, 0x00, 0x0a, 't','e','s','t','l','o','g','g','e','r',
TC_NULL, /* marker */
TC_OBJECT, /* message */
TC_CLASSDESC, 0x00, '.','o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','m','e','s','s','a','g','e','.','S','i','m','p','l','e','M','e','s','s','a','g','e',
 0x8b, 't','M','0', 0x60, 0xb7, 0xa2, 0xa8,
 SC_SERIALIZABLE, 0x00, 0x01,
 'L', 0x00, 0x07, 'm','e','s','s','a','g','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
TC_ENDBLOCKDATA,
TC_NULL,
TC_STRING, 0x00, 0x0c, 'o','n','e', 0x20, 'l','o','g', 0x20, 'l','i','n','e',
TC_OBJECT, /* source */
TC_CLASSDESC, 0x00, 0x1b, 'j','a','v','a','.','l','a','n','g','.','S','t','a','c','k','T','r','a','c','e','E','l','e','m','e','n','t',
 'a', 0x09, 0xc5, 0x9a, 0x26, '6', 0xdd, 0x85,
 SC_SERIALIZABLE, 0x00, 0x04,
 'I', 0x00, 0x0a, 'l','i','n','e','N','u','m','b','e','r',
 'L', 0x00, 0x0e, 'd','e','c','l','a','r','i','n','g','C','l','a','s','s',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
 'L', 0x00, 0x08, 'f','i','l','e','N','a','m','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
 'L', 0x00, 0x0a, 'm','e','t','h','o','d','N','a','m','e',
      TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
TC_ENDBLOCKDATA,
TC_NULL,
0x00, 0x00, 0x00, 0x64, /* lineNumber */
TC_STRING, 0x00, 0x00,  /* declaringClass */
TC_STRING, 0x00, 0x0a, 's','o','u','r','c','e','f','i','l','e',
TC_STRING, 0x00, 0x0c, 't','e','s','t','f','u','n','c','t','i','o','n',
TC_STRING, 0x00, 0x0a, 'm','a','i','n','t','h','r','e','a','d', /* threadName */
TC_NULL /* thrownProxy */
};

static const char expectation2[] = {
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x00,
0x00, /* isEndOfBatch */
0x01, /* isLocationRequired */
0x00,0x00,0x01,0x52,0xB1,0x4D,0x9E,0x0A, /* timeMillis */
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0a,
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0c,
TC_OBJECT, /* level */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0e,
0x00, 0x00, 0x01, 0x90,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_ENUM,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x12,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_STRING, 0x00, 0x2b, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','s','p','i','.','A','b','s','t','r','a','c','t','L','o','g','g','e','r',
TC_STRING, 0x00, 0x0a, 't','e','s','t','l','o','g','g','e','r',
TC_NULL, /* marker */
TC_OBJECT, /* message */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x18,
TC_STRING, 0x00, 0x10, 'a','n','o','t','h','e','r', 0x20, 'l','o','g', 0x20, 'l','i','n','e',
TC_OBJECT, /* source */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x1b,
0x00, 0x00, 0x00, 0x65, /* lineNumber */
TC_STRING, 0x00, 0x00,  /* declaringClass */
TC_STRING, 0x00, 0x0a, 's','o','u','r','c','e','f','i','l','e',
TC_STRING, 0x00, 0x0c, 't','e','s','t','f','u','n','c','t','i','o','n',
TC_STRING, 0x00, 0x0a, 'm','a','i','n','t','h','r','e','a','d', /* threadName */
TC_NULL, /* thrownProxy */
};

static const char expectation3[] = {
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x00,
0x00, /* isEndOfBatch */
0x01, /* isLocationRequired */
0x00,0x00,0x01,0x52,0xB1,0x4D,0x9E,0x0A, /* timeMillis */
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0a,
TC_OBJECT,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0c,
TC_OBJECT, /* level */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x0e,
0x00, 0x00, 0x01, 0x90,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_ENUM,
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x12,
TC_STRING, 0x00, 0x04, 'I','N','F','O',
TC_STRING, 0x00, 0x2b, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','s','p','i','.','A','b','s','t','r','a','c','t','L','o','g','g','e','r',
TC_STRING, 0x00, 0x0a, 't','e','s','t','l','o','g','g','e','r',
TC_NULL, /* marker */
TC_OBJECT, /* message */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x18,
TC_STRING, 0x00, 0x11, 's','o','m','e',' ','u','t','f','8',':',' ',0xc3,0xa4,0xc3,0xb6,0xc3,0xbc,
TC_OBJECT, /* source */
TC_REFERENCE, 0x00, 0x7E, 0x00, 0x1b,
0x00, 0x00, 0x00, 0x66, /* lineNumber */
TC_STRING, 0x00, 0x00,  /* declaringClass */
TC_STRING, 0x00, 0x10, 's','o','u','r','c','e','f','i','l','e',0xED,0xA0,0xBC,0xED,0xB3,0x91,
TC_STRING, 0x00, 0x0c, 't','e','s','t','f','u','n','c','t','i','o','n',
TC_STRING, 0x00, 0x0a, 'm','a','i','n','t','h','r','e','a','d', /* threadName */
TC_NULL, /* thrownProxy */
};
/* clang-format on */

void
test1(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
      const char *expectation, int expected_length)
{
    int i, bytes;
    char buf[1400];

    bytes = l4sc_formatmsg(layout, msg, buf, sizeof(buf));

    for (i = 0; i < expected_length; i++) {
        unsigned char c = ((unsigned char *)buf)[i];
        unsigned char x = ((unsigned char *)expectation)[i];
        printf("#%03u: 0x%02X, expect 0x%02X '%c'\n", i, c, x,
               ((x >= ' ') && (x < 0x7F)) ? x : '.');
        assert(c == x);
    }
    printf("%d bytes\n", bytes);
    assert(bytes == expected_length);
}

int
main(int argc, char *argv[])
{
    l4sc_logger_ptr_t logger;
    struct l4sc_layout layout;
    struct l4sc_logmessage msg;
    struct mempool *pool = get_default_mempool();

    logger = l4sc_get_logger("testlogger", 0);

    (*l4sc_log4j2_stream_layout_class.init)(&layout, sizeof(layout), pool);

    l4sc_init_logmessage(&msg, sizeof(msg), logger, INFO_LEVEL, "one log line",
                         12, "sourcefile", 100, "testfunction");

    memset(&msg.time, 0, sizeof(msg.time));
    l4sc_snprintf(msg.threadid, sizeof(msg.threadid), "mainthread");

    test1(&layout, &msg, expectation1, sizeof(expectation1));

    msg.line = 101;
    msg.msg = "another log line";
    msg.msglen = 16;
    msg.time.tv_day = 16836;
    msg.time.tv_sec = 43200uL;
    msg.time.tv_usec = 10000uL;

    test1(&layout, &msg, expectation2, sizeof(expectation2));

    msg.line = 102;
    msg.msg = "some utf8: \303\244\303\266\303\274";
    msg.msglen = 17;
    msg.file = "sourcefile\360\237\203\221"; /* playing cards ace */

    test1(&layout, &msg, expectation3, sizeof(expectation3));

    msg.line = 102;
    msg.msg = "some utf8: \344\366\374"; /* this is latin-1 ! */
    msg.msglen = 14;

    test1(&layout, &msg, expectation3, sizeof(expectation3));
    return (0);
}
