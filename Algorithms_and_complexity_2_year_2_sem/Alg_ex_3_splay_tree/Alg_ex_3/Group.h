#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include "Product.h"
#include "market.h"

using namespace std;

struct type {
	string group;
	unsigned int hash_key;
	Splay_tree_product *new_tree;
	type *left;
	type *right;
	type *parent;
};


class Splay_tree_type
{
public:

	type * root; //root of RB tree

	~Splay_tree_type() {};
	Splay_tree_type(market *pr);

	type *rotate_left(type *x);
	type *rotate_right(type *x);
	type *Splay_balance(type *x, unsigned int data);
	type *insert_node(type_element *type_el);
	type *delete_node(unsigned int data);
	type *is_node(unsigned int data);
	bool cout_tree();
	bool delete_tree();
	void ST_menu();
};

Splay_tree_type::Splay_tree_type(market *pr)
{
	root = NULL;

	for (int i = 0; i < (int)pr->type_of_products.size(); i++)
		if (insert_node(pr->type_of_products[i]) == NULL) {
			cout << "Out of memory" << endl;
			break;
	}
	ST_menu();
}

type *Splay_tree_type::rotate_left(type *x)
{
	type *y = x->right;

	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	if (y != NULL) //establish y->parent link
		y->parent = x->parent;
	if (x->parent == NULL)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;// link x and y

	if (x != NULL)
		x->parent = y;

	return y;
}

type *Splay_tree_type::rotate_right(type *x)
{
	type *y = x->left;

	x->left = y->right; // establish x->left link
	if (y->right != NULL)
		y->right->parent = x;
	if (y != NULL) // establish y->parent link
		y->parent = x->parent;
	if (x->parent == NULL)
		root = y;
	else if (x == x->parent->right)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->right = x; // link x and y

	if (x != NULL)
		x->parent = y;

	return y;

}

type *Splay_tree_type::Splay_balance(type *x, unsigned int data)
{
	if ((x == NULL) || (x->hash_key == data))
		return x;
	if (x->hash_key > data) {
		if (x->left == NULL)
			return x;

		if (x->left->hash_key > data) { //Zig-Zig
			x->left->left = Splay_balance(x->left->left, data);
			x = rotate_right(x);
		}
		else if (x->left->hash_key < data) { // Zig-Zag
			x->left->right = Splay_balance(x->left->right, data);
			if (root->left->right != NULL)
				root->left = rotate_left(x->left);
		}
		if (x->left != NULL)
			rotate_right(x);
		else
			return x;
	}
	else {
		if (x->right == NULL)
			return x;
		if (x->right->hash_key > data) { // Zag-Zig
			x->right->left = Splay_balance(x->right->left, data);
			if (x->right->left != NULL)
				x->right = rotate_right(x->right);
		}
		else if (x->right->hash_key < data) { // Zag-Zag
			x->right->right = Splay_balance(x->right->right, data);
			x = rotate_left(x);
		}
		if (x->right != NULL)
			rotate_left(x);
		else
			return x;
	}
}

type *Splay_tree_type::insert_node(type_element *type_el)
{ //working with RB_tree for the first time
	type *y, *x = root;

	x = Splay_balance(x, type_el->hash_key);
	if (x != NULL)
		if (x->hash_key == type_el->hash_key)
			return x;


	//initialize node
	y = new type;
	if (!y)
		return NULL;

	check_to_use = type_el;
	y->hash_key = type_el->hash_key;
	y->group = type_el->group;
	y->new_tree = new Splay_tree_product(type_el);

	if (x == NULL) {
		y->left = NULL;
		y->right = NULL;
		y->parent = NULL;
		root = y;
	}
	else {

		if (x->hash_key > type_el->hash_key) { // insert node in tree
			if (x == root)
				root = y;
			y->parent = x->parent;
			y->right = x;
			y->left = x->left;
			x->left = NULL;
			x->parent = y;
		}
		else {
			if (x == root)
				root = y;
			y->parent = x->parent;
			y->right = x->right;
			y->left = x;
			x->right = NULL;
			x->parent = y;
		}
	}
	return y;
}

type *Splay_tree_type::delete_node(unsigned int data)
{
	type *to_delete = is_node(data), *x;

	if (to_delete != NULL) {
		to_delete = Splay_balance(root, data);
		x = to_delete;
		if (to_delete->left == NULL)
			to_delete = to_delete->right;
		else if (to_delete->right == NULL)
			to_delete = to_delete->left;
		else {
			to_delete = Splay_balance(to_delete->left, data);
			to_delete->right = x->right;
		}

		x->new_tree->delete_tree();

		delete x;
	
	}

	x = NULL;
	return to_delete;
}

type *Splay_tree_type::is_node(unsigned int data)
{
	type *current = root;
	while (current != NULL) {
		if (data == current->hash_key)
			return current;
		else
			if (data < current->hash_key)
				current = current->left;
			else current = current->right;
	}
	return NULL;
}

bool Splay_tree_type::cout_tree()
{
	if (root == NULL)
		return false;

	queue<type *> cout_queue;

	cout_queue.push(root);
	while (!cout_queue.empty()) {
		type *x = cout_queue.front();
		cout_queue.pop();

		if (x->left != NULL)
			cout_queue.push(x->left);
		if (x->right != NULL)
			cout_queue.push(x->right);

		cout << x->hash_key << " ";
	}
	return true;
}


bool Splay_tree_type::delete_tree()
{
	if (root == NULL)
		return false;

	queue<type *> del_queue;

	del_queue.push(root);
	while (!del_queue.empty()) {
		type *node = del_queue.front();
		del_queue.pop();

		if (node->left != NULL)
			del_queue.push(node->left);
		if (node->right != NULL)
			del_queue.push(node->right);

		delete node;
	}

	return true;
}

void Splay_tree_type::ST_menu()
{
	int check = 1;
	while (check != 0) {
		cout << endl << "Main version of tree is ready." << endl;
		cout_tree();
		cout << endl << "Welcome to \"order-statistic tree\" menu." << endl;
		cout << "1 - Add new node." << endl;
		cout << "2 - Delete node." << endl;
		cout << "0 - exit." << endl;
		cin >> check;
		if (check == 1) {
			string name;
			cout << "Write a name of group to delete." << endl;
			cin >> name;

			type_element *new_el = new type_element;
			new_el->group = name;
			new_el->hash_key = hash_string(name);
			type *new_node = insert_node(new_el);

			cout << name << " has been added." << endl;
		}
		if (check == 2) {
			string name;
			cout << "Write a name of group to delete." << endl;
			cin >> name;
			main_data->size_of_f = main_data->size_of_f - (main_data->type_of_products[main_data->check_is_type(name)]->array_of_products.size());
			root = delete_node(hash_string(name));

		}
	}
	delete_tree();
}

