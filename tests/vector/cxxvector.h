/**
 * @file      cxxvector.h
 *
 * @brief     C++ access to barefootc vector implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-12-28
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_STRING_CXX_VECTOR_H_
#define _BFC_STRING_CXX_VECTOR_H_

#ifdef __cplusplus

#include <string>
#include <memory>
#include <stdexcept>

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

#include <stdio.h>
#include <string.h>

#include "tests/iterators/cxxiterator.h"

#if __cplusplus >= 201103L
#else
#define noexcept
#endif

namespace barefootc {
	template <class T, class Allocator = std::allocator<T> >
	class vector {
	public:
		// types:
		typedef T value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Allocator allocator_type;
		typedef barefootc::iterator<T> iterator;
		typedef iterator const_iterator;
		typedef iterator reverse_iterator;
		typedef iterator const_reverse_iterator;

	private:
		BFC_VECTOR(bfcvec_s, T, 4) bfcvec;

		const Allocator saved_allocator;

		void init_vector(struct mempool *pool)
		{
			BFC_VECTOR_INIT_POOL(&bfcvec, pool);
			bfc_init_vector_class(&bfcvec, sizeof(bfcvec), pool);
		}

		static void throw_replace_error(int rc)
		{
			switch (rc) {
			case ERANGE:
				throw(std::out_of_range("bad position"));
				return;
			default:
				char m[80];
				snprintf(m, sizeof(m), "substr error %d", rc);
				throw(std::runtime_error(m));
			}
		}

	public:
		// 23.3.6.2, construct/copy/destroy:
		vector(): saved_allocator()
		{
			init_vector(get_stdc_mempool());
		}

		explicit vector(const Allocator& a): saved_allocator(a)
		{
			init_vector(get_stdc_mempool());
		}

		explicit vector(size_type n): saved_allocator()
		{
			init_vector(get_stdc_mempool());
			BFC_VECTOR_SET_SIZE(&bfcvec, n);
		}

		vector(size_type n, const T& value): saved_allocator()
		{
			int rc;
			init_vector(get_stdc_mempool());
			if (sizeof(T) > 8) {
				// might be a C++ object, so we create copies
				// here, in order to use the proper constructor.
				// Since the "C" implementation does not know
				// about C++ objects, the destructor will never
				// be called, even when the vector is destroyed
				// and the memory is released.
				for (int i=0; i < n; i++) {
					void *p = bfc_vector_have(&bfcvec, i);
					if (!p) {
						throw_replace_error(ENOMEM);
					}
					T *copy = new(p) T(value);
				}
			} else {
				rc = bfc_container_assign_fill(
					(bfc_contptr_t)&bfcvec, n, &value);
				if (rc < 0) {
					throw_replace_error(-rc);
				}
			}
		}

		vector(size_type n, const T& value,
			const Allocator& a): saved_allocator(a)
		{
			int rc;
			init_vector(get_stdc_mempool());
			if (sizeof(T) > 8) {
				// might be a C++ object, see comment above.
				for (int i=0; i < n; i++) {
					void *p = bfc_vector_have(&bfcvec, i);
					if (!p) {
						throw_replace_error(ENOMEM);
					}
					T *copy = new(p) T(value);
				}
			} else {
				rc = bfc_container_assign_fill(
					(bfc_contptr_t)&bfcvec, n, &value);
				if (rc < 0) {
					throw_replace_error(-rc);
				}
			}
		}

#if 0
		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const Allocator& = Allocator());
#endif
		vector(const_iterator first, const_iterator last):
			saved_allocator()
		{
			int rc;
			init_vector(get_stdc_mempool());
			rc = bfc_container_assign_range((bfc_contptr_t)&bfcvec,
					first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
		}

		vector(const_iterator first, const_iterator last,
			const Allocator& a): saved_allocator(a)
		{
			int rc;
			init_vector(get_stdc_mempool());
			rc = bfc_container_assign_range((bfc_contptr_t)&bfcvec,
					first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
		}

		vector(const vector<T,Allocator>& x):
			saved_allocator(x.get_allocator())
		{
			bfc_init_vector_copy(&bfcvec, sizeof(bfcvec),
					     get_stdc_mempool(), x.contptr());
		}

		vector(const vector& x, const Allocator& a): saved_allocator(a)
		{
			bfc_init_vector_copy(&bfcvec, sizeof(bfcvec),
					     get_stdc_mempool(), x.contptr());
		}

#if __cplusplus >= 201103L
		vector(vector&&);
		vector(vector&&, const Allocator&);
		vector(initializer_list<T>, const Allocator& = Allocator());
#endif
		~vector()
		{
			bfc_destroy((bfc_objptr_t) &bfcvec);
		}

		bfc_contptr_t contptr() const
		{
			return ((bfc_contptr_t) &bfcvec);
		}

		vector<T,Allocator>& operator=(const vector<T,Allocator>& x)
		{
			bfc_init_vector_copy(&bfcvec, sizeof(bfcvec),
					     get_stdc_mempool(), x.contptr());
			return (*this);
		}

#if __cplusplus >= 201103L
		vector<T,Allocator>& operator=(vector<T,Allocator>&& x);
		vector& operator=(initializer_list<T>);
#endif
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const T& u);
#if __cplusplus >= 201103L
		void assign(initializer_list<T>);
#endif
		allocator_type get_allocator() const
		{
			return (saved_allocator);
		}

		// iterators:
		iterator begin()
		{
			bfc_iterator_t tmp;
			bfc_init_iterator(&tmp, sizeof(tmp),
					  (bfc_objptr_t) &bfcvec, 0);
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		iterator end()
		{
			bfc_iterator_t tmp;
			bfc_init_iterator(&tmp, sizeof(tmp),
					  (bfc_objptr_t) &bfcvec, BFC_NPOS);
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		const_iterator cbegin() const
		{
			bfc_iterator_t tmp;
			bfc_init_iterator(&tmp, sizeof(tmp),
					  (bfc_objptr_t) &bfcvec, 0);
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_iterator cend() const
		{
			bfc_iterator_t tmp;
			bfc_init_iterator(&tmp, sizeof(tmp),
					  (bfc_objptr_t) &bfcvec, BFC_NPOS);
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_reverse_iterator crbegin() const;
		const_reverse_iterator crend() const;

		// 23.3.6.3, capacity:
		size_type size() const
		{
			return (bfc_object_length((bfc_cobjptr_t)&bfcvec));
		}

		size_type max_size() const
		{
			return (bfc_container_max_size((bfc_contptr_t)&bfcvec));
		}

		void resize(size_type sz)
		{
			bfc_container_resize((bfc_contptr_t)&bfcvec, sz, NULL);
		}

		void resize(size_type sz, const T& c)
		{
			bfc_container_resize((bfc_contptr_t)&bfcvec, sz, &c);
		}

		size_type capacity() const
		{
			return (bfc_container_capacity((bfc_contptr_t)&bfcvec));
		}

		bool empty() const
		{
			return (size() == 0);
		}

		void reserve(size_type n)
		{
			bfc_container_reserve((bfc_contptr_t)&bfcvec, n);
		}

		void shrink_to_fit()
		{
			bfc_container_reserve((bfc_contptr_t)&bfcvec, 0);
		}

		// element access:
		reference operator[](size_type n)
		{
			return (at(n));
		}

		const_reference operator[](size_type n) const
		{
			return (at(n));
		}

		const_reference at(size_type n) const
		{
			const T *p;
			p = (const T*)bfc_container_index(
					(bfc_ccontptr_t)&bfcvec, n);
			return (*p);
		}

		reference at(size_type n)
		{
			T *p;
			p = (T*)bfc_container_index((bfc_contptr_t)&bfcvec, n);
			return (*p);
		}

		reference front()
		{
			T *p;
			p = (T*)bfc_container_index((bfc_contptr_t)&bfcvec, 0);
			return (*p);
		}

		const_reference front() const
		{
			const T *p;
			p = (const T*)bfc_container_first((bfc_ccontptr_t)&bfcvec);
			return (*p);
		}

		reference back()
		{
			return (at(size()));
		}

		const_reference back() const
		{
			return (at(size()));
		}


		// 23.3.6.4, data access
		// WARNING: Elements are NOT organized as an array!
		T* data() noexcept
		{
			T *p;
			p = (T*)bfc_container_index((bfc_contptr_t)&bfcvec, 0);
			return (p);
		}

		const T* data() const noexcept
		{
			const T *p;
			p = (const T*)bfc_container_first((bfc_contptr_t)&bfcvec);
			return (p);
		}


		// 23.3.6.5, modifiers:
		void push_back(const T& x)
		{
			bfc_container_push_back((bfc_contptr_t)&bfcvec, &x);
		}

#if __cplusplus >= 201103L
		void push_back(T&& x);
		template <class... Args>
		void emplace_back(Args&&... args);
#endif
		void pop_back()
		{
			bfc_container_pop_back((bfc_contptr_t)&bfcvec);
		}

		iterator insert(const_iterator position, const T& x)
		{
			iterator it(position);
			int rc;
			rc = bfc_container_insert_element(
				(bfc_contptr_t)&bfcvec, it.bfciter(), &x);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		iterator insert(const_iterator position,
				size_type n, const T& x)
		{
			iterator it(position);
			int rc;
			rc = bfc_container_insert_fill(
				(bfc_contptr_t)&bfcvec, it.bfciter(), n, &x);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		iterator insert(const_iterator position, int n, const T& x)
		{
			iterator it(position);
			int rc;
			rc = bfc_container_insert_fill(
				(bfc_contptr_t)&bfcvec, it.bfciter(), n, &x);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		iterator insert(const_iterator position,
				const_iterator first, const_iterator last)
		{
			iterator it(position);
			int rc;
			rc = bfc_container_insert_range((bfc_contptr_t)&bfcvec,
				it.bfciter(), first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		template <class InputIterator>
		iterator insert(const_iterator position,
				InputIterator first, InputIterator last);

#if __cplusplus >= 201103L
		iterator insert(const_iterator position, T&& x);
		iterator insert(const_iterator position, initializer_list<T> il);

		template <class... Args>
		iterator emplace(const_iterator position, Args&&... args);

#endif
		iterator erase(const_iterator position)
		{
			int rc;
			rc = bfc_container_erase_element((bfc_contptr_t)&bfcvec,
							 position.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(position);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			int rc;
			rc = bfc_container_erase_range((bfc_contptr_t)&bfcvec,
					 first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(first);
		}


		void swap(vector<T,Allocator>&);
		void clear();

		int __invariants()  // required by test bench
		{
			return (1); // whatever this is ???
		}
	};

	template <class T, class Allocator>
	bool operator==(const vector<T,Allocator>& x,
			const vector<T,Allocator>& y)
	{
		return bfc_equal_object(x.contptr(), y.contptr());
	}

	template <class T, class Allocator>
	bool operator< (const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

	template <class T, class Allocator>
	bool operator!=(const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

	template <class T, class Allocator>
	bool operator> (const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

	template <class T, class Allocator>
	bool operator>=(const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

	template <class T, class Allocator>
	bool operator<=(const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

	// 23.3.6.6, specialized algorithms:
	template <class T, class Allocator>
	void swap(vector<T,Allocator>& x, vector<T,Allocator>& y);
}

#endif /* __cplusplus */
#endif /* _BFC_STRING_CXX_VECTOR_H_ */
