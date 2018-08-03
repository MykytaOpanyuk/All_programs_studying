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
	node_colour colour;
	string name;
	string group;
	double price;
	unsigned int hash_key;
	unsigned int rank;
	products *left;
	products *right;
	products *parent;
};

class RB_tree_product {

public:

	products * product_son; // like NULL node
	products *root; //root of RB tree

	RB_tree_product() {};

	RB_tree_product(type_element *pr)
	{
		product_son = new products;
		check_to_use = pr;

		product_son->left = NULL;
		product_son->right = NULL;
		product_son->hash_key = 0;
		product_son->group = "";
		product_son->price = 0;
		product_son->name = "";
		product_son->parent = NULL;
		product_son->colour = Black;
		product_son->rank = 0;

		root = product_son;

		for (int i = 0; i < (int)pr->array_of_products.size(); i++)
			insert_node(pr->array_of_products[i]);

		RBT_menu();
	}



	void rotate_left(products *x)
	{
		products *y = x->right;

		x->right = y->left; //establish x->right link
		if (y->left != product_son)
			y->left->parent = x;
		if (y != product_son) //establish y->parent link
			y->parent = x->parent;
		if (x->parent == NULL)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;

		y->left = x;// link x and y

		if (x != product_son)
			x->parent = y;

		y->rank = x->rank;
		x->rank = x->left->rank + x->right->rank + 1;

	}



	void rotate_right(products *x)
	{
		products *y = x->left;

		x->left = y->right; // establish x->left link

		if (y->right != product_son)
			y->right->parent = x;

		if (y != product_son) // establish y->parent link
			y->parent = x->parent;

		if (x->parent == NULL)
			root = y;

		else if (x == x->parent->right)
			x->parent->left = y;

		else
			x->parent->right = y;

		y->right = x; // link x and y

		if (x != product_son)
			x->parent = y;

		y->rank = x->rank;
		x->rank = x->left->rank + x->right->rank + 1;
	}



	void insert_balance(products *x)
	{
		while ((x != root) && (x->parent->colour == Red)) { // check is node a root
			if (x->parent == x->parent->parent->left) { // have a violation
				products *y = x->parent->parent->right;
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
				products *y = x->parent->parent->left;
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



	products *insert_node(product_element *data)
	{ //working with RB_tree for the first time
		products *current = root, *parent, *x;
		parent = NULL;

		while (current != product_son) {
			if (data->hash_key == current->hash_key)
				return current;
			parent = current;
			//if ((current->left != son) || (current->right != son))
			current->rank++;
			if (data->hash_key < current->hash_key)
				current = current->left;
			else
				current = current->right;
		}

		x = new products;
		if (x == NULL) {
			cout << "Not enough memory!" << endl;
			return NULL;
		}

		x->name = data->name;
		x->price = data->price;
		x->hash_key = data->hash_key;
		x->group = data->group;
		x->parent = parent;
		x->left = product_son;
		x->right = product_son;
		x->colour = Red;
		x->rank = 1;

		if (parent) { // insert node in tree
			if (data->hash_key < parent->hash_key)
				parent->left = x;
			else
				parent->right = x;
		}
		else
			root = x;

		insert_balance(x);
		return x;
	}



	void delete_balance(products *x)
	{  //working with main version of RB_tree
		products* y = x;

		while (y) {
			y->rank--;
			y = y->parent;
		}

		while ((x != root) && (x->colour == Black)) {
			if (x == x->parent->left) {
				products *w = x->parent->right;
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
				products *w = x->parent->left;
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



	void delete_node(products *z)
	{
		products *x, *y;

		if ((z == NULL) || (z == product_son))
			return;

		if ((z->left == product_son) || (z->right == product_son))
			y = z;
		else { // find tree successor with a NIL node as a child
			y = z->right;
			while (y->left != product_son)
				y = y->left;
		}

		if (y->left != product_son) // x is y's only child
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

		delete y;
	}

	products *is_node(unsigned int to_check)
	{
		products *current = root;

		while ((current != product_son) && (current != NULL)) {
			if (to_check == current->hash_key)
				return current;
			else
				if (to_check < current->hash_key)
					current = current->left;
				else current = current->right;
		}

		return NULL;
	}



	bool cout_tree(products *current)
	{
		if ((current != product_son) && (current != NULL)) {
			cout << current->name << ", rank = " << current->rank << ". ";
			cout_tree(current->left);
			cout_tree(current->right);
			return true;
		}
		else
			return false;
	}

	products *os_select(products *x, unsigned int i) // finding rang of node, using decrease of i
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



	int os_rank(products *x)
	{
		unsigned int rank = x->left->rank + 1;
		products *y = x;

		while (y != root) {
			if (y == y->parent->right)
				rank = rank + y->parent->left->rank + 1;
			y = y->parent;
		}

		return rank;
	}



	bool delete_pr_tree(products *current)
	{
		if ((current != product_son) && (current != NULL)) {
			delete_pr_tree(current->left);
			delete_pr_tree(current->right);
			delete current;
			current = NULL;
			return true;
		}
		else
			return false;
	}

	void RBT_menu()
	{
		int check = 1;

		while (check != 0) {
			cout << endl << "Main version of subtree is ready." << endl;
			cout_tree(root);
			cout << endl << "Welcome to \"order-statistic tree\" menu." << endl;
			cout << "1 - Add new node." << endl;
			cout << "2 - Check rank of node." << endl;
			cout << "3 - Find out a node by decreasing of rank." << endl;
			cout << "4 - Delete node." << endl;
			cout << "0 - exit." << endl;
			cin >> check;

			if (check == 1) {
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

			if (check == 2) {
				string name;
				cout << "Write name of product to check rank." << endl;
				cin >> name;
				unsigned int to_check = hash_string(name);
				cout << "Rank is : " << os_rank(is_node(to_check)) << endl;
			}

			if (check == 3) {
				int rank;
				cout << "Write down a rank." << endl;
				cin >> rank;
				products *check = os_select(root, rank);
				cout << "The key of node is " << check->hash_key << endl;
			}

			if (check == 4) {
				string product_p;

				cout << "Write a name of product: " << endl;
				cin.get();
				getline(cin, product_p);
				int i = main_data->check_is_type(check_to_use->group);
				int pos = main_data->check_is_product(i, product_p);
				if (pos == -1)
					return;

				delete_node(is_node(main_data->type_of_products[i]->array_of_products[pos]->hash_key));

				unsigned int size = unsigned int(main_data->type_of_products[i]->array_of_products.size());

				main_data->type_of_products[i]->array_of_products.erase(main_data->type_of_products[i]->array_of_products.begin() + pos);
				if (size == 1)
					main_data->type_of_products.erase(main_data->type_of_products.begin() + i);

				main_data->save_changing();
			}

			//if (check == 0)
			//	delete_pr_tree(root);

		}
	}
};
