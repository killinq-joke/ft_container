#pragma once

# include <memory>
# include <algorithm>
# include <cstddef>
# include <tgmath.h>
# include "./utils/utils.hpp"
# include "./utils/random_access_iterator.hpp"
# include <iterator>

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
		
		explicit vector (const allocator_type& alloc = allocator_type()) : _alloc(alloc), _start(nullptr), _size(0), _capacity(0)
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
			difference_type n = ft::distance(first, last);

			_size = n;
			_capacity = n;
			_start = _alloc.allocate(n);
			for (difference_type i = 0; i < n; i++)
			{
				_alloc.construct(&_start[i], *(first++));
			}
		}
		
		vector (const vector& x) : _alloc(x.get_allocator()), _size(x.size()), _capacity(0)
		{
			*this = x;
		}

		vector &operator=(const vector& x)
		{
			this->clear();
			_alloc = x.get_allocator();
			this->insert(this->end(), x.begin(), x.end());
			return *this;
		}

		~vector()
		{
			this->clear();
			_alloc.deallocate(_start, _capacity);
		}
					
		void show_vector()
		{
			std::cout << "capacity == " << this->capacity() << std::endl;
			std::cout << "size == " << this->size() << std::endl;
			// std::cout << "begin == " << x.begin() << std::endl;
			// std::cout << "end == " << x.end() << std::endl;
			std::cout << "[";
			for (size_t i = 0; i < this->size(); i++)
			{
				// std::cout << "it == " << it << std::endl;
				std::cout <<  _start[i] << ", ";
			}
			std::cout << "]" << std::endl;
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
			if (n > _size)
			{
				while (_size < n)
				{
					this->push_back(val);
				}
			}
			else
			{
				while (n < _size)
				{
					_alloc.destroy(&_start[n]);
					_size--;
				}
			}
		}

		size_type   capacity(void) const { return _capacity; }

		bool        empty(void) const { return !_size; }

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
			this->clear();
			while (first != last)
			{
				this->push_back(*first);
				first++;
			}
			// std::cerr << "capacity == " << capacity() << std::endl;
		}
		
		void assign(size_type n, const value_type& val)
		{
			this->clear();
			if (!n)
				return ;
			// _start = _alloc.allocate(n);
			for (size_type i = 0; i < n; i++)
			{
				// _alloc.construct(&_start[i], val);
				this->push_back(val);
			}
			// std::cerr << "capacity == " << capacity() << std::endl;
		}

		void push_back(const value_type& val)
		{
			this->insert(this->end(), val);
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
			size_type	diff;
			iterator	it;
			size_type	i;

			diff = this->end() - position;
			if (_size >= _capacity)
				this->_increase_capacity();
			it = this->end(); 
			i = 0;
			while (i < diff)
			{
				*it = *(it - 1);
				i++;
				it--;
			}
			*it = val;
			_size++;
			return it;
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			size_type	diff;
			iterator	it;

			diff = abs(this->end() - position);
			while (_size + n > _capacity)
				this->_increase_capacity();
			it = (this->end() + n - 1);
			while (diff--)
			{
				*it = *(it - n);
				it--;
			}
			_size += n;
			while (n--)
			{
				*it = val;
				it--;
			}
		}

		template <class InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
		{
			size_type	diff;
			size_type	n;
			iterator	it;

			diff = abs(this->end() - position);
			n = 0;
			while (first++ != last)
			{
				n++;
			}
			while (_size + n > _capacity)
				this->_increase_capacity();
			it = (this->end() + n - 1);
			while (diff--)
			{
				*it = *(it - n);
				it--;
			}
			_size += n;
			while (n--)
			{
				*it = *(--last);
				it--;
			}
		}

		iterator erase(iterator position)
		{
			if (_size)
			{
				_alloc.destroy(&(*position));
				for (iterator it = position; it != this->end(); it++)
				{
					*it = *(it + 1);
				}
				_size--;
			}
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			difference_type diff = last - first;

			if (_size)
			{
				// std::cout << "diff == " << diff << std::endl;
				for (iterator it = first; it != last; it++)
				{
					_alloc.destroy(&(*it));
				}
				for (iterator it = first; it != this->end(); it++)
				{
					*it = *(it + diff);
				}
				_size -= diff;
			}
			return first;
		}

		void swap(vector& x)
		{
			if (x == *this)
				return ;

			pointer tmpstart = x._start;
			size_type tmpsize = x.size();
			size_type tmpcapacity = x.capacity();
			allocator_type tmpalloc = x.get_allocator();

			x._start = this->_start;
			x._size = this->_size;
			x._capacity = this->capacity();
			x._alloc = this->_alloc;

			this->_start = tmpstart;
			this->_size = tmpsize;
			this->_capacity = tmpcapacity;
			this->_alloc = tmpalloc;
		}

		void clear()
		{
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
