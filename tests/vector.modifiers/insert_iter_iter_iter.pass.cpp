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

// template <class Iter>
//   iterator insert(const_iterator position, Iter first, Iter last);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

#if 0
#include "test_iterators.h"
#define INPUT_ITERATOR(s)	input_iterator<const int*>(s)
#define FORWARD_ITERATOR(s)	forward_iterator<const int*>(s)
#else
#define INPUT_ITERATOR(s) \
	barefootc::iterator<int>(s)
#define FORWARD_ITERATOR(s) \
	barefootc::iterator<int>(s)
#endif

int main()
{
    {
        barefootc::vector<int> v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int>::iterator i = v.insert(v.cbegin() + 10, INPUT_ITERATOR(a),
                                        INPUT_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int> v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int>::iterator i = v.insert(v.cbegin() + 10, FORWARD_ITERATOR(a),
                                        FORWARD_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int, stack_allocator<int, 308> > v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int>::iterator i = v.insert(v.cbegin() + 10, INPUT_ITERATOR(a),
                                        INPUT_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int, stack_allocator<int, 300> > v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int>::iterator i = v.insert(v.cbegin() + 10, FORWARD_ITERATOR(a),
                                        FORWARD_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<int> v(100);
        barefootc::vector<int> v2(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int>::iterator i = v.insert(v2.cbegin() + 10, INPUT_ITERATOR(a),
                                        INPUT_ITERATOR(a+N));
        assert(false);
    }
#endif
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, INPUT_ITERATOR(a),
                                        INPUT_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, FORWARD_ITERATOR(a),
                                        FORWARD_ITERATOR(a+N));
        assert(v.size() == 100 + N);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (int k = 0; k < N; ++j, ++k)
            assert(v[j] == a[k]);
        for (; j < 105; ++j)
            assert(v[j] == 0);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        barefootc::vector<int, min_allocator<int>> v2(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::vector<int, min_allocator<int>>::iterator i = v.insert(v2.cbegin() + 10, INPUT_ITERATOR(a),
                                        INPUT_ITERATOR(a+N));
        assert(false);
    }
#endif
#endif
}
