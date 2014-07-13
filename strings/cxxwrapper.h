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

		basic_string(const basic_string& str);
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

		basic_string(size_type n, charT c, const Allocator& a = Allocator());
		template<class InputIterator>
		basic_string(InputIterator begin, InputIterator end, const Allocator& a = Allocator());
		// basic_string(initializer_list<charT>, const Allocator& = Allocator());
		basic_string(const basic_string&, const Allocator&);
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
			return (VMETHCALL(&bfcstr, size, (&bfcstr), 0));
		}

		size_type length() const noexcept
		{
			return (VMETHCALL(&bfcstr, size, (&bfcstr), 0));
		}

		size_type max_size() const noexcept
		{
			return (VMETHCALL(&bfcstr, max_size, (&bfcstr), 0));
		}

		void resize(size_type n, charT c);
		void resize(size_type n);
		size_type capacity() const noexcept
		{
			return (VMETHCALL(&bfcstr, capacity, (&bfcstr), 0));
		}

		void reserve(size_type res_arg = 0);
		void shrink_to_fit();

		void clear() noexcept
		{
			VMETHCALL(&bfcstr, resize, (&bfcstr, 0, 0), 0);
		}

		bool empty() const noexcept;
		
		// 21.4.5, element access:
		const_reference operator[](size_type pos) const noexcept;
		reference operator[](size_type pos) noexcept;
		const_reference at(size_type n) const;
		reference at(size_type n);
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
			VMETHCALL(&bfcstr, assign_char, (&bfcstr, c),
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
		basic_string& erase(size_type pos = 0, size_type n = npos);
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
		const charT* c_str() const noexcept;
		const charT* data() const noexcept;
		allocator_type get_allocator() const noexcept
		{
			return (saved_allocator);
		}

		size_type find (const basic_string& str, size_type pos = 0) const noexcept;
		size_type find (const charT* s, size_type pos, size_type n) const noexcept;
		size_type find (const charT* s, size_type pos = 0) const noexcept;
		size_type find (charT c, size_type pos = 0) const noexcept;
		size_type rfind(const basic_string& str, size_type pos = npos) const noexcept;
		size_type rfind(const charT* s, size_type pos, size_type n) const noexcept;
		size_type rfind(const charT* s, size_type pos = npos) const noexcept;
		size_type rfind(charT c, size_type pos = npos) const noexcept;
		size_type find_first_of(const basic_string& str, size_type pos = 0) const noexcept;
		size_type find_first_of(const charT* s, size_type pos, size_type n) const noexcept;
		size_type find_first_of(const charT* s, size_type pos = 0) const noexcept;
		size_type find_first_of(charT c, size_type pos = 0) const noexcept;
		size_type find_last_of (const basic_string& str, size_type pos = npos) const noexcept;
		size_type find_last_of (const charT* s, size_type pos, size_type n) const noexcept;
		size_type find_last_of (const charT* s, size_type pos = npos) const noexcept;
		size_type find_last_of (charT c, size_type pos = npos) const noexcept;
		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const noexcept;
		size_type find_first_not_of(const charT* s, size_type pos, size_type n) const noexcept;
		size_type find_first_not_of(const charT* s, size_type pos = 0) const noexcept;
		size_type find_first_not_of(charT c, size_type pos = 0) const noexcept;
		size_type find_last_not_of (const basic_string& str, size_type pos = npos) const noexcept;
		size_type find_last_not_of (const charT* s, size_type pos, size_type n) const noexcept;
		size_type find_last_not_of (const charT* s, size_type pos = npos) const noexcept;
		size_type find_last_not_of (charT c, size_type pos = npos) const noexcept;
		basic_string substr(size_type pos = 0, size_type n = npos) const;
		int compare(const basic_string& str) const noexcept
		{
			return (VMETHCALL(&bfcstr,
				compare_bfstr, (&bfcstr, &str.bfcstr),
				-1));
		}


		int compare(size_type pos1, size_type n1, const basic_string& str) const;
		int compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2) const;
		int compare(const charT* s) const noexcept
		{
			return (VMETHCALL(&bfcstr,
				compare_c_str, (&bfcstr, s),
				-1));
		}

		int compare(size_type pos1, size_type n1, const charT* s) const;
		int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;
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
