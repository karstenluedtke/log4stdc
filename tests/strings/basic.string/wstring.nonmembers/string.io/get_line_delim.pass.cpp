#include <wchar.h>
#include "tests/strings/cxxwrapper.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// template<class charT, class traits, class Allocator>
//   basic_istream<charT,traits>&
//   getline(basic_istream<charT,traits>& is,
//           basic_string<charT,traits,Allocator>& str, charT delim);

#include <string>
#include <sstream>
#include <cassert>

#include "tests/support/min_allocator.h"

int main()
{
    {
        std::istringstream in(L" abc*  def**   ghij");
        barefootc::basic_string<wchar_t> s(L"initial text");
        getline(in, s, '*');
        assert(in.good());
        assert(s == " abc");
        getline(in, s, '*');
        assert(in.good());
        assert(s == "  def");
        getline(in, s, '*');
        assert(in.good());
        assert(s == "");
        getline(in, s, '*');
        assert(in.eof());
        assert(s == "   ghij");
    }
    {
        std::wistringstream in(L" abc*  def**   ghij");
        std::wstring s(L"initial text");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L" abc");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L"  def");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L"");
        getline(in, s, L'*');
        assert(in.eof());
        assert(s == L"   ghij");
    }
#if __cplusplus >= 201103L
    {
        typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
        std::istringstream in(L" abc*  def**   ghij");
        S s(L"initial text");
        getline(in, s, '*');
        assert(in.good());
        assert(s == " abc");
        getline(in, s, '*');
        assert(in.good());
        assert(s == "  def");
        getline(in, s, '*');
        assert(in.good());
        assert(s == "");
        getline(in, s, '*');
        assert(in.eof());
        assert(s == "   ghij");
    }
    {
        typedef barefootc::basic_string<wchar_t, std::char_traits<wchar_t>, min_allocator<wchar_t>> S;
        std::wistringstream in(L" abc*  def**   ghij");
        S s(L"initial text");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L" abc");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L"  def");
        getline(in, s, L'*');
        assert(in.good());
        assert(s == L"");
        getline(in, s, L'*');
        assert(in.eof());
        assert(s == L"   ghij");
    }
#endif
}
