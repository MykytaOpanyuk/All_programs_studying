#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "market.h"
#include "Group.h"

using namespace std;

type_element *check_to_use;

struct products {
	string name;
	string group;
	double price;
	unsigned int hash_key;
	products *left;
	products *right;
	products *parent;
};


class Splay_tree_product
{
public:
	products * root; //root of RB tree

	~Splay_tree_product() {};
	Splay_tree_product(type_element *pr);

	products *rotate_left(products *x);
	products *rotate_right(products *x);
	products *Splay_balance(products *x, unsigned int data);
	products *insert_node(product_element *product_el);
	products *delete_node(unsigned int data);
	products *is_node(unsigned int data);
	bool cout_tree();
	bool delete_tree();
	void add_new_data();
	void ST_menu();
};

Splay_tree_product::Splay_tree_product(type_element *pr)
{
	root = NULL;

	for (int i = 0; i < (int)pr->array_of_products.size(); i++)
		insert_node(pr->array_of_products[i]);

	ST_menu();
}

products *Splay_tree_product::rotate_left(products *x)
{
	products *y = x->right;

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

products *Splay_tree_product::rotate_right(products *x)
{
	products *y = x->left;

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

products *Splay_tree_product::Splay_balance(products *x, unsigned int data)
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

products *Splay_tree_product::insert_node(product_element *product_el)
{ //working with RB_tree for the first time
	products *y, *x = root;

	x = Splay_balance(x, product_el->hash_key);
	if (x != NULL)
		if (x->hash_key == product_el->hash_key)
			return x;


	//initialize node
	y = new products;
	if (!y)
		return NULL;
	y->hash_key = product_el->hash_key;
	y->group = product_el->group;
	y->name = product_el->name;
	y->price = product_el->price;

	if (x == NULL) {
		y->left = NULL;
		y->right = NULL;
		y->parent = NULL;
		root = y;
	}
	else {

		if (x->hash_key > product_el->hash_key) { // insert node in tree
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

products *Splay_tree_product::delete_node(unsigned int data)
{
	products *to_delete = is_node(data), *x;

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

		delete x;

		x = NULL;
	}

	return to_delete;
}

products *Splay_tree_product::is_node(unsigned int data)
{
	products *current = root;
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

bool Splay_tree_product::cout_tree()
{
	if (root == NULL)
		return false;

	queue<products *> cout_queue;

	cout_queue.push(root);
	while (!cout_queue.empty()) {
		products *x = cout_queue.front();
		cout_queue.pop();

		if (x->left != NULL)
			cout_queue.push(x->left);
		if (x->right != NULL)
			cout_queue.push(x->right);

		cout << "Name : " << x->name << " " <<  x->hash_key << " ";
	}
	return true;
}


bool Splay_tree_product::delete_tree()
{
	if (root == NULL)
		return false;

	queue<products *> del_queue;

	del_queue.push(root);
	while (!del_queue.empty()) {
		products *node = del_queue.front();
		del_queue.pop();

		if (node->left != NULL)
			del_queue.push(node->left);
		if (node->right != NULL)
			del_queue.push(node->right);

		delete node;
	}

	return true;
}

void Splay_tree_product::add_new_data()
{
	string new_name;

	cout << "Write a name of product: " << endl;
	cin.ignore();
	getline(cin, new_name);
	int i = main_data->check_is_type(check_to_use->group);
	int pos = main_data->check_is_product(i, new_name);

	if (pos == -1) {
		main_data->size_of_f++;
		main_data->type_of_products[i]->array_of_products.push_back(new product_element);

		unsigned int size = unsigned int(main_data->type_of_products[i]->array_of_products.size()) - 1;

		main_data->type_of_products[i]->array_of_products[size]->name = new_name;

		cout << "Write a price of product:" << endl;
		cin >> main_data->type_of_products[i]->array_of_products[size]->price;
		main_data->type_of_products[i]->array_of_products[size]->group = check_to_use->group;
		main_data->type_of_products[i]->array_of_products[size]->hash_key = hash_string(new_name);
		cout << "New product was added." << endl;
		main_data->save_changing();

		insert_node(main_data->type_of_products[i]->array_of_products[size]);
	}
}

void Splay_tree_product::ST_menu()
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
		if (check == 1)
			add_new_data();

		if (check == 2) {
			string product_p;

			cout << "Write a name of product: " << endl;
			cin.get();
			getline(cin, product_p);
			int i = main_data->check_is_type(check_to_use->group);
			int pos = main_data->check_is_product(i, product_p);
			if (pos == -1)
				return;

			root = delete_node(main_data->type_of_products[i]->array_of_products[pos]->hash_key);

			unsigned int size = unsigned int(main_data->type_of_products[i]->array_of_products.size());

			main_data->type_of_products[i]->array_of_products.erase(main_data->type_of_products[i]->array_of_products.begin() + pos);
			if (size == 1)
				main_data->type_of_products.erase(main_data->type_of_products.begin() + i);
			main_data->size_of_f--;
			main_data->save_changing();


		}
	}
	delete_tree();
};

