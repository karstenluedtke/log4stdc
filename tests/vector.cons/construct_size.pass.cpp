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

// explicit vector(size_type n);

#include <vector>
#include <cassert>

//#include "DefaultOnly.h"
#include "tests/support/min_allocator.h"
#include "tests/support/test_allocator.h"

template <class C>
void
test2(typename C::size_type n, typename C::allocator_type const& a = typename C::allocator_type ())
{
#if _LIBCPP_STD_VER > 11
    C c(n, a);
    assert(c.__invariants());
    assert(c.size() == n);
    assert(c.get_allocator() == a);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        assert(*i == typename C::value_type());
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#endif
}

template <class C>
void
test1(typename C::size_type n)
{
    C c(n);
    assert(c.__invariants());
    assert(c.size() == n);
    assert(c.get_allocator() == typename C::allocator_type());
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        assert(*i == typename C::value_type());
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}

template <class C>
void
test(typename C::size_type n)
{
    test1<C> ( n );
    test2<C> ( n );
}

int main()
{
    test<barefootc::vector<int> >(50);
    //test<barefootc::vector<DefaultOnly> >(500);
    //assert(DefaultOnly::count == 0);
#if __cplusplus >= 201103L
    test<barefootc::vector<int, min_allocator<int>> >(50);
    test<barefootc::vector<DefaultOnly, min_allocator<DefaultOnly>> >(500);
    test2<barefootc::vector<DefaultOnly, test_allocator<DefaultOnly>> >( 100, test_allocator<DefaultOnly>(23));
    assert(DefaultOnly::count == 0);
#endif
}
