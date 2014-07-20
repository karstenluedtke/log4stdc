#include "tests/strings/cxxwrapper.h"
// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <string>
#include <cassert>

int main()
{
#if _LIBCPP_STD_VER > 11 
    using namespace std::literals;

    barefootc::basic_string<char> foo  =   ""s;
#endif
}
