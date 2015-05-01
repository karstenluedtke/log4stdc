/**
 * @file      cxxmap.h
 *
 * @brief     C++ access to barefootc map implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-04-12
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_STRING_CXX_MAP_H_
#define _BFC_STRING_CXX_MAP_H_

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
#include "barefootc/pair.h"
#include "barefootc/map.h"
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
	template <class Key, class T, class Compare = std::less<Key>,
		  class Allocator = std::allocator<std::pair<const Key, T> > >
	class map {
	public:
		// types:
		typedef Key key_type;
		typedef T mapped_type;
		typedef BFC_PAIR(value_type_s, Key, T) value_type;
		typedef Compare key_compare;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef barefootc::iterator<T> iterator;
		typedef iterator const_iterator;
		typedef iterator reverse_iterator;
		typedef iterator const_reverse_iterator;
		typedef iterator local_iterator;
		typedef iterator const_local_iterator;
		class pair_iterator :
		    public std::iterator<std::input_iterator_tag, value_type>
		{
		    const value_type* p;
		public:
		    pair_iterator(const value_type* x) : p(x) {}
		    pair_iterator(const pair_iterator& mit) : p(mit.p) {}
		    pair_iterator& operator++() {++p;return *this;}
		    pair_iterator operator++(int) {pair_iterator tmp(*this); operator++(); return tmp;}
		    bool operator==(const pair_iterator& rhs) {return p==rhs.p;}
		    bool operator!=(const pair_iterator& rhs) {return p!=rhs.p;}
		    const value_type& operator*() {return *p;}
		};

#if 0
		class value_compare {
		friend class map;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) {}
		public:
			typedef bool result_type;
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator()(const value_type& x,
					const value_type& y) const {
				return comp(x.first, y.first);
			}
		};
#endif

	private:
		BFC_MAP(bfcmap_s, value_type) bfcmap;

		const Allocator saved_allocator;
		float saved_max_load_factor;

		void init_map(struct mempool *pool,
			      bfc_number_t& k, bfc_number_t& v)
		{
			BFC_MAP_INIT(&bfcmap, 100,
				(bfc_classptr_t) &bfc_int_pair_class, pool);
			l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
			L4SC_DEBUG(log, "%s: sizes: k %d, v %d, value_type %d, elem_size %d",
				__FUNCTION__, (int) sizeof(k), (int) sizeof(v), (int) sizeof(value_type), (int) bfcmap.elem_size);
		}

		void init_map(struct mempool *pool,
			      bfc_number_t& k, bfc_string_t& v)
		{
			BFC_MAP_INIT(&bfcmap, 100,
				(bfc_classptr_t) &bfc_int_string_pair_class,
				pool);
			l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
			L4SC_DEBUG(log, "%s: sizes: k %d, v %d, value_type %d, elem_size %d",
				__FUNCTION__, (int) sizeof(k), (int) sizeof(v), (int) sizeof(value_type), (int) bfcmap.elem_size);
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
		map(): saved_allocator(), saved_max_load_factor(1.0)
		{
			key_type k;
			mapped_type v;
			init_map(get_stdc_mempool(), k, v);
		}

		explicit map(const Allocator& a):
			saved_allocator(a), saved_max_load_factor(1.0)
		{
			key_type k;
			mapped_type v;
			init_map(get_stdc_mempool(), k, v);
		}


		map(pair_iterator first, pair_iterator lim):
			saved_allocator(), saved_max_load_factor(1.0)
		{
			key_type k;
			mapped_type v;

			init_map(get_stdc_mempool(), k, v);
			for (pair_iterator it(first); it != lim; it++) {
				value_type pair = *it;
				bfc_map_insert_objects((bfc_contptr_t)&bfcmap,
						(bfc_objptr_t)&pair.first,
						(bfc_objptr_t)&pair.second,
						NULL, 0);
			}
		}

		map(const map<Key,T,Compare,Allocator>& x)
		{
			bfc_init_map_copy(&bfcmap, sizeof(bfcmap),
					  get_stdc_mempool(), x.contptr());
		}

#if __cplusplus >= 201103L
		map(map<Key,T,Compare,Allocator>&& x);
#endif
		
		size_type bucket_count() const noexcept
		{
			int rc = bfc_object_length(&bfcmap);
			return ((rc > 0)? rc: 0);
		}

		float load_factor() const noexcept
		{
			float f = size();
			if (f > 0) {
				size_t count = bucket_count();
				size_t limit = bfc_map_load_limit(&bfcmap);
				if (count > limit) {
					count = limit;
				}
				if (count > 0) {
					f /= count;
				}
			}
			return (f);
		}

		float max_load_factor() const noexcept
		{
			return (saved_max_load_factor);
		}

		void max_load_factor ( float z )
		{
			saved_max_load_factor = z;
		}

		void rehash(size_type n)
		{
		}
		
#if 0
// 23.4.4.2, construct/copy/destroy:
explicit map(const Compare& comp = Compare(),
const Allocator& = Allocator());
template <class InputIterator>
map(InputIterator first, InputIterator last,
const Compare& comp = Compare(), const Allocator& = Allocator());
map(const map<Key,T,Compare,Allocator>& x);
map(map<Key,T,Compare,Allocator>&& x);
explicit map(const Allocator&);
map(const map&, const Allocator&);
map(map&&, const Allocator&);
map(initializer_list<value_type>,
const Compare& = Compare(),
const Allocator& = Allocator());

// capacity:
bool
empty() const;
size_type size() const;
size_type max_size() const;
// 23.4.4.3, element access:
T& operator[](const key_type& x);
T& operator[](key_type&& x);
T&
at(const key_type& x);
const T& at(const key_type& x) const;
// 23.4.4.4, modifiers:
template <class... Args> pair<iterator, bool> emplace(Args&&... args);
template <class... Args> iterator emplace_hint(const_iterator position, Args&&... args);
template <class P>
pair<iterator, bool> insert(P&& x);
template <class P>
iterator insert(const_iterator position, P&&);
template <class InputIterator>
void insert(InputIterator first, InputIterator last);
void insert(initializer_list<value_type>);
void swap(map<Key,T,Compare,Allocator>&);
// observers:
key_compare
key_comp() const;
value_compare value_comp() const;
// 23.4.4.5, map size_type operations:
iterator lower_bound(const key_type& x);
const_iterator lower_bound(const key_type& x) const;
iterator upper_bound(const key_type& x);
const_iterator upper_bound(const key_type& x) const;
};
template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator< (const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator> (const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key,T,Compare,Allocator>& x,
const map<Key,T,Compare,Allocator>& y);
// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void swap(map<Key,T,Compare,Allocator>& x,
map<Key,T,Compare,Allocator>& y);
}
#endif

		~map()
		{
			bfc_destroy(&bfcmap);
		}

		bfc_contptr_t contptr() const
		{
			void *map = const_cast<void *>((const void *) &bfcmap);
			return ((bfc_contptr_t) map);
		}

		map<Key,T,Compare,Allocator>&
			operator=(const map<Key,T,Compare,Allocator>& x)
		{
			bfc_destroy(&bfcmap);
			bfc_init_map_copy(&bfcmap, sizeof(bfcmap),
					  get_stdc_mempool(), x.contptr());
			return (*this);
		}

#if __cplusplus >= 201103L
		map<Key,T,Compare,Allocator>&
			operator=(map<Key,T,Compare,Allocator>&& x);
		map& operator=(initializer_list<value_type>);
#endif

		allocator_type get_allocator() const
		{
			return (saved_allocator);
		}

		// iterators:
		iterator begin()
		{
			bfc_iterator_t tmp;
			bfc_container_begin_iterator(contptr(),
						     &tmp, sizeof(tmp));
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		iterator end()
		{
			bfc_iterator_t tmp;
			bfc_container_end_iterator(contptr(),&tmp,sizeof(tmp));
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_iterator begin() const
		{
			bfc_iterator_t tmp;
			bfc_container_begin_iterator(contptr(),
						     &tmp, sizeof(tmp));
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_iterator end() const
		{
			bfc_iterator_t tmp;
			bfc_container_end_iterator(contptr(),&tmp,sizeof(tmp));
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
			bfc_container_begin_iterator(contptr(),
						     &tmp, sizeof(tmp));
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_iterator cend() const
		{
			bfc_iterator_t tmp;
			bfc_container_end_iterator(contptr(),&tmp,sizeof(tmp));
			barefootc::iterator<T> it(tmp);
			return (it);
		}

		const_reverse_iterator crbegin() const;
		const_reverse_iterator crend() const;

		local_iterator begin(size_type n)
		{
			iterator it = end();

			if ((n < bucket_count())
			 && (bfc_map_index_value(contptr(), n) != NULL)) {
				it = begin();
				bfc_iterator_set_position(it.bfciter(), n);
			}
			return ((local_iterator) it);
		}

		local_iterator end(size_type n)
		{
			iterator it = end();

			if ((n+1 < bucket_count())
			 && (bfc_map_index_value(contptr(), n) != NULL)) {
				bfc_iterator_set_position(it.bfciter(), n+1);
			}
			return ((local_iterator) it);
		}

		const_local_iterator begin(size_type n) const
		{
			return cbegin(n);
		}

		const_local_iterator end(size_type n) const
		{
			return cend(n);
		}

		const_local_iterator cbegin(size_type n) const
		{
			const_iterator it = cend();

			if ((n < bucket_count())
			 && (bfc_map_index_value(contptr(), n) != NULL)) {
				it = cbegin();
				bfc_iterator_set_position(it.bfciter(), n);
			}
			return ((const_local_iterator) it);
		}

		const_local_iterator cend(size_type n) const
		{
			const_iterator it = cend();

			if ((n+1 < bucket_count())
			 && (bfc_map_index_value(contptr(), n) != NULL)) {
				bfc_iterator_set_position(it.bfciter(), n+1);
			}
			return ((const_local_iterator) it);
		}

		//  capacity:
		size_type size() const
		{
			return (bfc_map_size(contptr()));
		}

		bool empty() const
		{
			return (size() == 0);
		}

		size_type max_size() const
		{
			return (bfc_container_max_size(contptr()));
		}

		size_type max_bucket_count() const
		{
			return (bfc_container_max_size(contptr()));
		}

		int __invariants()  // required by test bench
		{
			return (1); // whatever this is ???
		}

		// 23.4.4.3, element access:
		T& operator[](const key_type& x)
		{
			T *p;
			p = (T*) bfc_map_find_value(contptr(),
						    (bfc_cobjptr_t)&x);
			if (p == NULL) {
				int idx = bfc_map_insert_objects(contptr(),
							(bfc_objptr_t)&x,
							(bfc_objptr_t)0,
							NULL, 0);
				if (idx < 0) {
					char msg[80];
					snprintf(msg, sizeof(msg),
						"map entry create error %d",
						-idx);
					throw(std::runtime_error(msg));
				}
				p = (T*) bfc_map_index_value(contptr(),
								(size_t) idx);
			}
			return (*p);
		}

#if 0
		T& operator[](key_type&& x)
		{
			T *pval;
			pval = (T*) bfc_map_find_value(contptr, &x);
			if (pval == NULL) {
				throw(std::out_of_range("no such entry"));
			}
			return (*pval);
		}
#endif

		T& at(const key_type& x)
		{
			T *p;
			p = (T*) bfc_map_find_value(contptr(),
						    (bfc_cobjptr_t)&x);
			if (p == NULL) {
				throw(std::out_of_range("no such entry"));
			}
			return (*p);
		}

		const T& at(const key_type& x) const
		{
			T *p;
			p = (T*)bfc_map_find_value(contptr(),(bfc_cobjptr_t)&x);
			if (p == NULL) {
				throw(std::out_of_range("no such entry"));
			}
			return (*p);
		}

		iterator find(const key_type& x)
		{
			iterator it = begin();
			bfc_iterptr_t bfcit = it.bfciter();
			int rc = bfc_map_find_iter(contptr(),(bfc_cobjptr_t)&x,
						   bfcit, sizeof(*bfcit));
			return ((rc >= 0)? it: end());
		}

		const_iterator find(const key_type& x) const
		{
			const_iterator it = cbegin();
			bfc_iterptr_t bfcit = it.bfciter();
			int rc = bfc_map_find_iter(contptr(),(bfc_cobjptr_t)&x,
						   bfcit, sizeof(*bfcit));
			return ((rc >= 0)? it: cend());
		}

		std::pair<iterator,iterator> equal_range(const key_type& x)
		{
			iterator it = begin();
			std::pair<iterator,iterator> iters;
			bfc_iterptr_t bfcit = it.bfciter();
			int rc = bfc_map_find_iter(contptr(),(bfc_cobjptr_t)&x,
						   bfcit, sizeof(*bfcit));
			if ((rc >= 0) && (it.distance(end()) > 0)) {
				iters.first = it;
				iters.second = it;
				bfc_iterator_set_position(
					iters.second.bfciter(),
					bfc_iterator_position(bfcit) + 1);
			} else {
				iters.first = end();
				iters.second = end();
			}
			return (iters);
		}

		std::pair<const_iterator,const_iterator>
				equal_range(const key_type& x) const
		{
			const_iterator it = cbegin();
			std::pair<const_iterator,const_iterator> iters;
			bfc_iterptr_t bfcit = it.bfciter();
			int rc = bfc_map_find_iter(contptr(), (bfc_cobjptr_t)&x,
						   bfcit, sizeof(*bfcit));
			if ((rc >= 0) && (it.distance(cend()) > 0)) {
				iters.first = it;
				iters.second = it;
				bfc_iterator_set_position(
					iters.second.bfciter(),
					bfc_iterator_position(bfcit) + 1);
			} else {
				iters.first = cend();
				iters.second = cend();
			}
			return (iters);
		}

		size_type count(const key_type& x) const
		{
			int rc = bfc_map_count(contptr(), (bfc_cobjptr_t)&x);
			return ((rc > 0)? rc: 0);
		}

		size_type bucket(const key_type& x) const
		{
			unsigned bc = bucket_count();
			unsigned rc = bfc_map_keyhashcode(contptr(),
						     (bfc_cobjptr_t)&x);
			return ((rc < bc)? rc: rc % bc);
		}

		size_type bucket_size(const key_type& x) const
		{
			int rc = bfc_map_bucket_size(contptr(),
						     (bfc_cobjptr_t)&x);
			return ((rc > 0)? rc: 0);
		}

		// 23.4.4.4, modifiers:

		std::pair<iterator, bool> insert(const value_type& x)
		{
			std::pair<iterator, bool> r;
			bfc_iterptr_t it = r.first.bfciter();
			int rc = bfc_map_insert_objects(contptr(),
						(bfc_objptr_t)&x.first,
						(bfc_objptr_t)&x.second,
						it, sizeof(*it));
			r.second = (rc >= 0);
			return (r);
		}

		iterator insert(const_iterator position, const value_type& x)
		{
			iterator r(position);
			bfc_iterptr_t it = r.bfciter();
			int rc = bfc_map_insert_objects(contptr(),
						(bfc_objptr_t)&x.first,
						(bfc_objptr_t)&x.second,
						it, sizeof(*it));
			return (r);
		}

		void insert(pair_iterator first, pair_iterator last)
		{
			for (pair_iterator it(first); it != last; it++) {
				value_type pair = *it;
				bfc_map_insert_objects(contptr(),
						(bfc_objptr_t)&pair.first,
						(bfc_objptr_t)&pair.second,
						NULL, 0);
			}
		}

		size_type erase(const key_type& x)
		{
			int rc = bfc_map_erase_key(contptr(),(bfc_cobjptr_t)&x);
			return ((rc >= 0)? 1: 0);
		}

		iterator erase(const_iterator position)
		{
			iterator it(cend());
			bfc_map_erase_iter(contptr(), position.bfciter());
			return (it);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			iterator it(cend());
			if (first.distance(last) == 1) {
				bfc_map_erase_iter(contptr(), first.bfciter());
				iterator j(last);
				it = j;
			} else if (first.distance(last) == 0) {
				iterator j(first);
				it = j;
			} else if ((first == cbegin()) && (last == cend())) {
				bfc_container_resize(contptr(), 0, 0);
				iterator j(cend());
				it = j;
			} else {
				throw(std::out_of_range("distance != 1"));
			}
			return (it);
		}

		void clear()
		{
			bfc_container_resize(contptr(), 0, 0);
		}
	};

	template <class Key, class T, class Compare, class Allocator>
	bool operator==(const map<Key,T,Compare,Allocator>& x,
			const map<Key,T,Compare,Allocator>& y)
	{
		return bfc_equal_object(x.contptr(), y.contptr());
	}

	template <class Key, class T, class Compare, class Allocator>
	bool operator!=(const map<Key,T,Compare,Allocator>& x,
			const map<Key,T,Compare,Allocator>& y)
	{
		return !bfc_equal_object(x.contptr(), y.contptr());
	}
}

#endif /* __cplusplus */
#endif /* _BFC_STRING_CXX_MAP_H_ */
