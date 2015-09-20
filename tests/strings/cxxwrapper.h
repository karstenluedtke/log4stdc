/**
 * @file      cxxwrapper.h
 *
 * @brief     C++ access to barefootc string implementation.
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

#include <wchar.h>
#include <stdio.h>
#include <string.h>

#include "tests/iterators/cxxiterator.h"

#if __cplusplus >= 201103L
#else
#define noexcept
#endif

namespace barefootc {
	template<class charT,
		class chrtraits = std::char_traits<charT>,
		class Allocator = std::allocator<charT> >
	class basic_string {
	public:
		// types:
		typedef chrtraits traits_type;
		typedef typename chrtraits::char_type value_type;
		typedef Allocator allocator_type;
		//typedef typename allocator_traits<Allocator>::size_type
		//	size_type;
		//typedef typename allocator_traits<Allocator>::difference_type
		//	difference_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		//typedef typename allocator_traits<Allocator>::pointer pointer;
		//typedef typename allocator_traits<Allocator>::const_pointer
		//	const_pointer;
		typedef barefootc::iterator<charT> iterator;
		typedef iterator const_iterator;
		typedef iterator reverse_iterator;
		typedef iterator const_reverse_iterator;

		static const size_type npos = -1;
		
	private:
		typedef bfc_objptr_t strptrT;
		typedef bfc_cobjptr_t cstrptrT;

		typedef bfc_string_classptr_t classptrT;

		bfc_string_t bfcstr;

		Allocator saved_allocator;
		
		static void throw_resize_error(int rc)
		{
			switch (rc) {
			case EINVAL:
				throw(std::length_error("bad length"));
				return;
			case ENOMEM:
				throw(std::bad_alloc());
				return;
			default:
				char m[80];
				snprintf(m, sizeof(m), "substr error %d", rc);
				throw(std::runtime_error(m));
			}
		}

		static void throw_substr_error(int rc)
		{
			switch (rc) {
			case ERANGE:
				throw(std::out_of_range("bad substr position"));
				return;
			default:
				char m[80];
				snprintf(m, sizeof(m), "substr error %d", rc);
				throw(std::runtime_error(m));
			}
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

		static int
		init_string(void *buf, size_t bufsize,
			    const char *s, size_t n)
		{
			return bfc_init_string_buffer(buf, bufsize, 0, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const char *s, size_t n)
		{
			return bfc_init_basic_string_buffer(buf, bufsize,
								pool, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const char *s)
		{
			return bfc_init_basic_string_c_str(buf,bufsize,pool,s);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, size_t n, char c)
		{
			return bfc_init_basic_string_fill(buf, bufsize,
								pool, n, c);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const char *s,
			    const basic_string& str)
		{
			int rc;
			rc = bfc_init_basic_string_copy(buf, bufsize, pool,
								&str.bfcstr);
			return (rc);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const char *s,
			    const basic_string& str, size_t pos, size_t n)
		{
			int rc;
			rc = bfc_init_basic_string_substr(buf, bufsize, pool,
							&str.bfcstr, pos, n);
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const char *s,
			    const_iterator begin, const_iterator end)
		{
			int rc;
			rc = bfc_init_basic_string_range(buf, bufsize,
					pool, begin.bfciter(), end.bfciter());
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    const wchar_t *s, size_t n)
		{
			return bfc_init_wstring_buffer(buf, bufsize, 0, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const wchar_t *s, size_t n)
		{
			return bfc_init_basic_wstring_buffer(buf, bufsize,
								pool, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const wchar_t *s)
		{
			return bfc_init_basic_wstring_c_str(buf,bufsize,pool,s);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, size_t n, wchar_t c)
		{
			return bfc_init_basic_wstring_fill(buf, bufsize,
								pool, n, c);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const wchar_t *s,
			    const basic_string& str)
		{
			int rc;
			rc = bfc_init_basic_wstring_copy(buf, bufsize, pool,
						(bfc_cobjptr_t) &str.bfcstr);
			return (rc);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const wchar_t *s,
			    const basic_string& str, size_t pos, size_t n)
		{
			int rc;
			rc = bfc_init_basic_wstring_substr(buf, bufsize, pool,
					(bfc_cobjptr_t) &str.bfcstr, pos, n);
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    bfc_mempool_t pool, const wchar_t *s,
			    const_iterator begin, const_iterator end)
		{
			int rc;
			rc = bfc_init_basic_wstring_range(buf, bufsize,
					pool, begin.bfciter(), end.bfciter());
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

	public:
		// 21.4.2, construct/copy/destroy:
		basic_string(): saved_allocator()
		{
			const charT *s = 0;
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s, 0);
		}

		explicit basic_string(const Allocator& a): saved_allocator(a)
		{
			const charT *s = 0;
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s, 0);
		}

		basic_string(const basic_string& str):
			saved_allocator(str.saved_allocator)
		{
			const charT *s = str.data();
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s, str);
		}

#if __cplusplus >= 201103L
		//basic_string(basic_string&& str) noexcept;
#endif

		basic_string(const basic_string& str,
			size_type pos, size_type n = npos): saved_allocator()
		{
			const charT *s = str.data();
			init_string(&bfcstr, sizeof(bfcstr),
				    get_stdc_mempool(), s, str, pos, n);
		}

		basic_string(const basic_string& str,
			size_type pos, size_type n,
			const Allocator& a): saved_allocator(a)
		{
			const charT *s = str.data();
			init_string(&bfcstr, sizeof(bfcstr),
				    get_stdc_mempool(), s, str, pos, n);
		}

		basic_string(const basic_string& str,
			size_type pos, const Allocator& a): saved_allocator(a)
		{
			const charT *s = str.data();
			init_string(&bfcstr, sizeof(bfcstr),
				    get_stdc_mempool(), s, str, pos, npos);
		}

		basic_string(const charT* s, size_type n):
			saved_allocator()
		{
			init_string(&bfcstr, sizeof(bfcstr), s, n);
		}

		basic_string(const charT* s, size_type n, const Allocator& a):
			saved_allocator(a)
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), s, n);
		}

		basic_string(const charT* s):
			saved_allocator()
		{
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s);
		}

		basic_string(const charT* s, const Allocator& a):
			saved_allocator(a)
		{
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s);
		}

		basic_string(size_type n, charT c):
			saved_allocator()
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), n, c);
		}

		basic_string(size_type n, charT c, const Allocator& a):
			saved_allocator(a)
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), n, c);
		}

		//template<class InputIterator>
		//basic_string(InputIterator begin, InputIterator end,
		//		const Allocator& a = Allocator());

		basic_string(const_iterator begin, const_iterator end,
				const Allocator& a): saved_allocator(a)
		{
			const charT *s = &(begin[0]);
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), s, begin, end);
		}

		basic_string(const_iterator begin, const_iterator end):
				saved_allocator(0)
		{
			const charT *s = &(begin[0]);
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), s, begin, end);
		}

#if __cplusplus >= 201103L
		// basic_string(initializer_list<charT>, const Allocator& = Allocator());
#endif

		basic_string(const basic_string& str, const Allocator& a):
			saved_allocator(a)
		{
			const charT *s = str.data();
			const size_t n = str.size();
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s, n);
		}

		// basic_string(basic_string&&, const Allocator&);

		~basic_string()
		{
#ifndef OMIT_BFC_STRING_DESTRUCTOR	/* just for speeding up tests */
			bfc_destroy(&bfcstr);
#endif
		}

		basic_string& operator=(const basic_string& str)
		{
			return assign(str);
		}

		basic_string& operator=(const charT* s)
		{
			return assign(s);
		}

		basic_string& operator=(charT c)
		{
			return assign(1, c);
		}

#if __cplusplus >= 201103L
		// basic_string& operator=(basic_string&& str) noexcept ;
		// basic_string& operator=(initializer_list<charT>);
#endif
		
		// 21.4.3, iterators:
		iterator begin() noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_begin_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}
			
		const_iterator begin() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_begin_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		iterator end() noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_end_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_iterator end() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_end_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		reverse_iterator rbegin() noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_begin_iterator(&bfcstr,
							  &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_reverse_iterator rbegin() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_begin_iterator(&bfcstr,
							  &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		reverse_iterator rend() noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_end_iterator(&bfcstr,
							&tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_reverse_iterator rend() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_end_iterator(&bfcstr,
							&tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}


		const_iterator cbegin() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_begin_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_iterator cend() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_end_iterator(&bfcstr, &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_reverse_iterator crbegin() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_begin_iterator(&bfcstr,
							  &tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		const_reverse_iterator crend() const noexcept
		{
			bfc_iterator_t tmp;
			bfc_string_reverse_end_iterator(&bfcstr,
							&tmp, sizeof(tmp));
			iterator it(tmp);
			return (it);
		}

		
		// 21.4.4, capacity:
		size_type size() const noexcept
		{
			return bfc_strlen(&bfcstr);
		}

		size_type length() const noexcept
		{
			return bfc_strlen(&bfcstr);
		}

		size_type max_size() const noexcept
		{
			return bfc_string_max_size(&bfcstr);
		}

		void resize(size_type n, charT c)
		{
			int rc;
			rc = bfc_string_resize(&bfcstr, n, c);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		void resize(size_type n)
		{
			int rc;
			rc = bfc_string_resize(&bfcstr, n, 0);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		size_type capacity() const noexcept
		{
			return bfc_string_capacity(&bfcstr);
		}

		void reserve(size_type res_arg = 0)
		{
			int rc;
			rc = bfc_string_reserve(&bfcstr, res_arg);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		void shrink_to_fit()
		{
			size_type len = length();
			bfc_string_reserve(&bfcstr, len);
		}

		void clear() noexcept
		{
			bfc_string_resize(&bfcstr, 0, 0);
		}

		bool empty() const noexcept
		{
			size_type len = length();
			return (len == 0);
		}
		
		// 21.4.5, element access:
		const_reference operator[](size_type pos) const noexcept
		{
			strptrT s = const_cast<strptrT>(&bfcstr);
			const charT *p;

			p = (const charT *) bfc_string_index(s, pos);
			return (*p);
		}

		reference operator[](size_type pos) noexcept
		{
			charT *p;

			p = (charT *) bfc_string_index(&bfcstr, pos);
			return (*p);
		}

		const_reference at(size_type n) const
		{
			strptrT s = const_cast<strptrT>(&bfcstr);
			const charT *p;

			p = (const charT *) bfc_string_index(s, n);
			if ((p == NULL) || (n >= length())) {
				throw(std::out_of_range("bad position"));
			}
			return (*p);
		}

		reference at(size_type n)
		{
			charT *p;

			p = (charT *) bfc_string_index(&bfcstr, n);
			if ((p == NULL) || (n >= length())) {
				throw(std::out_of_range("bad position"));
			}
			return (*p);
		}

		const charT& front() const noexcept
		{
			return at(0);
		}

		charT& front() noexcept
		{
			return at(0);
		}

		const charT& back() const noexcept
		{
			size_t n = length();
			return at((n > 0)? (n-1): 0);
		}

		charT& back() noexcept
		{
			size_t n = length();
			return at((n > 0)? (n-1): 0);
		}

		
		// 21.4.6, modifiers:
		basic_string& operator+=(const basic_string& str)
		{
			bfc_string_append(&bfcstr, &str.bfcstr);
			return(*this);
		}

		basic_string& operator+=(const charT* s)
		{
			bfc_string_append_c_str(&bfcstr, s);
			return(*this);
		}

		basic_string& operator+=(charT c)
		{
			bfc_string_push_back(&bfcstr, c);
			return(*this);
		}

#if __cplusplus >= 201103L
		// basic_string& operator+=(initializer_list<charT>);
#endif

		basic_string& append(const basic_string& str)
		{
			int rc;
			rc = bfc_string_append(&bfcstr, &str.bfcstr);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& append(const basic_string& str,
					size_type pos, size_type n = npos)
		{
			int rc;
			rc = bfc_string_append_substr(&bfcstr,
						      &str.bfcstr, pos, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& append(const charT* s, size_type n)
		{
			int rc;
			rc = bfc_string_append_buffer(&bfcstr, s, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& append(const charT* s)
		{
			int rc;
			rc = bfc_string_append_c_str(&bfcstr, s);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& append(size_type n, charT c)
		{
			int rc;
			rc = bfc_string_append_fill(&bfcstr, n, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		//template<class InputIterator>
		//basic_string& append(InputIterator first, InputIterator last);

		basic_string& append(const_iterator first, const_iterator last)
		{
			int rc;
			rc = bfc_string_append_range(&bfcstr,
					      first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

#if __cplusplus >= 201103L
		// basic_string& append(initializer_list<charT>);
#endif

		void push_back(charT c)
		{
			int rc;
			rc = bfc_string_push_back(&bfcstr, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
		}

		basic_string& assign(const basic_string& str)
		{
			int rc;
			rc = bfc_string_assign(&bfcstr, &str.bfcstr);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		// basic_string& assign(basic_string&& str) noexcept;
		basic_string& assign(const basic_string& str,
					size_type pos, size_type n = npos)
		{
			int rc;
			rc = bfc_string_assign_substr(&bfcstr,
						      &str.bfcstr, pos, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& assign(const charT* s, size_type n)
		{
			int rc;
			rc = bfc_string_assign_buffer(&bfcstr, s, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& assign(const charT* s)
		{
			int rc;
			rc = bfc_string_assign_c_str(&bfcstr, s);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& assign(size_type n, charT c)
		{
			int rc;
			rc = bfc_string_assign_fill(&bfcstr, n, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		//template<class InputIterator>
		//basic_string& assign(InputIterator first, InputIterator last);

		basic_string& assign(const_iterator first, const_iterator last)
		{
			int rc;
			rc = bfc_string_assign_range(&bfcstr,
					      first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

#if __cplusplus >= 201103L
		// basic_string& assign(initializer_list<charT>);
#endif
		basic_string& insert(size_type pos1, const basic_string& str)
		{
			int rc;
			rc = bfc_string_insert(&bfcstr, pos1, &str.bfcstr);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& insert(size_type pos1, const basic_string& str,
					size_type pos2, size_type n = npos)
		{
			int rc;
			rc = bfc_string_insert_substr(&bfcstr, pos1,
						      &str.bfcstr, pos2, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& insert(size_type pos, const charT* s, size_type n)
		{
			int rc;
			rc = bfc_string_insert_buffer(&bfcstr, pos, s, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& insert(size_type pos, const charT* s)
		{
			int rc;
			rc = bfc_string_insert_c_str(&bfcstr, pos, s);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& insert(size_type pos, size_type n, charT c)
		{
			int rc;
			rc = bfc_string_insert_fill(&bfcstr, pos, n, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		iterator insert(const_iterator p, charT c)
		{
			iterator it(p);
			int rc;
			rc = bfc_string_insert_char(&bfcstr, it.bfciter(), c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		iterator insert(const_iterator p, size_type n, charT c)
		{
			iterator it(p);
			int rc;
			rc = bfc_string_insert_fillit(&bfcstr,
						      it.bfciter(), n, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

		//template<class InputIterator>
		//iterator insert(const_iterator p,
		//		InputIterator first, InputIterator last);
		iterator insert(const_iterator p,
				const_iterator first, const_iterator last)
		{
			iterator it(p);
			int rc;
			rc = bfc_string_insert_range(&bfcstr, it.bfciter(),
					   first.bfciter(), last.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(it);
		}

#if __cplusplus >= 201103L
		// iterator insert(const_iterator p, initializer_list<charT>);
#endif

		basic_string& erase(size_type pos = 0, size_type n = npos)
		{
			int rc;
			rc = bfc_string_erase_seq(&bfcstr, pos, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return (*this);
		}

		iterator erase(const_iterator p)
		{
			iterator it(p);
			int rc;
			rc = bfc_string_erase_char(&bfcstr, it.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return (it);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			iterator f(first);
			iterator l(last);
			int rc;
			rc = bfc_string_erase_range(&bfcstr,
						    f.bfciter(), l.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return (f);
		}

		void pop_back() noexcept
		{
			bfc_string_pop_back(&bfcstr);
		}

		basic_string& replace(size_type pos1, size_type n1,
					const basic_string& str)
		{
			int rc;
			rc = bfc_string_replace(&bfcstr, pos1, n1, &str.bfcstr);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(size_type pos1, size_type n1,
					const basic_string& str,
					size_type pos2, size_type n2 = npos)
		{
			int rc;
			rc = bfc_string_replace_substr(&bfcstr, pos1, n1,
						       &str.bfcstr, pos2, n2);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(size_type pos, size_type n1,
					const charT* s, size_type n2)
		{
			int rc;
			rc = bfc_string_replace_buffer(&bfcstr, pos, n1, s, n2);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(size_type pos, size_type n1,
					const charT* s)
		{
			int rc;
			rc = bfc_string_replace_c_str(&bfcstr, pos, n1, s);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(size_type pos, size_type n1,
					size_type n2, charT c)
		{
			int rc;
			rc = bfc_string_replace_fill(&bfcstr, pos, n1, n2, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(const_iterator i1, const_iterator i2,
					const basic_string& str)
		{
			int rc;
			rc = bfc_string_replace_range(&bfcstr,
						i1.bfciter(), i2.bfciter(),
						&str.bfcstr);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(const_iterator i1, const_iterator i2,
					const charT* s, size_type n)
		{
			int rc;
			rc = bfc_string_replace_range_buffer(&bfcstr,
					i1.bfciter(), i2.bfciter(), s, n);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(const_iterator i1, const_iterator i2,
					const charT* s)
		{
			int rc;

			rc = bfc_string_replace_range_c_str(&bfcstr,
					i1.bfciter(), i2.bfciter(), s);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		basic_string& replace(const_iterator i1, const_iterator i2,
					size_type n, charT c)
		{
			int rc;
			rc = bfc_string_replace_range_fill(&bfcstr,
					i1.bfciter(), i2.bfciter(), n, c);
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}

		//template<class InputIterator>
		//basic_string& replace(const_iterator i1, const_iterator i2,
		//			InputIterator j1, InputIterator j2);

		basic_string& replace(const_iterator i1, const_iterator i2,
					const_iterator j1, const_iterator j2)
		{
			int rc;
			rc = bfc_string_replace_ranges(&bfcstr,
					i1.bfciter(), i2.bfciter(),
					j1.bfciter(), j2.bfciter());
			if (rc < 0) {
				throw_replace_error(-rc);
			}
			return(*this);
		}
#if __cplusplus >= 201103L
		// basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
#endif

		size_type copy(charT* s, size_type n, size_type pos = 0) const
		{
			size_type rc;
			rc = bfc_string_copy(&bfcstr, s, n, pos);
			if ((((ptrdiff_t)rc) < 0) || (rc > n)) {
				short err = -rc;
				rc = 0;
				throw_replace_error(err);
			}
			return (rc);
		}

		void swap(basic_string& str) noexcept
		{
			bfc_string_swap(&bfcstr, &str.bfcstr);
		}
		
		// 21.4.7, string operations:
		const charT* c_str() const noexcept
		{
			return (const charT *) bfc_string_data(&bfcstr);
		}

		const charT* data() const noexcept
		{
			return (const charT *) bfc_string_data(&bfcstr);
		}

		allocator_type get_allocator() const noexcept
		{
			return (saved_allocator);
		}

		size_type find (const basic_string& str, size_type pos = 0)
								const noexcept
		{
			return bfc_string_find_bfstr(&bfcstr, &str.bfcstr, pos);
		}

		size_type find (const charT* s, size_type pos, size_type n)
								const noexcept
		{
			return bfc_string_find_buffer(&bfcstr, s, pos, n);
		}

		size_type find (const charT* s,size_type pos = 0)const noexcept
		{
			return bfc_string_find_c_str(&bfcstr, s, pos);
		}

		size_type find (charT c, size_type pos = 0) const noexcept
		{
			return bfc_string_find_char(&bfcstr, c, pos);
		}

		size_type rfind(const basic_string& str, size_type pos = npos)
								const noexcept
		{
			return bfc_string_rfind_bfstr(&bfcstr,&str.bfcstr,pos);
		}

		size_type rfind(const charT* s, size_type pos, size_type n)
								const noexcept
		{
			return bfc_string_rfind_buffer(&bfcstr, s, pos, n);
		}

		size_type rfind(const charT* s, size_type pos = npos)
								const noexcept
		{
			return bfc_string_rfind_c_str(&bfcstr, s ,pos);
		}

		size_type rfind(charT c, size_type pos = npos) const noexcept
		{
			return bfc_string_rfind_char(&bfcstr, c, pos);
		}

		size_type find_first_of(const basic_string& str,
					size_type pos = 0) const noexcept
		{
			return bfc_string_find_first_of_bfstr(&bfcstr,
							      &str.bfcstr, pos);
		}

		size_type find_first_of(const charT* s, size_type pos,
					size_type n) const noexcept
		{
			return bfc_string_find_first_of_buffer(&bfcstr,
								s, pos, n);
		}

		size_type find_first_of(const charT* s, size_type pos = 0)
								const noexcept
		{
			return bfc_string_find_first_of_c_str(&bfcstr, s, pos);
		}

		size_type find_first_of(charT c, size_type pos = 0)
								const noexcept
		{
			return bfc_string_find_first_of_char(&bfcstr, c, pos);
		}

		size_type find_last_of (const basic_string& str,
					size_type pos = npos) const noexcept
		{
			return bfc_string_find_last_of_bfstr(&bfcstr,
							     &str.bfcstr, pos);
		}

		size_type find_last_of (const charT* s, size_type pos,
					size_type n) const noexcept
		{
			return bfc_string_find_last_of_buffer(&bfcstr,
								s, pos, n);
		}

		size_type find_last_of (const charT* s, size_type pos = npos)
								const noexcept
		{
			return bfc_string_find_last_of_c_str(&bfcstr, s, pos);
		}

		size_type find_last_of (charT c, size_type pos = npos)
								const noexcept
		{
			return bfc_string_find_last_of_char(&bfcstr, c, pos);
		}

		size_type find_first_not_of(const basic_string& str,
					size_type pos = 0) const noexcept
		{
			return bfc_string_find_first_not_of_bfstr(&bfcstr,
							      &str.bfcstr, pos);
		}

		size_type find_first_not_of(const charT* s,
				size_type pos, size_type n) const noexcept
		{
			return bfc_string_find_first_not_of_buffer(&bfcstr,
								   s, pos, n);
		}

		size_type find_first_not_of(const charT* s, size_type pos = 0)
								const noexcept
		{
			return bfc_string_find_first_not_of_c_str(&bfcstr,
								  s, pos);
		}

		size_type find_first_not_of(charT c, size_type pos = 0)
								const noexcept
		{
			return bfc_string_find_first_not_of_char(&bfcstr,
								 c, pos);
		}

		size_type find_last_not_of (const basic_string& str,
					size_type pos = npos) const noexcept
		{
			return bfc_string_find_last_not_of_bfstr(&bfcstr,
							     &str.bfcstr, pos);
		}

		size_type find_last_not_of (const charT* s,
				size_type pos, size_type n) const noexcept
		{
			return bfc_string_find_last_not_of_buffer(&bfcstr,
								  s, pos, n);
		}

		size_type find_last_not_of (const charT* s, size_type pos=npos)
								const noexcept
		{
			return bfc_string_find_last_not_of_c_str(&bfcstr,
								 s, pos);
		}

		size_type find_last_not_of (charT c, size_type pos = npos)
								const noexcept
		{
			return bfc_string_find_last_not_of_char(&bfcstr,
								c, pos);
		}

		basic_string substr(size_type pos = 0, size_type n = npos) const
		{
			basic_string s(saved_allocator);
			int rc;
			rc = bfc_string_shared_substr(&bfcstr, pos, n,
					       &s.bfcstr, sizeof(s.bfcstr));
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return s;
		}

		int compare(const basic_string& str) const noexcept
		{
			int rc;
			rc = bfc_string_compare_bfstr(&bfcstr, &str.bfcstr);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1,
			    const basic_string& str) const
		{
			int rc;
			rc = bfc_string_compare_substr(&bfcstr, pos1, n1,
						       &str.bfcstr);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		/* not in C++ standard */
		int compare(size_type pos1, size_type n1,
			const basic_string& str, size_type pos2) const
		{
			int rc;
			rc = bfc_string_compare_substrs(&bfcstr, pos1, n1,
						    &str.bfcstr, pos2, npos);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1,
			const basic_string& str,size_type pos2,size_type n2)
									const
		{
			int rc;
			rc = bfc_string_compare_substrs(&bfcstr, pos1, n1,
						    &str.bfcstr, pos2, n2);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(const charT* s) const noexcept
		{
			int rc;
			rc = bfc_string_compare_c_str(&bfcstr, s);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1, const charT* s) const
		{
			int rc;
			rc = bfc_string_compare_substr_c_str(&bfcstr, pos1, n1,
							     s);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1,
			    const charT* s, size_type n2) const
		{
			int rc;
			rc = bfc_string_compare_buffer(&bfcstr, pos1, n1,
							s, n2);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}


		int __invariants()  // required by test bench
		{
			return (1); // whatever this is ???
		}
	};

	/*
	 * Non-members
	 */

	/* operator+ string */
	template <class charT, class traits, class Alloc>
	basic_string<charT,traits,Alloc>
	operator+(const basic_string<charT,traits,Alloc>& lhs,
		  const basic_string<charT,traits,Alloc>& rhs)
	{
		size_t len = lhs.length() + rhs.length() + 1;
		charT *buf = lhs.get_allocator().allocate(len);
		basic_string<charT> res(buf, len, lhs.get_allocator());
		res.clear();
		res.assign(lhs);
		res.append(rhs);
		return (res);
	}

	/* operator+ c-string */
	template <class charT, class traits, class Alloc>
	basic_string<charT,traits,Alloc>
	operator+(const basic_string<charT,traits,Alloc>& lhs, const charT* rhs)
	{
		size_t len = lhs.length() + traits::length(rhs) + 1;
		charT *buf = lhs.get_allocator().allocate(len);
		basic_string<charT> res(buf, len, lhs.get_allocator());
		res.clear();
		res.assign(lhs);
		res.append(rhs);
		return (res);
	}

	template <class charT, class traits, class Alloc>
	basic_string<charT,traits,Alloc>
	operator+(const charT* lhs, const basic_string<charT,traits,Alloc>& rhs)
	{
		size_t len = traits::length(lhs) + rhs.length() + 1;
		charT *buf = rhs.get_allocator().allocate(len);
		basic_string<charT> res(buf, len, rhs.get_allocator());
		res.clear();
		res.assign(lhs);
		res.append(rhs);
		return (res);
	}


	/* operator+ character */
	template <class charT, class traits, class Alloc>
	basic_string<charT,traits,Alloc>
	operator+(const basic_string<charT,traits,Alloc>& lhs, charT rhs)
	{
		size_t len = lhs.length() + 2;
		charT *buf = lhs.get_allocator().allocate(len);
		basic_string<charT> res(buf, len, lhs.get_allocator());
		res.clear();
		res.assign(lhs);
		res.append(rhs);
		return (res);
	}

	template <class charT, class traits, class Alloc>
	basic_string<charT,traits,Alloc>
	operator+(charT lhs, const basic_string<charT,traits,Alloc>& rhs)
	{
		size_t len = rhs.length() + 2;
		charT *buf = rhs.get_allocator().allocate(len);
		basic_string<charT> res(buf, len, rhs.get_allocator());
		res.clear();
		res.assign(lhs);
		res.append(rhs);
		return (res);
	}


	template <class charT, class traits, class Alloc>
	bool
	operator== (const basic_string<charT,traits,Alloc>& lhs,
                    const basic_string<charT,traits,Alloc>& rhs)
	{
		return (lhs.compare(rhs) == 0);
	}

	template <class charT, class traits, class Alloc>
	bool
	operator== (const charT* lhs,
		    const basic_string<charT,traits,Alloc>& rhs)
	{
		return (rhs.compare(lhs) == 0);
	}

	template <class charT, class traits, class Alloc>
	bool
	operator== (const basic_string<charT,traits,Alloc>& lhs,
		    const charT* rhs)
	{
		return (lhs.compare(rhs) == 0);
	}



	template <class charT, class traits, class Alloc>
	bool operator!= (const basic_string<charT,traits,Alloc>& lhs,
                   const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator!= (const charT* lhs, const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator!= (const basic_string<charT,traits,Alloc>& lhs, const charT* rhs);


	template <class charT, class traits, class Alloc>
	bool operator<  (const basic_string<charT,traits,Alloc>& lhs,
                   const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator<  (const charT* lhs, const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator<  (const basic_string<charT,traits,Alloc>& lhs, const charT* rhs);


	template <class charT, class traits, class Alloc>
	bool operator<= (const basic_string<charT,traits,Alloc>& lhs,
                   const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator<= (const charT* lhs, const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator<= (const basic_string<charT,traits,Alloc>& lhs, const charT* rhs);


	template <class charT, class traits, class Alloc>
	bool operator>  (const basic_string<charT,traits,Alloc>& lhs,
                   const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator>  (const charT* lhs, const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator>  (const basic_string<charT,traits,Alloc>& lhs, const charT* rhs);


	template <class charT, class traits, class Alloc>
	bool operator>= (const basic_string<charT,traits,Alloc>& lhs,
                   const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator>= (const charT* lhs, const basic_string<charT,traits,Alloc>& rhs);
	template <class charT, class traits, class Alloc>
	bool operator>= (const basic_string<charT,traits,Alloc>& lhs, const charT* rhs);

}

#endif /* __cplusplus */
#endif /* _BFC_STRING_CXX_WRAPPER_H_ */
