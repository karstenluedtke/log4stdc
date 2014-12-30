/**
 * @file      cxxiterator.h
 *
 * @brief     C++ access to barefootc iterator implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-06-29
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_STRING_CXX_WRAPPER_H_
#define _BFC_STRING_CXX_WRAPPER_H_

#ifdef __cplusplus

#include <string>
#include <memory>
#include <stdexcept>

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

#include <stdio.h>
#include <string.h>
#include <wchar.h>

#if __cplusplus >= 201103L
#else
#define noexcept
#endif

namespace barefootc {
	template<class T>
	class iterator {
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		iterator()
		{
			l4sc_logger_ptr_t logger =
				l4sc_get_logger("barefootc.string",16);
			L4SC_WARN(logger, "%s()", __FUNCTION__);
			bfc_init_iterator(&bfcit, sizeof(bfcit), NULL, 0);
		}

		iterator(const iterator& it)
		{
			VOID_METHCALL(bfc_iterator_classptr_t, &it.bfcit,
				clone, (&it.bfcit, &bfcit, sizeof(bfcit)));
		}

		iterator(const bfc_iterator_t& bfciter)
		{
			VOID_METHCALL(bfc_iterator_classptr_t, &bfciter,
				clone, (&bfciter, &bfcit, sizeof(bfcit)));
		}

		iterator(const char *s)
		{
			bfc_init_cstr_iterator(&bfcit, sizeof(bfcit), s, 0);
		}

		iterator(const wchar_t *s)
		{
			bfc_init_wstr_iterator(&bfcit, sizeof(bfcit), s, 0);
		}

		iterator(const int *s)
		{
			bfc_init_intarray_iterator(&bfcit, sizeof(bfcit), s, 0);
		}

		~iterator()
		{
		}

		void operator=(const iterator& rhs)
		{
			VOID_METHCALL(bfc_iterator_classptr_t, &rhs.bfcit,
				clone, (&rhs.bfcit, &bfcit, sizeof(bfcit)));
		}

		int equals(const iterator& rhs) const
		{
			bfc_iterptr_t l=const_cast<bfc_iterptr_t>(&bfcit);
			bfc_iterptr_t r=const_cast<bfc_iterptr_t>(&rhs.bfcit);
			RETURN_METHCALL(bfc_iterator_classptr_t, &bfcit,
					equals, (l, r), 0);
		}

		int operator==(const iterator& rhs) const
		{
			return (equals(rhs));
		}

		int operator!=(const iterator& rhs) const
		{
			return (!equals(rhs));
		}

		const T& operator*() const
		{
			const void *p;
			RETVAR_METHCALL(p, bfc_iterator_classptr_t, &bfcit,
					first, (&bfcit), NULL);
			if (p == NULL) {
				throw(std::out_of_range("bad position"));
			}
			const T *cp = (const T *) p;
			return (*cp);
		}
		
		T& operator*()
		{
			void *p;
			RETVAR_METHCALL(p, bfc_iterator_classptr_t, &bfcit,
					index, (&bfcit, 0), NULL);
			if (p == NULL) {
				throw(std::out_of_range("bad position"));
			}
			T *cp = (T *) p;
			return (*cp);
		}
		
		T& operator[](size_t pos)
		{
			void *p;
			RETVAR_METHCALL(p, bfc_iterator_classptr_t, &bfcit,
					index, (&bfcit, pos), NULL);
			if (p == NULL) {
				throw(std::out_of_range("bad position"));
			}
			T *cp = (T *) p;
			return (*cp);
		}
		
		void advance(ptrdiff_t n)
		{
			VOID_METHCALL(bfc_iterator_classptr_t, &bfcit,
					advance, (&bfcit, n));
		}

		ptrdiff_t distance(const iterator& last) const
		{
			bfc_iterptr_t f=const_cast<bfc_iterptr_t>(&bfcit);
			bfc_iterptr_t l=const_cast<bfc_iterptr_t>(&last.bfcit);
			RETURN_METHCALL(bfc_iterator_classptr_t, &bfcit,
					distance, (f, l), 0);
		}

		iterator& operator++()
		{
			advance(1);
			return (*this);
		}

		iterator operator++(int postincr)
		{
			iterator it(*this);
			advance(1);
			return (it);
		}

		iterator& operator--()
		{
			advance(-1);
			return (*this);
		}

		iterator operator--(int postdecr)
		{
			iterator it(*this);
			advance(-1);
			return (it);
		}

		bfc_iterptr_t bfciter()
		{
			return (&bfcit);
		}

	private:
		struct bfc_iterator bfcit;
	};

	template<class T>
	int distance(const iterator<T>& lhs, const iterator<T>& rhs)
	{
		return (lhs.distance(rhs));
	}

	template<class T>
	int operator-(const iterator<T>& lhs, const iterator<T>& rhs)
	{
		return (rhs.distance(lhs));
	}

	template<class T>
	iterator<T> next(const iterator<T>& iter, ptrdiff_t n=1)
	{
		iterator<T> it(iter);
		it.advance(n);
		return (it);
	}

	template<class T>
	iterator<T> operator+(const iterator<T>& iter, ptrdiff_t n)
	{
		iterator<T> it(iter);
		it.advance(n);
		return (it);
	}

	template<class T>
	iterator<T> operator-(const iterator<T>& iter, ptrdiff_t n)
	{
		iterator<T> it(iter);
		it.advance(-n);
		return (it);
	}

}

#endif /* __cplusplus */
#endif /* _BFC_STRING_CXX_ITERATOR_H_ */
