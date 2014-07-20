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

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"

#define noexcept

namespace barefootc {
	template<class charT,
		class chrtraits = std::char_traits<charT>,
		class Allocator = std::allocator<charT> > class basic_string {
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
		class iterator {
		};
		typedef iterator const_iterator;
		typedef std::reverse_iterator<iterator>
			reverse_iterator;
		typedef std::reverse_iterator<const_iterator>
			const_reverse_iterator;

		static const size_type npos = -1;
		
	private:
		struct wrapped_string;
		typedef struct wrapped_string *strptrT;
		typedef const struct wrapped_string *cstrptrT;

		struct wrapped_string_class;
		typedef const struct wrapped_string_class *classptrT;

		struct wrapped_string {
			BFC_STRINGHDR(classptrT,charT)
			void *_lib_spare[4];
		};

		struct wrapped_string_class {
			BFC_STRING_CLASS_DEF(classptrT,strptrT,cstrptrT,charT)
		};

		struct wrapped_string bfcstr;
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
				throw(std::runtime_error("substr error " + rc));
			}
		}

		static void throw_substr_error(int rc)
		{
			switch (rc) {
			case ERANGE:
				throw(std::out_of_range("bad substr position"));
				return;
			default:
				throw(std::runtime_error("substr error " + rc));
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
			    struct mempool *pool, const char *s, size_t n)
		{
			return bfc_init_basic_string_buffer(buf, bufsize,
								pool, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    struct mempool *pool, size_t n, char c)
		{
			return bfc_init_basic_string_fill(buf, bufsize,
								pool, n, c);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    const wchar_t *s, size_t n)
		{
			return bfc_init_wstring_buffer(buf, bufsize, 0, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    struct mempool *pool, const wchar_t *s, size_t n)
		{
			return bfc_init_basic_wstring_buffer(buf, bufsize,
								pool, s, n);
		}

		static int
		init_string(void *buf, size_t bufsize,
			    struct mempool *pool, size_t n, wchar_t c)
		{
			return bfc_init_basic_wstring_fill(buf, bufsize,
								pool, n, c);
		}

	public:
		// 21.4.2, construct/copy/destroy:
		basic_string(): saved_allocator()
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), bfcstr.buf, 0);
		}

		explicit basic_string(const Allocator& a): saved_allocator(a)
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), bfcstr.buf, 0);
		}

		basic_string(const basic_string& str): saved_allocator()
		{
			const charT *s = str.data();
			const size_t n = str.size();
			init_string(&bfcstr, sizeof(bfcstr),
					get_stdc_mempool(), s, n);
		}

		//basic_string(basic_string&& str) noexcept;
		basic_string(const basic_string& str, size_type pos, size_type n = npos, const Allocator& a = Allocator());

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
				get_stdc_mempool(), s, chrtraits::length(s));
		}

		basic_string(const charT* s, const Allocator& a):
			saved_allocator(a)
		{
			init_string(&bfcstr, sizeof(bfcstr),
				get_stdc_mempool(), s, chrtraits::length(s));
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

		template<class InputIterator>
		basic_string(InputIterator begin, InputIterator end, const Allocator& a = Allocator());
		// basic_string(initializer_list<charT>, const Allocator& = Allocator());
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
		}

		basic_string& operator=(const basic_string& str);
		// basic_string& operator=(basic_string&& str) noexcept ;
		basic_string& operator=(const charT* s);
		basic_string& operator=(charT c);
		// basic_string& operator=(initializer_list<charT>);
		
		// 21.4.3, iterators:
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;
		
		// 21.4.4, capacity:
		size_type size() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					size, (&bfcstr),
					0);
		}

		size_type length() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					size, (&bfcstr),
					0);
		}

		size_type max_size() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					max_size, (&bfcstr),
					0);
		}

		void resize(size_type n, charT c)
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
					resize, (&bfcstr, n, c),
					-ENOSYS);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		void resize(size_type n)
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
					resize, (&bfcstr, n, 0),
					-ENOSYS);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		size_type capacity() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					capacity, (&bfcstr),
					0);
		}

		void reserve(size_type res_arg = 0)
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
					reserve, (&bfcstr, res_arg),
					-ENOSYS);
			if (rc < 0) {
				throw_resize_error(-rc);
			}
		}

		void shrink_to_fit()
		{
			size_type len = length();
			VOID_METHCALL(classptrT, &bfcstr,
					reserve, (&bfcstr, len));
		}

		void clear() noexcept
		{
			VOID_METHCALL(classptrT, &bfcstr,
					resize, (&bfcstr, 0, 0));
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

			p = VMETHCALL(&bfcstr, ref, (s, pos), NULL);
			return (*p);
		}

		reference operator[](size_type pos) noexcept
		{
			charT *p;

			p = VMETHCALL(&bfcstr, ref, (&bfcstr, pos), NULL);
			return (*p);
		}

		const_reference at(size_type n) const
		{
			strptrT s = const_cast<strptrT>(&bfcstr);
			const charT *p;

			p = VMETHCALL(&bfcstr, ref, (s, n), NULL);
			return (*p);
		}

		reference at(size_type n)
		{
			charT *p;

			p = VMETHCALL(&bfcstr, ref, (&bfcstr, n), NULL);
			return (*p);
		}

		const charT& front() const noexcept;
		charT& front() noexcept;
		const charT& back() const noexcept;
		charT& back() noexcept;
		
		// 21.4.6, modifiers:
		basic_string& operator+=(const basic_string& str)
		{
			VMETHCALL(&bfcstr, append_bfstr, (&bfcstr, str),
				  &bfcstr);
		}

		basic_string& operator+=(const charT* s)
		{
			VMETHCALL(&bfcstr, append_c_str, (&bfcstr, s),
				  &bfcstr);
		}

		basic_string& operator+=(charT c)
		{
			VMETHCALL(&bfcstr, push_back, (&bfcstr, c), 0);
		}

		// basic_string& operator+=(initializer_list<charT>);

		basic_string& append(const basic_string& str)
		{
			VMETHCALL(&bfcstr, append_bfstr, (&bfcstr, str),
				  &bfcstr);
		}

		basic_string& append(const basic_string& str, size_type pos, size_type n);

		basic_string& append(const charT* s, size_type n)
		{
			VMETHCALL(&bfcstr, append_buffer, (&bfcstr, s, n),
				  &bfcstr);
		}

		basic_string& append(const charT* s)
		{
			VMETHCALL(&bfcstr, append_c_str, (&bfcstr, s),
				  &bfcstr);
		}

		basic_string& append(size_type n, charT c)
		{
			VMETHCALL(&bfcstr, append_fill, (&bfcstr, n, c),
				  &bfcstr);
		}

		template<class InputIterator> basic_string& append(InputIterator first, InputIterator last);
		// basic_string& append(initializer_list<charT>);

		void push_back(charT c)
		{
			VMETHCALL(&bfcstr, push_back, (&bfcstr, c), 0);
		}

		basic_string& assign(const basic_string& str)
		{
			VMETHCALL(&bfcstr, assign_bfstr, (&bfcstr,&str.bfcstr),
				  &bfcstr);
		}

		// basic_string& assign(basic_string&& str) noexcept;
		basic_string& assign(const basic_string& str, size_type pos, size_type n);

		basic_string& assign(const charT* s, size_type n)
		{
			VMETHCALL(&bfcstr, assign_buffer, (&bfcstr, s, n),
				  &bfcstr);
		}

		basic_string& assign(const charT* s)
		{
			VMETHCALL(&bfcstr, assign_c_str, (&bfcstr, s),
				  &bfcstr);
		}

		basic_string& assign(size_type n, charT c)
		{
			VMETHCALL(&bfcstr, assign_fill, (&bfcstr, n, c),
				  &bfcstr);
		}

		template<class InputIterator> basic_string& assign(InputIterator first, InputIterator last);
		// basic_string& assign(initializer_list<charT>);
		basic_string& insert(size_type pos1, const basic_string& str);
		basic_string& insert(size_type pos1, const basic_string& str, size_type pos2, size_type n);
		basic_string& insert(size_type pos, const charT* s, size_type n);
		basic_string& insert(size_type pos, const charT* s);
		basic_string& insert(size_type pos, size_type n, charT c);
		iterator insert(const_iterator p, charT c);
		iterator insert(const_iterator p, size_type n, charT c);
		template<class InputIterator> iterator insert(const_iterator p, InputIterator first, InputIterator last);
		// iterator insert(const_iterator p, initializer_list<charT>);

		basic_string& erase(size_type pos = 0, size_type n = npos)
		{
			VOID_METHCALL(classptrT, &bfcstr,
					erase_seq, (&bfcstr, pos, n));
			return (*this);
		}

		iterator erase(const_iterator p);
		iterator erase(const_iterator first, const_iterator last);
		void pop_back() noexcept;
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str);
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2);
		basic_string& replace(size_type pos, size_type n1, const charT* s, size_type n2);
		basic_string& replace(size_type pos, size_type n1, const charT* s);
		basic_string& replace(size_type pos, size_type n1, size_type n2, charT c);
		basic_string& replace(const_iterator i1, const_iterator i2, const basic_string& str);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s, size_type n);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s);
		basic_string& replace(const_iterator i1, const_iterator i2, size_type n, charT c);
		template<class InputIterator> basic_string& replace(const_iterator i1, const_iterator i2, InputIterator j1, InputIterator j2);
		// basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
		size_type copy(charT* s, size_type n, size_type pos = 0) const;
		void swap(basic_string& str) noexcept;
		
		// 21.4.7, string operations:
		const charT* c_str() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr, data, (&bfcstr),
					bfcstr.buf);
		}

		const charT* data() const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr, data, (&bfcstr),
					bfcstr.buf);
		}

		allocator_type get_allocator() const noexcept
		{
			return (saved_allocator);
		}

		size_type find (const basic_string& str, size_type pos = 0)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					find_bfstr, (&bfcstr, &str.bfcstr, pos),
					npos);
		}

		size_type find (const charT* s, size_type pos, size_type n)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					find_buffer, (&bfcstr, s, pos, n),
					npos);
		}

		size_type find (const charT* s,size_type pos = 0)const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					find_c_str, (&bfcstr, s, pos),
					npos);
		}

		size_type find (charT c, size_type pos = 0) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					find_char, (&bfcstr, c, pos),
					npos);
		}

		size_type rfind(const basic_string& str, size_type pos = npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					rfind_bfstr, (&bfcstr,&str.bfcstr,pos),
					npos);
		}

		size_type rfind(const charT* s, size_type pos, size_type n)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					rfind_buffer, (&bfcstr, s, pos, n),
					npos);
		}

		size_type rfind(const charT* s, size_type pos = npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					rfind_c_str, (&bfcstr, s, pos),
					npos);
		}

		size_type rfind(charT c, size_type pos = npos) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
					rfind_char, (&bfcstr, c, pos),
					npos);
		}

		size_type find_first_of(const basic_string& str,
					size_type pos = 0) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_of_bfstr, (&bfcstr,&str.bfcstr,pos),
				npos);
		}

		size_type find_first_of(const charT* s, size_type pos,
					size_type n) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_of_buffer, (&bfcstr, s, pos, n),
				npos);
		}

		size_type find_first_of(const charT* s, size_type pos = 0)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_of_c_str, (&bfcstr, s, pos),
				npos);
		}

		size_type find_first_of(charT c, size_type pos = 0)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_of_char, (&bfcstr, c, pos),
				npos);
		}

		size_type find_last_of (const basic_string& str,
					size_type pos = npos) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_of_bfstr, (&bfcstr, &str.bfcstr, pos),
				npos);
		}

		size_type find_last_of (const charT* s, size_type pos,
					size_type n) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_of_buffer, (&bfcstr, s, pos, n),
				npos);
		}

		size_type find_last_of (const charT* s, size_type pos = npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_of_c_str, (&bfcstr, s, pos),
				npos);
		}

		size_type find_last_of (charT c, size_type pos = npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_of_char, (&bfcstr, c, pos),
				npos);
		}

		size_type find_first_not_of(const basic_string& str,
					size_type pos = 0) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
			    find_first_not_of_bfstr,(&bfcstr,&str.bfcstr,pos),
				npos);
		}

		size_type find_first_not_of(const charT* s,
				size_type pos, size_type n) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_not_of_buffer, (&bfcstr, s, pos, n),
				npos);
		}

		size_type find_first_not_of(const charT* s, size_type pos = 0)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_not_of_c_str, (&bfcstr, s, pos),
				npos);
		}

		size_type find_first_not_of(charT c, size_type pos = 0)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_first_not_of_char, (&bfcstr, c, pos),
				npos);
		}

		size_type find_last_not_of (const basic_string& str,
					size_type pos = npos) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
			    find_last_not_of_bfstr,(&bfcstr,&str.bfcstr,pos),
				npos);
		}

		size_type find_last_not_of (const charT* s,
				size_type pos, size_type n) const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_not_of_buffer, (&bfcstr, s, pos, n),
				npos);
		}

		size_type find_last_not_of (const charT* s, size_type pos=npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_not_of_c_str, (&bfcstr, s, pos),
				npos);
		}

		size_type find_last_not_of (charT c, size_type pos = npos)
								const noexcept
		{
			RETURN_METHCALL(classptrT, &bfcstr,
				find_last_not_of_char, (&bfcstr, c, pos),
				npos);
		}

		basic_string substr(size_type pos = 0, size_type n = npos) const
		{
			basic_string s(saved_allocator);
			int rc;

			RETVAR_METHCALL(rc, classptrT, &bfcstr,
					substr, (&bfcstr, pos, n,
					         &s.bfcstr, sizeof(s.bfcstr)),
					-ENOSYS);
			if (rc < 0) {
				throw_substr_error(-rc);
			}
			return s;
		}

		int compare(const basic_string& str) const noexcept
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_bfstr, (&bfcstr, &str.bfcstr),
				-ENOSYS);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1,
			    const basic_string& str) const
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_substr, (&bfcstr,pos1,n1,&str.bfcstr),
				-ENOSYS);
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
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_substrs, (&bfcstr, pos1, n1,
						  &str.bfcstr, pos2, npos),
				-ENOSYS);
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
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_substrs, (&bfcstr, pos1, n1,
						  &str.bfcstr, pos2, n2),
				-ENOSYS);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(const charT* s) const noexcept
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_c_str, (&bfcstr, s),
				-ENOSYS);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1, const charT* s) const
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_substr_c_str, (&bfcstr, pos1, n1, s),
				-ENOSYS);
			if (rc < -1) {
				throw_substr_error(-rc);
			}
			return (rc);
		}

		int compare(size_type pos1, size_type n1,
			    const charT* s, size_type n2) const
		{
			int rc;
			RETVAR_METHCALL(rc, classptrT, &bfcstr,
				compare_buffer, (&bfcstr, pos1, n1, s, n2),
				-ENOSYS);
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
