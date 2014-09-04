#include <wchar.h>
#include "tests/strings/cxxwrapper.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// size_type max_size() const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test1(const S& s)
{
    S s2(s);
    const size_t sz = s2.max_size() - 1;
    try { s2.resize(sz, 'x'); }
    catch ( const std::bad_alloc & ) { return ; }
    assert ( s2.size() ==  sz );
}

template <class S>
void
test2(const S& s)
{
    S s2(s);
    const size_t sz = s2.max_size();
    l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 0);
    L4SC_DEBUG(logger, "%s: trying %ld chars", __FUNCTION__, (long) sz);
    try { s2.resize(sz, 'x'); }
    catch ( const std::bad_alloc & ) { return ; }
    L4SC_DEBUG(logger, "%s: s.size %ld, s2.size %ld, expect %ld",
		__FUNCTION__, (long) s.size(), (long) s2.size(), (long) sz);
    // original llvm source: assert ( s.size() ==  sz );
    assert ( s2.size() ==  sz );
}

template <class S>
void
test3(const S& s)
{
    S s2(s);
    const size_t sz = s2.max_size() + 1;
    try { s2.resize(sz, 'x'); }
    catch ( const std::length_error & ) { return ; }
    assert ( false );
}

template <class S>
void
test(const S& s)
{
    assert(s.max_size() >= s.size());
    test1(s);
    test2(s);
    test3(s);
}

int main()
{
  l4sc_configure_from_xml_file("log4j.xml");
  l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 0);
  L4SC_TRACE(logger,
    "tests/strings/basic.string/string.capacity/max_size.pass starting");
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S());
    test(S(L"123"));
    test(S(L"12345678901234567890123456789012345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S());
    test(S(L"123"));
    test(S(L"12345678901234567890123456789012345678901234567890"));
    }
#endif
}
