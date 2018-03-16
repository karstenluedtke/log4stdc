
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#if defined(__STDC__) || defined(STDC_HEADERS) || defined(HAVE_WCHAR_H)
#include <wchar.h>
#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <wchar.h>
#endif

#include "log4stdc.h"

void
test1(const char *expectation, const char *fmt, const char *s)
{
    int rc;
    char buf[200];

    rc = l4sc_snprintf(buf, sizeof(buf), fmt, s);
    printf("test1: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
    assert(rc == strlen(expectation));
    assert(strcmp(buf, expectation) == 0);
    assert(rc == strlen(buf));
}

void
test2(const char *expectation, const char *fmt, int val)
{
    int rc;
    char buf[200];

    rc = l4sc_snprintf(buf, sizeof(buf), fmt, val);
    printf("test2: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
    assert(rc == strlen(expectation));
    assert(strcmp(buf, expectation) == 0);
    assert(rc == strlen(buf));
}

void
test3(const char *expectation, const char *fmt, ...)
{
    int rc;
    va_list ap;
    char buf[200];

    va_start(ap, fmt);
    rc = l4sc_vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    printf("test3: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
    assert(rc == strlen(expectation));
    assert(strcmp(buf, expectation) == 0);
    assert(rc == strlen(buf));
}

#if defined(WEOF)
#define L_ABCDEF L"ABCDEF"
#else
static const unsigned short L_ABCDEF[] = {'A', 'B', 'C', 'D', 'E', 'F', 0};
#endif

int
main(int argc, char *argv[])
{
    /* l4sc_snprintf might use the vsnprintf from the "C" lib */
    test1("abcdef", "abc%s", "def");
    test1("abcd", "abc%.1s", "def");
    test1("abc  d", "abc%3.1s", "def");
    test1("abcd  ", "abc%-3.1s", "def");

    test2("abc1k", "abc%dk", 1);
    test2("abc12k", "abc%dk", 12);
    test2("abc123k", "abc%dk", 123);
    test2("   123", "%6d", 123);
    test2("   123", "%6u", 123);
    test2("  0123", "%6.4d", 123);
    test2("  0123", "%6.4u", 123);
    test2("0123  ", "%-6.4d", 123);
    test2("  +123", "%+6.3d", 123);
    test2("+00123", "%+6.5d", 123);
    test2("000123", "%.6d", 123);
    test2("  -123", "%6d", -123);
    test2(" -0123", "%6.4d", -123);
    test2("-0123 ", "%-6.4d", -123);

    test2("0", "%d", 0);
    test2("", "%.d", 0);
    test2("1", "%.d", 1);
    test2("0", "%.1d", 0);

    /* this will test l4sc_vsnprintf - not the "C" lib. */
    test3("abcdef", "abc%s", "def");
    test3("abcd", "abc%.1s", "def");
    test3("abc  d", "abc%3.1s", "def");
    test3("abcd  ", "abc%-3.1s", "def");

    test3("abc1k", "abc%dk", 1);
    test3("abc12k", "abc%dk", 12);
    test3("abc123k", "abc%dk", 123);
    test3("   123", "%6d", 123);
    test3("   123", "%6u", 123);
    test3("  0123", "%6.4d", 123);
    test3("  0123", "%6.4u", 123);
    test3("0123  ", "%-6.4d", 123);
    test3("  +123", "%+6.3d", 123);
    test3("+00123", "%+6.5d", 123);
    test3("000123", "%.6d", 123);
    test3("  -123", "%6d", -123);
    test3(" -0123", "%6.4d", -123);
    test3("-0123 ", "%-6.4d", -123);

    test3("0", "%d", 0);
    test3("", "%.d", 0);
    test3("1", "%.d", 1);
    test3("0", "%.1d", 0);

    test3(" +1234567890", "%+12.10ld", 1234567890L);
    test3("+1234567890 ", "%-+12ld", 1234567890L);
    test3("aabb +1234567890. kk", "aa%s%+12.10ld.%3s", "bb", 1234567890L,
          "kk");
    test3("+4012345678 ", "%-+12lu", 4012345678uL);

    test3("abc", "%.*s", 3, "abcdef");
    test3("  abc", "%*.*s", 5, 3, "abcdef");

    test3("1456", "%o", 01456);
    test3("001456", "%06o", 01456);
    test3("001456", "%.6o", 01456);

    test3("14ab", "%x", 0x14ab);
    test3("0014ab", "%06x", 0x14ab);
    test3("14abcd", "%06lx", 0x14abcdL);
    test3("0x14ab", "0x%lx", 0x14abL);
    test3("0x0004", "0x%.4x", '\004');
    test3("0x14ab", "%p", (void *)0x14ab);
    if (sizeof(void *) >= 4) {
        test3("0x14abcdef", "%p", (void *)0x14abcdef);
        test3("0xffffffff", "%p", (void *)0xffffffff);
    }

    test3("0000FFFF", "%08X", 0xffffu);
    test3("   65535", "%8u", 0xffffu);
    test3("00032767", "%08d", 0x7fff);
    test3("-0032767", "%08d", -0x7fff);
    test3("  -32768", "%8d", -0x8000);
    test3("00177777", "%08o", 0xffffu);

    test3("FFFFFFFF", "%08lX", 0xffffffffuL);
    test3("4294967295", "%8lu", 0xffffffffuL);
    test3("2147483647", "%08ld", 0x7fffffffL);
    test3("-2147483647", "%08ld", -0x7fffffffL);
    test3("-2147483648", "%08ld", -0x80000000L);
    test3("37777777777", "%08lo", 0xffffffffuL);

    test3("ABCDEF", "%ls", L_ABCDEF);
    test3("   ABC", "%6.3ls", L_ABCDEF);
    test3("ABC", "%.*ls", 3, L_ABCDEF);

    return (0);
}
