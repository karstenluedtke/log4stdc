#include "tests/vector/cxxvector.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// vector& operator=(const vector& c);

#include <vector>
#include <cassert>
#include "tests/support/test_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
    l4sc_configure_from_xml_file("log4j.xml");
    {
        barefootc::vector<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        barefootc::vector<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        barefootc::vector<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        barefootc::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
#if 0
        assert(l2.get_allocator() == other_allocator<int>(5));
#endif
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int> > l(3, 2, min_allocator<int>());
        barefootc::vector<int, min_allocator<int> > l2(l, min_allocator<int>());
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == min_allocator<int>());
    }
#endif
}
