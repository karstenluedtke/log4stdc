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

// vector(size_type n, const value_type& x, const allocator_type& a);

#include <vector>
#include <cassert>
#include "tests/support/min_allocator.h"

template <class C>
void
test(typename C::size_type n, const typename C::value_type& x,
     const typename C::allocator_type& a)
{
    C c(n, x, a);
    assert(c.__invariants());
    assert(a == c.get_allocator());
    assert(c.size() == n);
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        assert(*i == x);
}

int main()
{
    test<barefootc::vector<int> >(50, 3, std::allocator<int>());
#if __cplusplus >= 201103L
    test<barefootc::vector<int, min_allocator<int>> >(50, 3, min_allocator<int>());
#endif
}
