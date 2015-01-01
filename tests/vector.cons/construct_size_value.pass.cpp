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

// vector(size_type n, const value_type& x);

#include <vector>
#include <cassert>

#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

template <class C>
void
test(typename C::size_type n, const typename C::value_type& x)
{
    C c(n, x);
    assert(c.__invariants());
    assert(c.size() == n);
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        assert(*i == x);
}

int main()
{
    test<barefootc::vector<int> >(50, 3);
    test<barefootc::vector<int, stack_allocator<int, 50> > >(50, 5);
#if __cplusplus >= 201103L
    test<barefootc::vector<int, min_allocator<int>> >(50, 3);
#endif
}
