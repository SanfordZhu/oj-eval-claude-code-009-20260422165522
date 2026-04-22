#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>
#include <new>

namespace sjtu
{

template<typename T>
class vector
{
public:
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		T *ptr;
	public:
		iterator(T *p = nullptr) : ptr(p) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr + n);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n);
		}
		int operator-(const iterator &rhs) const
		{
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		iterator& operator++()
		{
			++ptr;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		iterator& operator--()
		{
			--ptr;
			return *this;
		}
		T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		friend class vector;
		friend class const_iterator;
	};

	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T *ptr;
	public:
		const_iterator(const T *p = nullptr) : ptr(p) {}
		const_iterator(const iterator &it) : ptr(it.ptr) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n);
		}
		int operator-(const const_iterator &rhs) const
		{
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++()
		{
			++ptr;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--()
		{
			--ptr;
			return *this;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
	};

private:
	T *begin_ptr;
	T *end_ptr;
	T *cap_ptr;
	size_t sz;
	size_t cap;

	void reserve(size_t new_cap)
	{
		if (new_cap <= cap)
			return;
		T *new_begin = static_cast<T*>(::operator new(new_cap * sizeof(T)));
		T *new_end = new_begin;
		for (size_t i = 0; i < sz; ++i)
		{
			::new (new_end) T(begin_ptr[i]);
			++new_end;
		}
		for (size_t i = 0; i < sz; ++i)
			begin_ptr[i].~T();
		::operator delete(begin_ptr);
		begin_ptr = new_begin;
		end_ptr = new_end;
		cap_ptr = begin_ptr + new_cap;
		cap = new_cap;
	}

public:
	vector() : begin_ptr(nullptr), end_ptr(nullptr), cap_ptr(nullptr), sz(0), cap(0) {}

	vector(const vector &other) : begin_ptr(nullptr), end_ptr(nullptr), cap_ptr(nullptr), sz(0), cap(0)
	{
		if (other.sz > 0)
		{
			begin_ptr = static_cast<T*>(::operator new(other.sz * sizeof(T)));
			end_ptr = begin_ptr;
			cap_ptr = begin_ptr + other.sz;
			cap = other.sz;
			for (size_t i = 0; i < other.sz; ++i)
			{
				::new (end_ptr) T(other.begin_ptr[i]);
				++end_ptr;
			}
			sz = other.sz;
		}
	}

	~vector()
	{
		for (size_t i = 0; i < sz; ++i)
			begin_ptr[i].~T();
		::operator delete(begin_ptr);
	}

	vector &operator=(const vector &other)
	{
		if (this == &other)
			return *this;
		for (size_t i = 0; i < sz; ++i)
			begin_ptr[i].~T();
		::operator delete(begin_ptr);
		begin_ptr = nullptr;
		end_ptr = nullptr;
		cap_ptr = nullptr;
		sz = 0;
		cap = 0;
		if (other.sz > 0)
		{
			begin_ptr = static_cast<T*>(::operator new(other.sz * sizeof(T)));
			end_ptr = begin_ptr;
			cap_ptr = begin_ptr + other.sz;
			cap = other.sz;
			for (size_t i = 0; i < other.sz; ++i)
			{
				::new (end_ptr) T(other.begin_ptr[i]);
				++end_ptr;
			}
			sz = other.sz;
		}
		return *this;
	}

	T & at(const size_t &pos)
	{
		if (pos >= sz)
			throw index_out_of_bound();
		return begin_ptr[pos];
	}

	const T & at(const size_t &pos) const
	{
		if (pos >= sz)
			throw index_out_of_bound();
		return begin_ptr[pos];
	}

	T & operator[](const size_t &pos)
	{
		if (pos >= sz)
			throw index_out_of_bound();
		return begin_ptr[pos];
	}

	const T & operator[](const size_t &pos) const
	{
		if (pos >= sz)
			throw index_out_of_bound();
		return begin_ptr[pos];
	}

	const T & front() const
	{
		if (sz == 0)
			throw container_is_empty();
		return begin_ptr[0];
	}

	const T & back() const
	{
		if (sz == 0)
			throw container_is_empty();
		return begin_ptr[sz - 1];
	}

	iterator begin()
	{
		return iterator(begin_ptr);
	}

	const_iterator begin() const
	{
		return const_iterator(begin_ptr);
	}

	const_iterator cbegin() const
	{
		return const_iterator(begin_ptr);
	}

	iterator end()
	{
		return iterator(end_ptr);
	}

	const_iterator end() const
	{
		return const_iterator(end_ptr);
	}

	const_iterator cend() const
	{
		return const_iterator(end_ptr);
	}

	bool empty() const
	{
		return sz == 0;
	}

	size_t size() const
	{
		return sz;
	}

	void clear()
	{
		for (size_t i = 0; i < sz; ++i)
			begin_ptr[i].~T();
		sz = 0;
		end_ptr = begin_ptr;
	}

	iterator insert(iterator pos, const T &value)
	{
		size_t idx = pos - begin();
		if (sz == cap)
		{
			size_t new_cap = cap == 0 ? 1 : cap * 2;
			T *new_begin = static_cast<T*>(::operator new(new_cap * sizeof(T)));
			T *new_end = new_begin;
			for (size_t i = 0; i < idx; ++i)
			{
				::new (new_end) T(begin_ptr[i]);
				++new_end;
			}
			::new (new_end) T(value);
			++new_end;
			for (size_t i = idx; i < sz; ++i)
			{
				::new (new_end) T(begin_ptr[i]);
				++new_end;
			}
			for (size_t i = 0; i < sz; ++i)
				begin_ptr[i].~T();
			::operator delete(begin_ptr);
			begin_ptr = new_begin;
			end_ptr = new_end;
			cap_ptr = begin_ptr + new_cap;
			cap = new_cap;
			++sz;
			return iterator(begin_ptr + idx);
		}
		else
		{
			::new (end_ptr) T(begin_ptr[sz - 1]);
			++end_ptr;
			for (size_t i = sz; i > idx; --i)
				begin_ptr[i] = begin_ptr[i - 1];
			begin_ptr[idx] = value;
			++sz;
			return iterator(begin_ptr + idx);
		}
	}

	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > sz)
			throw index_out_of_bound();
		return insert(begin() + ind, value);
	}

	iterator erase(iterator pos)
	{
		size_t idx = pos - begin();
		if (idx >= sz)
			throw index_out_of_bound();
		begin_ptr[idx].~T();
		for (size_t i = idx; i < sz - 1; ++i)
		{
			::new (begin_ptr + i) T(begin_ptr[i + 1]);
			begin_ptr[i + 1].~T();
		}
		--sz;
		--end_ptr;
		return iterator(begin_ptr + idx);
	}

	iterator erase(const size_t &ind)
	{
		if (ind >= sz)
			throw index_out_of_bound();
		return erase(begin() + ind);
	}

	void push_back(const T &value)
	{
		if (sz == cap)
		{
			size_t new_cap = cap == 0 ? 1 : cap * 2;
			reserve(new_cap);
		}
		::new (end_ptr) T(value);
		++end_ptr;
		++sz;
	}

	void pop_back()
	{
		if (sz == 0)
			throw container_is_empty();
		--end_ptr;
		--sz;
		end_ptr->~T();
	}
};

}

#endif
