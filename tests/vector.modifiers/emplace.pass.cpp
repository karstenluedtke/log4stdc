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

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

class A
{
    int i_;
    double d_;

    A(const A&);
    A& operator=(const A&);
public:
    A(int i, double d)
        : i_(i), d_(d) {}

    A(A&& a)
        : i_(a.i_),
          d_(a.d_)
    {
        a.i_ = 0;
        a.d_ = 0;
    }

    A& operator=(A&& a)
    {
        i_ = a.i_;
        d_ = a.d_;
        a.i_ = 0;
        a.d_ = 0;
        return *this;
    }

    int geti() const {return i_;}
    double getd() const {return d_;}
};

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        barefootc::vector<A> c;
        barefootc::vector<A>::iterator i = c.emplace(c.cbegin(), 2, 3.5);
        assert(i == c.begin());
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        i = c.emplace(c.cend(), 3, 4.5);
        assert(i == c.end()-1);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        i = c.emplace(c.cbegin()+1, 4, 6.5);
        assert(i == c.begin()+1);
        assert(c.size() == 3);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c[1].geti() == 4);
        assert(c[1].getd() == 6.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
    }
    {
        barefootc::vector<A, stack_allocator<A, 7> > c;
        barefootc::vector<A, stack_allocator<A, 7> >::iterator i = c.emplace(c.cbegin(), 2, 3.5);
        assert(i == c.begin());
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        i = c.emplace(c.cend(), 3, 4.5);
        assert(i == c.end()-1);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        i = c.emplace(c.cbegin()+1, 4, 6.5);
        assert(i == c.begin()+1);
        assert(c.size() == 3);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c[1].geti() == 4);
        assert(c[1].getd() == 6.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<A> c1;
        barefootc::vector<A> c2;
        barefootc::vector<A>::iterator i = c1.emplace(c2.cbegin(), 2, 3.5);
        assert(false);
    }
#endif
#if __cplusplus >= 201103L
    {
        barefootc::vector<A, min_allocator<A>> c;
        barefootc::vector<A, min_allocator<A>>::iterator i = c.emplace(c.cbegin(), 2, 3.5);
        assert(i == c.begin());
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        i = c.emplace(c.cend(), 3, 4.5);
        assert(i == c.end()-1);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        i = c.emplace(c.cbegin()+1, 4, 6.5);
        assert(i == c.begin()+1);
        assert(c.size() == 3);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c[1].geti() == 4);
        assert(c[1].getd() == 6.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<A, min_allocator<A>> c1;
        barefootc::vector<A, min_allocator<A>> c2;
        barefootc::vector<A, min_allocator<A>>::iterator i = c1.emplace(c2.cbegin(), 2, 3.5);
        assert(false);
    }
#endif
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
