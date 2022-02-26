#pragma once

# include <memory>
# include <algorithm>
# include <cstddef>
# include <tgmath.h>
# include "./utils/utils.hpp"
# include "./utils/random_access_iterator.hpp"

namespace ft
{
	template < class T, class Alloc = std::allocator<T> >
	class vector
	{   
	public:

		typedef T                                           				value_type;
		typedef Alloc                                       				allocator_type;
		typedef typename allocator_type::reference          				reference;
		typedef typename allocator_type::const_reference    				const_reference;
		typedef typename allocator_type::pointer            				pointer;
		typedef typename allocator_type::const_pointer      				const_pointer;
		typedef ft::random_access_iterator<value_type>               		iterator;
		typedef ft::random_access_iterator<const value_type>            	const_iterator;
		typedef ft::reverse_iterator<iterator>             					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>       					const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type		difference_type; 
		typedef typename allocator_type::size_type          				size_type;
		
		explicit vector (const allocator_type& alloc = allocator_type()) : _alloc(Alloc), _start(nullptr), _capacity(0)
		{}

		explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type())
		{
		}

		template <class InputIterator>
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type(),
						typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr) : _alloc(alloc)
		{
		}
		
		vector (const vector& x)
		{
		}
		
		~vector()
		{
		}
					
		vector &operator=(const vector& x)
		{
		}

		iterator begin() {};
		const_iterator begin() const {}

		iterator end()
		{
		}

		const_iterator end() const
		{
		}

		reverse_iterator rbegin(){}

		const_reverse_iterator rbegin() const {}

		reverse_iterator rend()
		{}

		const_reverse_iterator rend() const {}

		size_type   size(void) const {}

		size_type   max_size(void) const {}

		void        resize(size_type n, value_type val = value_type())
		{
		}

		size_type   capacity(void) const {}

		bool        empty(void) const {}

		void        reserve(size_type n)
		{
		}

		reference operator[](size_type n) {}

		const_reference operator[](size_type n) const {}
		
		reference at(size_type n)
		{
		}

		const_reference at(size_type n) const
		{
		}

		reference front() {}

		const_reference front() const {}
		
		reference back() { return (*(_end - 1)); }

		const_reference back() const { return (*(_end - 1)); }

		template <class InputIterator>
			void assign(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
			{
				bool is_valid;
				if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value))
					throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
			}
		
		void assign(size_type n, const value_type& val)
		{
		}

		void push_back(const value_type& val)
		{

		}

		void pop_back()
		{
		}

		iterator insert(iterator position, const value_type& val)
		{
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
		}

		template <class InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
		{
			bool is_valid;
			if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::value))
				throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
		}

		iterator erase(iterator position)
		{
		}

		iterator erase(iterator first, iterator last)
		{
		}

		void swap(vector& x)
		{
		}

		void clear()
		{
			for (size_t i = 0; i < _size; i++)
			{
				_alloc.destroy(&_start[i]);
			}
		}

		allocator_type get_allocator() const
		{
			return allocator_type(_alloc);
		}

	private:
		allocator_type  _alloc;
		pointer         _start;
		size_type		_size;
		size_type		_capacity;

		void	increase_capacity()
		{
			if (!_capacity)
			{
				_start = _alloc.allocate(1);
				_capacity = 1;
			}
			else if (_capacity < this->max_size() / 2)
			{
				pointer tmp = _alloc.allocate(_capacity * 2);

				for (iterator it = this->begin(), size_type i = 0; it != this->end(); it++, i++)
				{
					_alloc.construct(tmp + i, it.base());
				}
				this->clear();
				_alloc.deallocate(_start, _capacity);
				_capacity *= 2;
			}
			else
			{
				pointer tmp = _alloc.allocate(_capacity * 2);

				_alloc.allocate(this->max_size());
				for (iterator it = this->begin(), size_type i = 0; it != this->end(); it++, i++)
				{
					_alloc.construct(tmp + i, it.base());
				}
				this->clear();
				_alloc.deallocate(_start, _capacity);
				_capacity = this->max_size();
			}
		}
	};

	template <class T, class Alloc>
	bool operator== (const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::vector<T>::const_iterator first1 = lhs.begin();
		typename ft::vector<T>::const_iterator first2 = rhs.begin();
		while (first1 != lhs.end())
		{
			if (first2 == rhs.end() || *first1 != *first2)
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs == rhs));
	}
	
	template <class T, class Alloc>
	bool operator<  (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Alloc>
	bool operator<= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(rhs < lhs));
	}

	template <class T, class Alloc>
	bool operator>  (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class Alloc>
	bool operator>= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs < rhs));
	}
	
	template <class T, class Alloc>
	void swap (vector<T,Alloc>& x, vector<T,Alloc>&y)
	{
		x.swap(y);
	}
}
