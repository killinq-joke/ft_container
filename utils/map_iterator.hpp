#pragma once
#include <memory>
#include "utils.hpp"

namespace ft
{

template <typename T, bool b>
class binaryiterator : public ft::bidirectional_iterator<T, b>
{

public:
	typedef T  value_type;
	typedef typename ft::isConst<b, value_type&, const value_type&>::type reference;
	typedef typename ft::isConst<b, value_type*, const value_type*>::type pointer;
	typedef ptrdiff_t						difference_type;

	typedef bidirectional_iterator_tag		iterator_category;

	typedef binaryiterator<T, b>				self;
	typedef ft::node<T>							Node;
private:
	typedef typename ft::isConst<b, Node*, const Node*>::type linkPtr;
public:

	binaryiterator(void) : _ptr(nullptr)
	{
	}

	binaryiterator(linkPtr ptr) : _ptr(ptr)
	{
	}

	binaryiterator(const binaryiterator& x) : _ptr(x._ptr)
	{
	}

	binaryiterator& operator=(const binaryiterator& x)
	{
		if (this == &x)
			return (*this);
		this->_ptr = x._ptr;
		return (*this);
	}

	operator bidirectional_iterator<T, 1> () {
		bidirectional_iterator<T, 1> ret(_ptr);
		return ret;
	}

    // operator binaryiterator<const T, 1> () const
	// { return (binaryiterator<const T, 1>(this->_ptr)); }

	virtual ~binaryiterator() {}

	reference operator*() const
	{ return static_cast<linkPtr>(_ptr)->data; }

	pointer operator->() const
	{ return &static_cast<linkPtr>(_ptr)->data; }
 

	linkPtr base(void) const
	{ return (this->_ptr); }

	self& operator++(void)
	{
		if ((_ptr == _ptr->parent->right || _ptr->parent->isnull) && _ptr->right->isnull)
		{
			_ptr = _ptr->right;
			return *this;
		}
		if (_ptr->isnull && !_ptr->isrend)
		{
			_ptr = _ptr->parent;
			return *this;
		}
		if (!_ptr->right->isnull)
		{
			_ptr = _ptr->right;
			while (!_ptr->left->isnull)
				_ptr = _ptr->left;
		}
		else
		{
			linkPtr p = _ptr->parent;
			while (p && _ptr == p->right)
			{
				_ptr = p;
				p = p->parent;
			}
			if (_ptr->right != p)
				_ptr = p;
		}
		return *this;
	}

	self operator++(int)
	{
		binaryiterator ret(*this);
		operator++();
		return (ret);
	}

	self& operator--(void)
	{
		if (_ptr->isnull && !_ptr->isrend)
		{
			_ptr = _ptr->parent;
			return *this;
		}
		if (_ptr->left->isrend && !_ptr->parent->isnull)
		{
			_ptr = _ptr->left;
			return *this;
		}
		else if (!_ptr->left->isnull)
		{
			_ptr = _ptr->left;
			while (!_ptr->right->isnull)
				_ptr = _ptr->right;
		}
		else
		{
			linkPtr p = _ptr->parent;
			while (p && _ptr == p->left)
			{
				_ptr = p;
				p = p->parent;
			}
			if (_ptr->left != p)
				_ptr = p;
		}
		return *this;
	}

	self operator--(int)
	{
		binaryiterator ret(*this);
		operator--();
		return (ret);
	}

	friend bool operator==(const self &x, const self &y) {return (x._ptr == y._ptr);}
	friend bool operator!=(const self &x, const self &y) {return (x._ptr != y._ptr);}

	linkPtr _ptr;
};

template <typename T, bool b>
bool operator==(const ft::binaryiterator<T, b> lhs, const ft::binaryiterator<T, b> rhs)
{
	return (lhs.base() == rhs.base());
}

template <typename T, bool b>
bool operator!=(const ft::binaryiterator<T, b> lhs, const ft::binaryiterator<T, b> rhs)
{
	return (lhs.base() != rhs.base());
}

}