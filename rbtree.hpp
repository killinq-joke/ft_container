#pragma once
#include <memory>
#include "utils/utils.hpp"

namespace ft
{

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const pair<T1, T2> p)
{
    os << "key == " << p.first << " value == " << p.second;
    return os;
}

enum	colors
{
	BLACK,
	RED
};

template <class Pair>
struct node {
  Pair data;
  node *parent;
  node *left;
  node *right;
  colors color;
};


template <class Pair, class Compare = less<Pair> >
class rbtree
{
public:
	typedef node<Pair>					Node;
	typedef Node*						NodePtr;
	typedef typename Pair::first_type	key_type;
	typedef typename Pair::second_type	value_type;
	typedef Compare						key_compare;

	rbtree() : comparator(Compare())
	{
		TNULL = new Node;
		TNULL->color = BLACK;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	void prefix()
	{
		prefix(this->root);
	}

	void infix()
	{
		infix(this->root);
	}

	void suffix()
	{
		suffix(this->root);
	}

	NodePtr search(value_type k)
	{
		return search(this->root, k);
	}

	NodePtr min(NodePtr node)
	{
		while (node->left != TNULL)
			node = node->left;
		return node;
	}

	NodePtr maximum(NodePtr node)
	{
		while (node->right != TNULL)
			node = node->right;
		return node;
	}

	NodePtr successor(NodePtr x)
	{
		if (x->right != TNULL)
			return min(x->right);

		NodePtr y = x->parent;
		while (y != TNULL && x == y->right)
		{
			x = y;
			y = y->parent;
		}
		return y;
	}

	NodePtr predecessor(NodePtr x)
	{
		if (x->left != TNULL)
			return maximum(x->left);

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left)
		{
			x = y;
			y = y->parent;
		}

		return y;
	}

	void leftRotate(NodePtr x)
	{
		NodePtr y = x->right;

		x->right = y->left;
		if (y->left != TNULL)
			y->left->parent = x;
		y->parent = x->parent;
		if (x->parent == nullptr)
			this->root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	void rightRotate(NodePtr x)
	{
		NodePtr y = x->left;

		x->left = y->right;
		if (y->right != TNULL)
			y->right->parent = x;
		y->parent = x->parent;
		if (x->parent == nullptr)
			this->root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

	void insert(Pair val)
	{
		NodePtr node = new Node;

		node->parent = nullptr;
		node->data = val;
		node->left = TNULL;
		node->right = TNULL;
		node->color = RED;

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL)
		{
			y = x;
			if (comparator(node->data, x->data))
				x = x->left;
			else
				x = x->right;
		}

		node->parent = y;
		if (y == nullptr)
			root = node;
		else if (comparator(node->data, y->data))
			y->left = node;
		else
			y->right = node;

		if (node->parent == nullptr)
		{
			node->color = BLACK;
			return;
		}

		if (node->parent->parent == nullptr)
			return;

		insertFix(node);
	}

	NodePtr getRoot()
	{
		return this->root;
	}

	void deleteNode(Pair data)
	{
		deleteNodeHelper(this->root, data);
	}

	void printTree()
	{
		if (root)
			print(this->root, "", true);
	}

private:
	NodePtr root;
	NodePtr TNULL;
	key_compare comparator;

	void initializeNULLNode(NodePtr node, NodePtr parent)
	{
		node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = BLACK;
	}

	void prefix(NodePtr node)
	{
		if (node != TNULL)
		{
			std::cout << node->data << " ";
			prefix(node->left);
			prefix(node->right);
		}
	}

	void infix(NodePtr node)
	{
		if (node != TNULL)
		{
			infix(node->left);
			std::cout << node->data << " ";
			infix(node->right);
		}
	}

	void suffix(NodePtr node)
	{
		if (node != TNULL)
		{
			suffix(node->left);
			suffix(node->right);
			std::cout << node->data << " ";
		}
	}

	NodePtr search(NodePtr node, key_type key)
	{
		if (node == TNULL || key == node->data)
			return node;

		if (comparator(key, node->data))
			return search(node->left, key);

		return search(node->right, key);
	}

	void deleteFix(NodePtr x)
	{
		NodePtr s;

		while (x != root && x->color == BLACK)
		{
			if (x == x->parent->left)
			{
				s = x->parent->right;
				if (s->color == RED)
				{
					s->color = BLACK;
					x->parent->color = RED;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == BLACK && s->right->color == BLACK)
				{
					s->color = RED;
					x = x->parent;
				}
				else
				{
					if (s->right->color == BLACK)
					{
						s->left->color = BLACK;
						s->color = RED;
						rightRotate(s);
						s = x->parent->right;
					}

					s->color = x->parent->color;
					x->parent->color = BLACK;
					s->right->color = BLACK;
					leftRotate(x->parent);
					x = root;
				}
			}
			else
			{
				s = x->parent->left;
				if (s->color == RED)
				{
					s->color = BLACK;
					x->parent->color = RED;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == BLACK && s->right->color == BLACK)
				{
					s->color = RED;
					x = x->parent;
				}
				else
				{
					if (s->left->color == BLACK)
					{
						s->right->color = BLACK;
						s->color = RED;
						leftRotate(s);
						s = x->parent->left;
					}
					s->color = x->parent->color;
					x->parent->color = BLACK;
					s->left->color = BLACK;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = BLACK;
	}

	void swap(NodePtr u, NodePtr v)
	{
		if (u->parent == nullptr)
			root = v;
		else if (u == u->parent->left)
			u->parent->left = v;
		else
			u->parent->right = v;

		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, Pair val)
	{
		NodePtr z = TNULL;
		NodePtr x, y;

		while (node != TNULL)
		{
			if (node->data == val)
				z = node;

			if (comparator(node->data, val))
				node = node->right;
			else
				node = node->left;
		}

		if (z == TNULL)
		{
			std::cout << "Key not found in the tree" << std::endl;
			return;
		}

		y = z;
		colors y_original_color = y->color;
		if (z->left == TNULL)
		{
			x = z->right;
			swap(z, z->right);
		}
		else if (z->right == TNULL)
		{
			x = z->left;
			swap(z, z->left);
		}
		else
		{
			y = min(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z)
			{
				x->parent = y;
			}
			else
			{
				swap(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			swap(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == BLACK)
			deleteFix(x);
	}

	void insertFix(NodePtr k)
	{
		NodePtr u;

		while (k->parent->color == RED)
		{
			if (k->parent == k->parent->parent->right)
			{
				u = k->parent->parent->left;
				if (u->color == RED)
				{
					u->color = BLACK;
					k->parent->color = BLACK;
					k->parent->parent->color = RED;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->left)
					{
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = BLACK;
					k->parent->parent->color = RED;
					leftRotate(k->parent->parent);
				}
			}
			else
			{
				u = k->parent->parent->right;

				if (u->color == RED)
				{
					u->color = BLACK;
					k->parent->color = BLACK;
					k->parent->parent->color = RED;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->right)
					{
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = BLACK;
					k->parent->parent->color = RED;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root)
				break;
		}
		root->color = BLACK;
	}

	void print(NodePtr root, std::string indent, bool last) {
		if (root != TNULL)
		{
			std::cout << indent;
			if (last)
			{
				std::cout << "R----";
				indent += "   ";
			}
			else
			{
				std::cout << "L----";
				indent += "|  ";
			}

			std::string sColor = root->color ? "RED" : "BLACK";
			std::cout << root->data << "(" << sColor << ")" << std::endl;
			print(root->left, indent, false);
			print(root->right, indent, true);
		}
	}

};

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
	typedef ft::bidirectional_iterator<value_type> 		iterator;
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
	}

	// template <class InputIterator>
	// map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	// : _tree(rbtree<value_type, key_compare>()), _comparator(comp), _alloc(alloc)
	// {
	// }

	// map (const map& x)
	// {
	// }

	~map()
	{
	}

	// pair<iterator,bool> insert(const value_type& val)
	// {
		
	// 	while (_comparator(val.first, ))
	// 	{

	// 	}
	// 	return ();
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
