#pragma once
#include "vector.hpp"

namespace ft
{

template <class T, class Container = ft::vector<T> >
class stack
{

public:
    typedef Container container_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    
    explicit stack(const container_type& ctnr = container_type()) : _container(ctnr)
    {
    }

	stack(const stack& other)
	{
		*this = other;
	}

    stack& operator=(const stack& other)
    {
		this->_container = other.c();
        // *this = other;
        return *this;
    }

    ~stack()
    {
    }

    bool	empty() const
    {
        return _container.empty();
    }

    size_type	size() const
    {
        return _container.size();
    }

    value_type&	top()
    {
        return _container.back();
    }

    const	value_type& top() const
    {
        return _container.back();
    }

    void	push(const value_type& val)
    {
        _container.push_back(val);
    }

    void pop()
    {
        _container.pop_back();
    }

    container_type  c() const
    {
        return _container;
    }

private:
    container_type  _container;

};

template <class T, class Container>
bool operator==(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() == rhs.c());
}

template <class T, class Container>
bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() != rhs.c());
}

template <class T, class Container>
bool operator<(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() < rhs.c());
}

template <class T, class Container>
bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() <= rhs.c());
}

template <class T, class Container>
bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() > rhs.c());
}

template <class T, class Container>
bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
{
    return (lhs.c() >= rhs.c());
}

};