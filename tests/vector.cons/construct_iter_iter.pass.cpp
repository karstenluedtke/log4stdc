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

// template <class InputIter> vector(InputIter first, InputIter last);

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

template <class C, class Iterator>
void
test(Iterator first, Iterator last)
{
    C c(first, last);
    assert(c.__invariants());
    assert(c.size() == barefootc::distance(first, last));
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i, ++first)
        assert(*i == *first);
}

int main()
{
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a)/sizeof(a[0]);
    test<barefootc::vector<int> >(INPUT_ITERATOR(a), INPUT_ITERATOR(an));
    test<barefootc::vector<int> >(FORWARD_ITERATOR(a), FORWARD_ITERATOR(an));
    test<barefootc::vector<int> >(BIDIR_ITERATOR(a), BIDIR_ITERATOR(an));
    test<barefootc::vector<int> >(RANDOM_ITERATOR(a), RANDOM_ITERATOR(an));
    test<barefootc::vector<int>,barefootc::iterator<int> >(a, an);

    test<barefootc::vector<int, stack_allocator<int, 63> > >(INPUT_ITERATOR(a), INPUT_ITERATOR(an));
    test<barefootc::vector<int, stack_allocator<int, 18> > >(FORWARD_ITERATOR(a), FORWARD_ITERATOR(an));
    test<barefootc::vector<int, stack_allocator<int, 18> > >(BIDIR_ITERATOR(a), BIDIR_ITERATOR(an));
    test<barefootc::vector<int, stack_allocator<int, 18> > >(RANDOM_ITERATOR(a), RANDOM_ITERATOR(an));
    test<barefootc::vector<int, stack_allocator<int, 18> >,barefootc::iterator<int> >(a, an);
#if __cplusplus >= 201103L
    test<barefootc::vector<int, min_allocator<int>> >(INPUT_ITERATOR(a), INPUT_ITERATOR(an));
    test<barefootc::vector<int, min_allocator<int>> >(FORWARD_ITERATOR(a), FORWARD_ITERATOR(an));
    test<barefootc::vector<int, min_allocator<int>> >(BIDIR_ITERATOR(a), BIDIR_ITERATOR(an));
    test<barefootc::vector<int, min_allocator<int>> >(RANDOM_ITERATOR(a), RANDOM_ITERATOR(an));
    test<barefootc::vector<int>,barefootc::iterator<int> >(a, an);
#endif
}
