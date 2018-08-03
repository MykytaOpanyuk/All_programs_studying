#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "product.h"

using namespace std;

struct type {
	node_colour colour;
	string group;
	unsigned int hash_key;
	unsigned int rank;
	RB_tree_product *new_tree;
	type *left;
	type *right;
	type *parent;
};

class RB_tree_group
{

public:

	type * type_son; // like NULL node
	type *root; //root of RB tree

	~RB_tree_group();
	RB_tree_group(market *pr);
	void rotate_left(type *x);
	void rotate_right(type *x);
	void insert_balance(type *x);
	type *insert_node(type_element *type_el);
	void delete_balance(type *x);
	void delete_node(type *z);
	type *is_node(unsigned int data);
	bool cout_tree(type *current);
	type *os_select(type *x, unsigned int i);
	int os_rank(type *x);
	bool delete_tree(type *current);
	void RBT_menu();
};
RB_tree_group::~RB_tree_group()
{

}

RB_tree_group::RB_tree_group(market *pr)
{
	type_son = new type;

	type_son->left = NULL;
	type_son->right = NULL;
	type_son->new_tree = NULL;
	type_son->hash_key = 0;
	type_son->group = "";
	type_son->parent = NULL;
	type_son->colour = Black;
	type_son->rank = 0;

	root = type_son;

	for (int i = 0; i < (int)pr->type_of_products.size(); i++) {
		insert_node(pr->type_of_products[i]);
	}

	RBT_menu();
}

void RB_tree_group::rotate_left(type *x)
{
	type *y = x->right;

	x->right = y->left; //establish x->right link
	if (y->left != type_son)
		y->left->parent = x;
	if (y != type_son) //establish y->parent link
		y->parent = x->parent;
	if (x->parent == NULL)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;// link x and y

	if (x != type_son)
		x->parent = y;

	y->rank = x->rank;
	x->rank = x->left->rank + x->right->rank + 1;
}



void RB_tree_group::rotate_right(type *x)
{
	type *y = x->left;

	x->left = y->right; // establish x->left link

	if (y->right != type_son)
		y->right->parent = x;

	if (y != type_son) // establish y->parent link
		y->parent = x->parent;

	if (x->parent == NULL)
		root = y;

	else if (x == x->parent->right)
		x->parent->left = y;

	else
		x->parent->right = y;

	y->right = x; // link x and y

	if (x != type_son)
		x->parent = y;

	y->rank = x->rank;
	x->rank = x->left->rank + x->right->rank + 1;
}



void RB_tree_group::insert_balance(type *x)
{
	while ((x != root) && (x->parent->colour == Red)) { // check is node a root
		if (x->parent == x->parent->parent->left) { // have a violation
			type *y = x->parent->parent->right;
			if (y->colour == Red) { // uncle is red
				x->parent->colour = Black;
				y->colour = Black;
				x->parent->parent->colour = Red;
				x = x->parent->parent;
			}
			else { // uncle is black
				if (x == x->parent->right) { // make x a left child
					x = x->parent;
					rotate_left(x);
				}

				x->parent->colour = Black; //change colour
				x->parent->parent->colour = Red;
				rotate_right(x->parent->parent);
			}
		}
		else { // mirror image of above code
			type *y = x->parent->parent->left;
			if (y->colour == Red) { // uncle is red
				x->parent->colour = Black;
				y->colour = Black;
				x->parent->parent->colour = Red;
				x = x->parent->parent;
			}
			else { // uncle is black
				if (x == x->parent->left) {
					x = x->parent;
					rotate_right(x);
				}
				x->parent->colour = Black;
				x->parent->parent->colour = Red;
				rotate_left(x->parent->parent);
			}
		}
	}

	root->colour = Black;
}



type *RB_tree_group::insert_node(type_element *type_el)
{ //working with RB_tree for the first time
	type *current = root, *parent, *x;
	parent = 0;

	while (current != type_son) {
		if (type_el->hash_key == current->hash_key)
			return current;
		parent = current;
		//if ((current->left != son) || (current->right != son))
		current->rank++;
		if (type_el->hash_key < current->hash_key)
			current = current->left;
		else
			current = current->right;
	}

	x = new type;
	if (x == NULL) {
		cout << "Not enough memory!" << endl;
		return NULL;
	}

	x->group = type_el->group;
	x->hash_key = type_el->hash_key;
	x->parent = parent;
	x->left = type_son;
	x->right = type_son;
	x->colour = Red;
	x->rank = 1;
	x->new_tree = new RB_tree_product(type_el);

	if (parent) { // insert node in tree
		if (type_el->hash_key < parent->hash_key)
			parent->left = x;
		else
			parent->right = x;
	}
	else
		root = x;

	insert_balance(x);
	return x;
}



void RB_tree_group::delete_balance(type *x)
{  //working with main version of RB_tree
	type* y = x;

	while (y) {
		y->rank--;
		y = y->parent;
	}

	while ((x != root) && (x->colour == Black)) {
		if (x == x->parent->left) {
			type *w = x->parent->right;
			if (w->colour == Red) {
				w->colour = Black;
				x->parent->colour = Red;
				rotate_left(x->parent);
				w = x->parent->right;
			}

			if (w->left->colour == Black && w->right->colour == Black) {
				w->colour = Red;
				x = x->parent;
			}

			else {
				if (w->right->colour == Black) {
					w->left->colour = Black;
					w->colour = Red;
					rotate_right(w);
					w = x->parent->right;
				}

				w->colour = x->parent->colour;
				x->parent->colour = Black;
				w->right->colour = Black;
				rotate_left(x->parent);
				x = root;
			}
		}

		else {
			type *w = x->parent->left;
			if (w->colour == Red) {
				w->colour = Black;
				x->parent->colour = Red;
				rotate_right(x->parent);
				w = x->parent->left;
			}

			if (w->right->colour == Black && w->left->colour == Black) {
				w->colour = Red;
				x = x->parent;
			}

			else {
				if (w->left->colour == Black) {
					w->right->colour = Black;
					w->colour = Red;
					rotate_left(w);
					w = x->parent->left;
				}

				w->colour = x->parent->colour;
				x->parent->colour = Black;
				w->left->colour = Black;
				rotate_right(x->parent);
				x = root;
			}
		}
	}

	x->colour = Black;
}



void RB_tree_group::delete_node(type *z)
{
	type *x, *y;

	if ((z == NULL) || (z == type_son))
		return;

	if ((z->left == type_son) || (z->right == type_son))
		y = z;

	else { // find tree successor with a NIL node as a child
		y = z->right;
		while (y->left != type_son)
			y = y->left;
	}

	if (y->left != type_son) // x is y's only child
		x = y->left;
	else {
		x = y->right;
		x->rank--;
	}

	// remove y from the parent chain
	x->parent = y->parent;

	if (y->parent)
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	else
		root = x;

	if (y != z)
		z->hash_key = y->hash_key;

	if (y->colour == Black)
		delete_balance(x);

	y->new_tree->delete_pr_tree(y->new_tree->root);

	delete y;
}



type *RB_tree_group::is_node(unsigned int data)
{
	type *current = root;

	while ((current != type_son) && (current != NULL)) {
		if (data == current->hash_key)
			return current;
		else
			if (data < current->hash_key)
				current = current->left;
			else current = current->right;
	}

	return NULL;
}



bool RB_tree_group::cout_tree(type *current)
{
	if ((current != type_son) && (current != NULL)) {
		if ((current->new_tree->root == current->new_tree->product_son) || (current->new_tree->root == NULL))
			delete_node(current);
		else 
			cout << current->group << ", rank = " << current->rank << ". ";
		cout_tree(current->left);
		cout_tree(current->right);
		return true;
	}
	else
		return false;
}



type *RB_tree_group::os_select(type *x, unsigned int i) // finding rang of node, using decrease of i
{ // i is a sum of node's rang
	unsigned int rang = x->left->rank + 1;

	if (i == rang)
		return x;
	else {
		if (i < rang)
			return os_select(x->left, i);
		else
			return os_select(x->right, (i - rang));
	}
}



int RB_tree_group::os_rank(type *x)
{
	unsigned int rank = x->left->rank + 1;
	type *y = x;

	while (y != root) {
		if (y == y->parent->right)
			rank = rank + y->parent->left->rank + 1;
		y = y->parent;
	}

	return rank;
}



bool RB_tree_group::delete_tree(type *current)
{
	if ((current != type_son) && (current != NULL)) {
		delete_tree(current->left);
		delete_tree(current->right);

		delete current;
		current = NULL;
		return true;
	}
	else
		return false;
}

void RB_tree_group::RBT_menu()
{
	int check = 1;

	while (check != 0) {
		cout << endl << "Main version of main tree is ready." << endl;
		cout_tree(root);
		cout << endl << "Welcome to \"order-statistic tree\" menu." << endl;
		cout << "1 - Check rank of node." << endl;
		cout << "2 - Find out a node by decreasing of rank." << endl;
		cout << "3 - Delete node." << endl;
		cout << "0 - exit." << endl;
		cin >> check;

		if (check == 1) {
			string name;
			cout << "Write a name of group to check rank." << endl;
			cin >> name;
			cout << "Rank is : " << os_rank(is_node(hash_string(name))) << endl;
		}

		if (check == 2) {
			int rank;
			cout << "Write down a rank." << endl;
			cin >> rank;
			type *check = os_select(root, rank);
			cout << "The key of node is " << check->hash_key << endl;
		}

		if (check == 3) {
			string name;
			cout << "Write a name of group to delete." << endl;
			cin >> name;
			delete_node(is_node(hash_string(name)));
		}

		if (check == 0)
			delete_tree(root);
	}
}
