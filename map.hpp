#pragma once
#include "vector.hpp"
#include "rbtree.hpp"

namespace ft
{

template < class Key,                                     // map::key_type
           class T,                                       // map::mapped_type
           class Compare = less<Key>,                     // map::key_compare
           class Alloc = std::allocator<pair<const Key,T> >    // map::allocator_type
            >
class map
{

public:
    
    typedef Key                                                         key_type;
    typedef T                                                           mapped_type;
    typedef ft::pair<const key_type, mapped_type>                       value_type;
    typedef Compare                                                     key_compare;

    class value_compare : ft::binary_function<value_type, value_type, bool>
    {
        friend class map<key_type, mapped_type, key_compare, Alloc>;
        
        protected:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        
        public:

            bool operator() (const value_type& x, const value_type& y) const
            { return (comp(x.first, y.first)); }
    };

    typedef Alloc                                       				allocator_type;
    typedef typename allocator_type::reference          				reference;
    typedef typename allocator_type::const_reference    				const_reference;
    typedef typename allocator_type::pointer            				pointer;
    typedef typename allocator_type::const_pointer      				const_pointer;
    typedef ft::bidirectional_iterator<value_type>               		iterator;
    typedef ft::bidirectional_iterator<const value_type>            	const_iterator;
    typedef ft::reverse_iterator<iterator>             					reverse_iterator;
    typedef ft::reverse_iterator<const_iterator>       					const_reverse_iterator;
    typedef typename ft::iterator_traits<iterator>::difference_type		difference_type; 
    typedef typename allocator_type::size_type          				size_type;

    explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _comp(comp), _alloc(alloc)
    {
    }

    template <class InputIterator>
    map(InputIterator first, InputIterator last,
        const key_compare& comp = key_compare(),
        const allocator_type& alloc = allocator_type()
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr)) : _comp(comp), _alloc(alloc)
    {
    }

    // map(const map& x)
    // {
    // }

    ~map()
    {
		this->clear();
    }

	iterator begin()
	{
		
	}

	const_iterator begin() const
	{

	}

    allocator_type get_allocator() const
    {
        return _alloc;
    }

private:
    rbtree<value_type, key_compare> 			_tree;
    key_compare     							_comp;
    allocator_type  							_alloc;

}

};