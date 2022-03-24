#pragma once
#include "vector.hpp"
#include "./utils/rbtree.hpp"

namespace ft
{

template <class Key, class T, class Compare = less<Key>, class Alloc = std::allocator<pair<const Key,T> > >
class map
{
public:
	typedef Compare											key_compare;
	typedef Key												key_type;
	typedef T												mapped_type;
	typedef pair<const key_type, mapped_type>				value_type;
	typedef	Alloc											allocator_type;
	typedef	typename allocator_type::reference				reference;
	typedef typename allocator_type::const_reference		const_reference;
	typedef typename allocator_type::pointer 				pointer;
	typedef typename allocator_type::const_pointer			const_pointer;
	typedef ft::bidirectional_iterator<value_type> 			iterator;
	typedef ft::bidirectional_iterator<const value_type>	const_iterator;
	typedef reverse_iterator<iterator>						reverse_iterator;
	// typedef reverse_iterator<const_iterator>				const_reverse_iterator;
	typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
	typedef size_t											size_type;
	typedef rbtree<value_type, key_compare>					tree_type;

	class value_compare : std::binary_function<value_type, value_type, bool>
	{
		friend class map<key_type, mapped_type, key_compare, Alloc>;
		
		protected:
			Compare comp;
			value_compare (Compare c) : comp(c) {}
		
		public:
			bool operator() (const value_type& x, const value_type& y) const
			{ return (comp(x.first, y.first)); }
	};

	explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _tree(rbtree<value_type, key_compare>()), _comparator(comp), _alloc(alloc)
	{
		_tree();
	}

	template <class InputIterator>
	map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)
	: _tree(), _comparator(comp), _alloc(alloc)
	{
		this->insert(first, last);
	}

	map(const map& x)
	: _tree(), _comparator(x._comparator), _alloc(x._alloc)
	{
		*this = x;
	}

	map& operator=(const map& x)
	{
		if (this == &x)
			return (*this);
		this->clear();
		this->insert(x.begin(), x.end());
		return (*this);
	}

	~map()
	{
		this->clear();
	}

	iterator begin()
	{
		return iterator(_tree.min(_tree.getRoot()));
	}

	const_iterator begin() const
	{
		return const_iterator(_tree.min(_tree.getRoot()));
	}

	iterator end()
	{
		return iterator(_tree.max(_tree.getRoot())->right);
	}

	const_iterator end() const
	{
		return const_iterator(_tree.max(_tree.getRoot())->right);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(_tree.max(_tree.getRoot()));
	}

	//end null node isn't implemented on the left
	// const_reverse_iterator rbegin() const
	// {
	// 	return const_reverse_iterator(_tree.min(_tree.getRoot())->left);
	// }

	bool empty() const
	{
		return !_tree.size();
	}

	size_type size() const
	{
		return _tree.size();
	}

	size_type max_size() const
	{
		return _alloc.max_size();
	}

	mapped_type& operator[](const key_type& k)
	{
		return (*((this->insert(make_pair(k,mapped_type()))).first)).second;
	}

	// pair<iterator,bool> insert(const value_type& val)
	// {
	// }

	// iterator insert(iterator position, const value_type& val)
	// {

	// }

	// template <class InputIterator>
	// void insert(InputIterator first, InputIterator last)
	// {

	// }

	tree_type		_tree;
	allocator_type	_alloc;
	key_compare		_comparator;
};


}