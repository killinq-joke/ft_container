#pragma once
#include <memory>
#include "utils.hpp"

namespace ft
{

template <class Pair, class Node = node<Pair>, class Compare = less<Pair>, class Alloc = std::allocator<Node> >
class rbtree
{
public:
	typedef typename Pair::first_type			key_type;
	typedef typename Pair::second_type			value_type;
	typedef Compare								key_compare;
	typedef size_t								size_type;
	typedef Alloc								allocator_type;
	typedef Node*								NodePtr;

	rbtree() : comparator(Compare()), _size(0), _alloc(allocator_type())
	{
		TNULL = _alloc.allocate(1);
		_alloc.construct(TNULL, Node());
		TNULL->color = BLACK;
		TNULL->isnull = true;
		TNULL->left = TNULL;
		TNULL->right = TNULL;
		TNULL->parent = TNULL;
		root = TNULL;
		REND = this->createNode();
		REND->isnull = true;
		REND->isrend = true;
		REND->left = nullptr;
		TNULL->left = REND;
	}

	NodePtr createNode(void)
	{
		NodePtr newNode = _alloc.allocate(1);

		_alloc.construct(newNode, Node());
		newNode->color = BLACK;
		newNode->left = TNULL;
		newNode->right = TNULL;
		newNode->parent = newNode;
		newNode->isrend = false;
		return (newNode);
	}

	~rbtree()
	{
		deleteAll(this->root);
		delete TNULL;
	}

	size_type size()
	{
		return this->_size;
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

	NodePtr search(key_type k)
	{
		return search(this->root, k);
	}

	NodePtr min(NodePtr node)
	{
		while (!node->left->isnull)
			node = node->left;
		return node;
	}

	NodePtr max(NodePtr node)
	{
		while (!node->right->isnull)
			node = node->right;
		return node;
	}

	NodePtr successor(NodePtr x)
	{
		if (!x->right->isnull)
			return min(x->right);

		NodePtr y = x->parent;
		while (!y->isnull && x == y->right)
		{
			x = y;
			y = y->parent;
		}
		return y;
	}

	NodePtr predecessor(NodePtr x)
	{
		if (!x->left->isnull)
			return max(x->left);

		NodePtr y = x->parent;
		while (!y->isnull && x == y->left)
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
		if (!y->left->isnull)
			y->left->parent = x;
		y->parent = x->parent;
		if (x->parent->isnull)
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
		if (!y->left->isnull)
			y->right->parent = x;
		y->parent = x->parent;
		if (x->parent == TNULL)
			this->root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

	NodePtr insert(NodePtr begin, Pair val)
	{
		NodePtr node = this->createNode();

		node->color = RED;
		node->data = val;

		NodePtr y = TNULL;
		NodePtr x = begin;

		while (!x->isnull)
		{
			y = x;
			if (comparator(node->data, x->data))
				x = x->left;
			else
				x = x->right;
		}

		node->parent = y;
		if (y->isnull)
			root = node;
		else if (comparator(node->data, y->data))
			y->left = node;
		else
			y->right = node;

		if (node->parent->isnull)
		{
			node->color = BLACK;
			TNULL->parent = this->max(this->root);
			this->min(this->root)->left = REND;
			REND->parent = this->min(this->root);
			_size++;
			return node;
		}

		if (node->parent->parent->isnull)
		{
			TNULL->parent = this->max(this->root);
			this->min(this->root)->left = REND;
			REND->parent = this->min(this->root);
			_size++;
			return node;
		}

		insertFix(node);
		TNULL->parent = this->max(this->root);
		this->min(this->root)->left = REND;
		REND->parent = this->min(this->root);
		_size++;
		return node;
	}

	NodePtr	end()
	{
		return (TNULL);
	}

	NodePtr	begin()
	{
		return (this->min(this->root));
	}

	NodePtr	rbegin()
	{
		return (this->max(this->root));
	}

	NodePtr	rend()
	{
		return (REND);
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
	NodePtr			root;
	NodePtr			TNULL;
	NodePtr			REND;
	key_compare 	comparator;
	size_type		_size;
	allocator_type	_alloc;

	void prefix(NodePtr node)
	{
		if (!node->isnull)
		{
			std::cout << node->data << " ";
			prefix(node->left);
			prefix(node->right);
		}
	}

	void infix(NodePtr node)
	{
		if (!node->isnull)
		{
			infix(node->left);
			std::cout << node->data << " ";
			infix(node->right);
		}
	}

	void deleteAll(NodePtr node)
	{
		if (!node->isnull)
		{
			deleteAll(node->left);
			deleteAll(node->right);
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}
	}

	NodePtr search(NodePtr node, key_type key)
	{
		if (node->isnull || key == node->data)
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
		if (u->parent->isnull)
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

		while (!node->isnull)
		{
			if (node->data == val)
				z = node;

			if (comparator(node->data, val))
				node = node->right;
			else
				node = node->left;
		}

		if (z->isnull)
		{
			std::cout << "Key not found in the tree" << std::endl;
			return;
		}

		y = z;
		colors y_original_color = y->color;
		if (z->left->isnull)
		{
			x = z->right;
			swap(z, z->right);
		}
		else if (z->right->isnull)
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
				x->parent = y;
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
		_size--;
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
		if (!root->isnull)
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

}
