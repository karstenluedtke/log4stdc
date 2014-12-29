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

// void push_back(const value_type& x);

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

static l4sc_logger_ptr_t logger;

int main()
{
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger("barefootc.container", 0);
    {
        barefootc::vector<int> c;
        c.push_back(0);
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(1);
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(2);
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(3);
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(4);
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
    }
    {
        barefootc::vector<int, stack_allocator<int, 15> > c;
        c.push_back(0);
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(1);
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(2);
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(3);
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(4);
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> c;
        c.push_back(0);
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(1);
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(2);
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(3);
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
        c.push_back(4);
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == j);
    }
#endif
}
