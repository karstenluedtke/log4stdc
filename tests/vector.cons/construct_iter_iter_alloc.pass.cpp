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

// template <class InputIter> vector(InputIter first, InputIter last,
//                                   const allocator_type& a);

#include <vector>
#include <cassert>

#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

#if 0
#include "test_iterators.h"
#define INPUT_ITERATOR(s)	input_iterator<const int*>(s)
#define FORWARD_ITERATOR(s)	forward_iterator<const int*>(s)
#define BIDIR_ITERATOR(s)	bidirectional_iterator<const int*>(s)
#define RANDOM_ITERATOR(s)	random_access_iterator<const int*>(s)
#else
#define INPUT_ITERATOR(s)	barefootc::iterator<int>(s)
#define FORWARD_ITERATOR(s)	barefootc::iterator<int>(s)
#define BIDIR_ITERATOR(s)	barefootc::iterator<int>(s)
#define RANDOM_ITERATOR(s)	barefootc::iterator<int>(s)
#endif

template <class C, class Iterator, class A>
void
test(Iterator first, Iterator last, const A& a)
{
    C c(first, last, a);
    assert(c.__invariants());
    assert(c.size() == barefootc::distance(first, last));
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i, ++first)
        assert(*i == *first);
}

#if __cplusplus >= 201103L

template <class T>
struct implicit_conv_allocator : min_allocator<T>
{
    implicit_conv_allocator(void* p) {}
    implicit_conv_allocator(const implicit_conv_allocator&) = default;
};

#endif

int main()
{
    {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a)/sizeof(a[0]);
    std::allocator<int> alloc;
    test<barefootc::vector<int> >(INPUT_ITERATOR(a), INPUT_ITERATOR(an), alloc);
    test<barefootc::vector<int> >(FORWARD_ITERATOR(a), FORWARD_ITERATOR(an), alloc);
    test<barefootc::vector<int> >(BIDIR_ITERATOR(a), BIDIR_ITERATOR(an), alloc);
    test<barefootc::vector<int> >(RANDOM_ITERATOR(a), RANDOM_ITERATOR(an), alloc);
    test<barefootc::vector<int>, barefootc::iterator<int> >(a, an, alloc);
    }
#if __cplusplus >= 201103L
    {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a)/sizeof(a[0]);
    min_allocator<int> alloc;
    test<barefootc::vector<int, min_allocator<int>> >(INPUT_ITERATOR(a), INPUT_ITERATOR(an), alloc);
    test<barefootc::vector<int, min_allocator<int>> >(FORWARD_ITERATOR(a), FORWARD_ITERATOR(an), alloc);
    test<barefootc::vector<int, min_allocator<int>> >(BIDIR_ITERATOR(a), BIDIR_ITERATOR(an), alloc);
    test<barefootc::vector<int, min_allocator<int>> >(RANDOM_ITERATOR(a), RANDOM_ITERATOR(an), alloc);
    test<barefootc::vector<int, min_allocator<int>>, barefootc::iterator<int> >(a, an, alloc);
    test<barefootc::vector<int, implicit_conv_allocator<int>>, barefootc::iterator<int> >(a, an, nullptr);
    }
#endif
}
