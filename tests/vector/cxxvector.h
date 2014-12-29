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

#include "tests/strings/cxxwrapper.h"

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
		typedef size_t size_type; // see 23.2
		typedef ptrdiff_t difference_type;// see 23.2
		typedef Allocator allocator_type;
		//typedef typename allocator_traits<Allocator>::pointer pointer;
		//typedef typename allocator_traits<Allocator>::const_pointer const_pointer;
		//typedef implementation-defined iterator; // see 23.2
		//typedef implementation-defined const_iterator; // see 23.2
		//typedef std::reverse_iterator<iterator> reverse_iterator;
		//typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef barefootc::iterator<vector,T> iterator;
		typedef iterator const_iterator;
		typedef iterator reverse_iterator;
		typedef iterator const_reverse_iterator;

	private:
		BFC_VECTOR(bfcvec_s, T, 4) bfcvec;

	public:
		// 23.3.6.2, construct/copy/destroy:
		vector()
		{
			struct mempool *pool = get_stdc_mempool();
			BFC_VECTOR_INIT_POOL(&bfcvec, pool);
			bfc_init_vector_class(&bfcvec, sizeof(bfcvec), pool);
		}

		explicit vector(const Allocator&)
		{
			struct mempool *pool = get_stdc_mempool();
			BFC_VECTOR_INIT_POOL(&bfcvec, pool);
			bfc_init_vector_class(&bfcvec, sizeof(bfcvec), pool);
		}

		explicit vector(size_type n)
		{
			struct mempool *pool = get_stdc_mempool();
			BFC_VECTOR_INIT_POOL(&bfcvec, pool);
			bfc_init_vector_class(&bfcvec, sizeof(bfcvec), pool);
		}

		vector(size_type n, const T& value,
			const Allocator& = Allocator());
		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const Allocator& = Allocator());
		vector(const vector<T,Allocator>& x);
		vector(const vector&, const Allocator&);
#if __cplusplus >= 201103L
		vector(vector&&);
		vector(vector&&, const Allocator&);
		vector(initializer_list<T>, const Allocator& = Allocator());
#endif
		~vector()
		{
			bfc_destroy(&bfcvec);
		}

		vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
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
		allocator_type get_allocator() const;

		// iterators:
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		const_reverse_iterator crbegin() const;
		const_reverse_iterator crend() const;

		// 23.3.6.3, capacity:
		size_type size() const
		{
			return (bfc_object_length(&bfcvec));
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
			return (at(0));
		}

		const_reference front() const
		{
			return (at(0));
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
		T* data() noexcept;
		const T* data() const noexcept;

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

		iterator insert(const_iterator position, const T& x);
		iterator insert(const_iterator position,
				size_type n, const T& x);
		template <class InputIterator>
		iterator insert(const_iterator position,
				InputIterator first, InputIterator last);

#if __cplusplus >= 201103L
		iterator insert(const_iterator position, T&& x);
		iterator insert(const_iterator position, initializer_list<T> il);

		template <class... Args>
		iterator emplace(const_iterator position, Args&&... args);

#endif
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);

		void swap(vector<T,Allocator>&);
		void clear();
	};

	template <class T, class Allocator>
	bool operator==(const vector<T,Allocator>& x,
			const vector<T,Allocator>& y);

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
