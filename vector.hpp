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
		
		explicit vector (const allocator_type& alloc = allocator_type()) : _alloc(alloc), _start(nullptr), _capacity(0)
		{}

		explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(n), _capacity(n)
		{
			_start = _alloc.allocate(n);
			for (size_type i = 0; i < n; i++)
			{
				_alloc.construct(&_start[i], val);
			}
		}

		template <class InputIterator>
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type(),
						typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr) : _alloc(alloc)
		{
			if (!ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value)
				throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
			for (size_type i = 0; first != last; first++, i++)
			{
				if (_capacity == _size)
					this->_increase_capacity();
				_alloc.construct(&_start[i], *first);
				_size++;
			}
		}
		
		vector (const vector& x) : _alloc(x.get_allocator()), _size(x.size())
		{
			*this = x;
		}
		
		~vector()
		{
			this->clear();
			_alloc.deallocate(_start, _capacity);
		}
					
		vector &operator=(const vector& x)
		{
			this->clear();
			this->insert(this->end(), x.begin(), x.end());
			return *this;
		}

		iterator begin() { return _start; };
		const_iterator begin() const { return _start; }

		iterator end()
		{
			return _start + _size;
		}

		const_iterator end() const
		{
			return _start + _size;
		}

		reverse_iterator rbegin(){ return reverse_iterator(this->end()); }

		const_reverse_iterator rbegin() const { return reverse_iterator(this->end()); }

		reverse_iterator rend()
		{ return reverse_iterator(this->begin()); }

		const_reverse_iterator rend() const { return reverse_iterator(this->begin()); }

		size_type   size(void) const { return _size; }

		size_type   max_size(void) const { return _alloc.max_size(); }

		void        resize(size_type n, value_type val = value_type())
		{
			/*Resizes the container so that it contains n elements.
			If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
			If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n. If val is specified, the new elements are initialized as copies of val, otherwise, they are value-initialized.
			If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.*/
			if (n > _size)
			{
				while (_size < n)
				{
					this->push_back(val);
				}
			}
			else if (n < _size)
			{
				for (; n < _size; n++)
				{
					_alloc.destroy(&_start[n]);
				}
			}
		}

		size_type   capacity(void) const { return _capacity; }

		bool        empty(void) const { return !_size; }

  
		/*Requests that the vector capacity be at least enough to contain n elements.
		If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
		In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
		This function has no effect on the vector size and cannot alter its elements.*/ 
		void        reserve(size_type n)
		{
			if (n > _capacity)
			{
				this->_increase_capacity();
			}
		}

		reference operator[](size_type n) { return _start[n]; }

		const_reference operator[](size_type n) const { return _start[n]; }
		
		reference at(size_type n)
		{
			if (n >= _size)
				throw std::out_of_range("index out of range");
			return _start[n];
		}

		const_reference at(size_type n) const
		{
			if (n < 0 || n >= _size)
				throw std::out_of_range("index out of range");
			return _start[n];
		}

		reference front() { return *_start; }

		const_reference front() const { return *_start; }
		
		reference back() { return (*(this->end() - 1)); }

		const_reference back() const { return (*(this->end() - 1)); }

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
		{
			bool is_valid;

			if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category>::value))
				throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
			this->clear();
			for (iterator it = first; it != last; it++)
			{
				if (_size == _capacity)
					this->_increase_capacity();
				this->push_back(*it);
			}
		}
		
		void assign(size_type n, const value_type& val)
		{
			this->clear();
			_alloc.deallocate(_start, _capacity);
			for (size_type i = 0; i < n; i++)
			{
				if (_capacity == _size)
					this->_increase_capacity();
				_alloc.construct(&_start[i], val);
				_size++;
			}
		}

		void push_back(const value_type& val)
		{
			if (_size == _capacity)
				this->_increase_capacity();
			_alloc.construct(this->end().base(), val);
			_size++;
		}

		void pop_back()
		{
			if (_size)
			{
				_alloc.destroy((this->end() - 1).base());
				_size--;
			}
		}

		iterator insert(iterator position, const value_type& val)
		{
			if (_size == _capacity)
				this->_increase_capacity();
			*(this->end()) = 42;
			*(this->end()-1) = 43;
			std::cout << position.base() << std::endl;
			std::cout << this->begin() << std::endl;
			std::cout << this->end() << std::endl;
			// for (iterator it = this->end(); it != position; it--)
			// {
			// 	*it = *(it - 1);
			// }
			*position = val;
			_size++;
			return position;
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			iterator it = this->end() + n - 1;

			while (_size + n > _capacity)
				this->_increase_capacity();
			while (it != (position + n))
			{
				*it = *(it - n);
				it--;
			}
			while (it != position)
			{
				*it = val;
				it--;
			}
			*position = val;
			_size += n;
		}

		template <class InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
		{
			bool is_valid;

			if (!(is_valid = ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::value))
				throw (ft::InvalidIteratorException<typename ft::is_ft_iterator_tagged<typename ft::iterator_traits<InputIterator>::iterator_category >::type>());
			
			difference_type diff = last - first;

			while (_size + diff > _capacity)
				this->_increase_capacity();
			iterator it = this->end() + diff - 1;
			while (it != (position + diff))
			{
				*it = *(it - diff);
				it--;
			}
			while (it != position && last != first)
			{
				*it = *last;
				last--;
				it--;
			}
			*position = *first;
			_size += diff;
		}

		iterator erase(iterator position)
		{
			iterator it;

			if (_size)
			{
				_alloc.destroy(*position);
				while (position != this->end())
				{
					*it = *(it + 1);
					it++;
				}
				_size--;
			}
		}

		iterator erase(iterator first, iterator last)
		{
			difference_type diff = last - first;

			if (_size)
			{
				while (first != last)
				{
					_alloc.destroy(*first);
					first++;
				}
				while (first != this->end())
				{
					*first = *(first + diff);
					first++;
				}
				_size -= diff;
			}
		}

		void swap(vector& x)
		{
			vector tmp = x;

			x = *this;
			*this = tmp;
		}

		void clear()
		{
			std::cout << "size == " << _size << std::endl;
			while (_size)
			{
				_alloc.destroy(&_start[_size - 1]);
				_size--;
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

		void	_increase_capacity()
		{
			if (!_capacity)
			{
				_start = _alloc.allocate(1);
				_capacity = 1;
			}
			else if (_capacity < this->max_size() / 2)
			{
				pointer tmp = _alloc.allocate(_capacity * 2);

				size_type i = 0;
				for (iterator it = this->begin(); it != this->end(); it++, i++)
				{
					_alloc.construct((tmp + i), *it);
				}
				// this->clear();
				_alloc.deallocate(_start, _capacity);
				_start = tmp;
				_capacity *= 2;
			}
			else
			{
				throw std::runtime_error("capacity error");
			}
		}
	};

	template <class T, class Alloc>
	bool operator==(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
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
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs == rhs));
	}
	
	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(rhs < lhs));
	}

	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs < rhs));
	}
	
	template <class T, class Alloc>
	void swap(vector<T,Alloc>& x, vector<T,Alloc>&y)
	{
		x.swap(y);
	}
}
