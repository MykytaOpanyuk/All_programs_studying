#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "market.h"
#include <string>

#define NMAX 100000
using namespace std;

class tree_node {
public:
	string group;
	string name;
	double price;
	int key;
	int degree;
	bool mark; // is it node loose his sons
	tree_node *parent;
	vector<tree_node *> children;
	tree_node *left;
	tree_node *right;

	tree_node(product_element *data)
	{
		degree = 0;
		parent = NULL;
		mark = false;
		key = data->key;
		name = data->name;
		group = data->group;
		price = data->price;
	}
};

void delete_tree(tree_node *cur)
{
	if (cur == NULL)
		return;
	if (cur->degree != 0)
		for (unsigned int i = 0; i < cur->children.size(); i++)
			delete_tree(cur->children[i]);

	delete cur;
}

struct Heap {
	vector<tree_node *> nodes;
	int n;
	tree_node *min;
};

class Fibonacci_heap {

public:
	vector<Heap *> heaps;

	Fibonacci_heap();
	~Fibonacci_heap()
	{
		for (unsigned int i = 0; i < heaps.size(); i++)
			heap_delete(heaps[i]);
	}
	Heap *construct();
	int find_position(Heap *f_heap, tree_node *x);
	void cut(Heap *f_heap, tree_node *x, tree_node *y);
	void cascading_cut(Heap *f_heap, tree_node *y);
	void heap_link(Heap *f_heap, tree_node *x, tree_node *y);
	void insert_data(Heap *f_heap, product_element *data);
	void insert_node(Heap *f_heap, tree_node *x);
	void unite_heaps(int first, int second);
	void heap_delete(Heap *f_heap);
	void delete_node(Heap *f_heap, tree_node *x);
	void consolidate(Heap *f_heap);
	tree_node *extract_min(Heap *f_heap);
	void decrease_key(Heap *f_heap, tree_node *x, int data);
};

Fibonacci_heap::Fibonacci_heap()
{
	heaps.push_back(construct());

	for (unsigned int i = 0; i < main_data->array_of_products.size(); i++)
		insert_data(heaps[0], main_data->array_of_products[i]);

	cout << "products from file were added to Fibonacci heap." << endl;

	int check;

	do {
		cout << "Heaps : ";
		cout << "Main heap (products from file) is 0";
		for (unsigned int i = 1; i < heaps.size(); i++)
			cout << ", " << i;
		cout << endl;
		cout << "1.Create another heap." << endl;
		cout << "2.Unite 2 selected heaps." << endl;
		cout << "3.Delete selected heap." << endl;
		cout << "4.Change selected heap." << endl;
		cout << "5.Exit." << endl;

		cin >> check;

		if (check == 1) {
			heaps.push_back(construct());
			cout << "New heap is already created." << endl;
		}
		if (check == 2) {
			unsigned index_1, index_2;

			cout << "Which heaps (2) you want to select ? " << endl;
			do {
				cout << "First :";
				cin >> index_1;
				cout << endl;
				if (index_1 > heaps.size())
					cout << "Wrong index" << endl;
			} while (index_1 > heaps.size());
			do {
				cout << "Second :";
				cin >> index_2;
				cout << endl;
				if (index_2 > heaps.size())
					cout << "Wrong index" << endl;
			} while (index_2 > heaps.size());

			unite_heaps(index_1, index_2);

			cout << index_1 << " and " << index_2 << " heaps have been succesfully deleted." << endl <<
				"Added new heap." << endl;
		}
		if (check == 3) {
			unsigned index;

			cout << "Which heap you want to delete ? " << endl;
			do {
				cout << "Index :";
				cin >> index;
				cout << endl;
				if (index > heaps.size())
					cout << "Wrong index" << endl;
			} while (index > heaps.size());

			heap_delete(heaps[index]);

			cout << index << "heap has been succesfully deleted." << endl;

		}
		if (check == 4) {
			unsigned int index, check_2;

			cout << "Which heap you want to select ? " << endl;
			do {
				cout << "Index :";
				cin >> index;
				cout << endl;
				if (index > heaps.size())
					cout << "Wrong index" << endl;
			} while (index > heaps.size());

			do {
				vector<tree_node *> dataVault;
				tree_node *ptr = NULL;

				for (unsigned int i = 0; i < main_data->array_of_products.size(); i++) {
					tree_node *new_element = new tree_node(main_data->array_of_products[i]);
					dataVault.push_back(new_element);
					cout << new_element->name << " || ";
				}

				cout << endl << "1.Get min." << endl;
				cout << "2.Insert new product." << endl;
				cout << "3.Delete product from heap." << endl;
				cout << "4.Exctract min." << endl;
				cout << "5.Exit." << endl;

				cin >> check_2;

				if (check_2 == 1) {
					cout << heaps[index]->min->key << endl;
					cout << heaps[index]->min->name << endl;
					cout << heaps[index]->min->group << endl;
					cout << heaps[index]->min->price << endl;
				}

				if (check_2 == 2)
					insert_data(heaps[index], main_data->new_data());

				if (check - 2 == 3) {
					string to_delete;
					bool founded = false;

					cout << "Write name of product to delete :" << endl;

					cin.get();
					getline(cin, to_delete);

					for (unsigned int i = 0; i < dataVault.size(); i++)
						if (dataVault[i]->name == to_delete) {
							ptr = dataVault[i];
							founded = true;
							break;
						}

					if (founded) {
						delete_node(heaps[index], ptr);

						for (size_t i = 0; i < dataVault.size(); i++)
							if (dataVault[i] == ptr) {
								dataVault.erase(dataVault.begin() + i);
								break;
							}
						main_data->delete_data(to_delete);
					}
				}

				if (check_2 == 4) {
					ptr = extract_min(heaps[index]);
					main_data->delete_data(ptr->name);
					for (size_t i = 0; i < dataVault.size(); i++)
						if (dataVault[i] == ptr) {
							dataVault.erase(dataVault.begin() + i);
							break;
						}
				}

			} while (check_2 != 5);

		}
	} while (check != 5);
	main_data->save_changing();
}

Heap *Fibonacci_heap::construct()
{
	Heap *new_heap = new Heap;
	new_heap->min = NULL;
	new_heap->n = 0;
	return new_heap;
}

int Fibonacci_heap::find_position(Heap *f_heap, tree_node *x)
{
	if (f_heap != NULL)
		for (unsigned int i = 0; i < f_heap->nodes.size(); i++)
			if (f_heap->nodes[i]->key == x->key)
				return i;
	return 0;
}

void Fibonacci_heap::cut(Heap *f_heap, tree_node *x, tree_node *y)
{
	for (unsigned int i = 0; i < y->children.size(); i++)
		if (y->children[i]->key == x->key) {
			y->children[i]->left->right = y->children[i]->right;
			y->children[i]->right->left = y->children[i]->left;
			y->degree--;
		}
	insert_node(f_heap, x);
	x->parent = NULL;
	x->mark = false;
}

void Fibonacci_heap::cascading_cut(Heap *f_heap, tree_node *y)
{
	tree_node *z = y->parent;
	if (z != NULL) {
		if (y->mark == false)
			y->mark = true;
		else {
			cut(f_heap, y, z);
			cascading_cut(f_heap, z);
		}
	}
}

void Fibonacci_heap::heap_link(Heap *f_heap, tree_node *y, tree_node *x)
{
	int i = find_position(f_heap, y);
	if (y->left != NULL)
		y->left->right = y->right;
	if (y->right != NULL)
		y->right->left = y->left;

	x->children.push_back(y);
	if (x->children.size() == 1)
		y->left = y->right = y;
	else {
		y->left = x->children[x->children.size() - 2];
		y->right = x->children[0];
	}
	x->degree = x->degree + y->degree;
	y->mark = false;
}

void Fibonacci_heap::insert_data(Heap *f_heap, product_element *data)
{
	tree_node *x = new tree_node(data);

	f_heap->nodes.push_back(x);

	if (f_heap->min == NULL) {
		f_heap->min = x;
		x->left = x;
		x->right = x;
	}
	else if (x->key < f_heap->min->key) {
		f_heap->min = x;
		f_heap->nodes[f_heap->nodes.size() - 1]->right = x;
		f_heap->nodes[0]->left = x;
		x->left = f_heap->nodes[f_heap->nodes.size() - 2];
		x->right = f_heap->nodes[0];
	}
	f_heap->n++;
}

void Fibonacci_heap::insert_node(Heap *f_heap, tree_node *x)
{
	x->parent = NULL;
	x->mark = false;

	if (f_heap == NULL)
		f_heap = construct();

	f_heap->nodes.push_back(x);

	if (f_heap->min == NULL) {
		f_heap->min = x;
		x->left = x;
		x->right = x;
	}
	else if (x->key < f_heap->min->key) {
		f_heap->min = x;
		f_heap->nodes[f_heap->nodes.size() - 1]->right = x;
		f_heap->nodes[0]->left = x;
		x->left = f_heap->nodes[f_heap->nodes.size() - 2];
		x->right = f_heap->nodes[0];
	}
	f_heap->n++;
}

void Fibonacci_heap::unite_heaps(int first, int second) //index of heaps in vector
{
	heaps.push_back(construct());
	if (heaps[first] != NULL)
		for (int i = 0; i < heaps[first]->n; i++)
			heaps[heaps.size()]->nodes.push_back(heaps[first]->nodes[i]);
	heaps[heaps.size()]->n = heaps[heaps.size()]->n + heaps[first]->n;

	if (heaps[second] != NULL)
		for (int i = 0; i < heaps[first]->n; i++)
			heaps[heaps.size()]->nodes.push_back(heaps[first]->nodes[i]);
	heaps[heaps.size()]->n = heaps[heaps.size()]->n + heaps[second]->n;

	heaps[heaps.size()]->nodes[0]->left = heaps[heaps.size()]->nodes[heaps[heaps.size()]->nodes.size()];
	heaps[heaps.size()]->nodes[heaps[heaps.size()]->nodes.size()]->right = heaps[heaps.size()]->nodes[0];
	heaps[first]->nodes[heaps[first]->nodes.size()]->right = heaps[second]->nodes[0];
	heaps[second]->nodes[0]->left = heaps[first]->nodes[heaps[first]->nodes.size()];

	heaps[heaps.size()]->min = heaps[first]->min;

	if ((heaps[heaps.size()]->min == NULL) || ((heaps[second]->min != NULL) && (heaps[second]->min->key < heaps[heaps.size()]->min->key)))
		heaps[heaps.size()]->min = heaps[second]->min;

	heap_delete(heaps[first]);
	heap_delete(heaps[second]);
	heaps.erase(heaps.begin() + first); // delete first
	heaps.erase(heaps.begin() + second); // delete second
}

void Fibonacci_heap::heap_delete(Heap *f_heap)
{
	if (f_heap == NULL)
		return;

	unsigned int i = 0;
	tree_node *cur = f_heap->min;

	while (i < f_heap->nodes.size()) {
		tree_node *timetable = cur->right;
		delete_tree(cur);
		cur = timetable;
		i++;
	}
}

void Fibonacci_heap::delete_node(Heap *f_heap, tree_node *x)
{
	decrease_key(f_heap, x, -NMAX);
	extract_min(f_heap);
}

void Fibonacci_heap::consolidate(Heap *f_heap)
{
	vector<tree_node *> array;

	int k = (int)log2(f_heap->n);

	for (int i = 0; i <= k; i++)
		array.push_back(NULL);

	for (unsigned int i = 0; i < f_heap->nodes.size(); i++) {
		tree_node *x = f_heap->nodes[i];
		int d = x->degree;
		while (array[d] != NULL) {
			tree_node *y = array[d];
			if (x->key > y->key)
				swap(x, y);
			heap_link(f_heap, x, y);
			array[d] = NULL;
			d++;
		}
		array[d] = x;
	}
	f_heap->min = NULL;

	for (int i = 0; i <= k; i++)
		if (array[i] != NULL) {
			if (f_heap->min == NULL) {
				f_heap->nodes.push_back(array[i]);
				f_heap->min = array[i];
				array[i]->left = array[i];
				array[i]->right = array[i];
			}
			else
				insert_node(f_heap, array[i]);
		}
}

tree_node *Fibonacci_heap::extract_min(Heap *f_heap)
{
	tree_node *to_extract = f_heap->min;
	if (to_extract != NULL) {
		for (unsigned int i = 0; i < to_extract->children.size(); i++)
			insert_node(f_heap, to_extract->children[i]);
	}
	f_heap->min = NULL;

	int i = find_position(f_heap, to_extract);
	tree_node *y = f_heap->nodes[i];

	if (y->left != NULL)
		y->left->right = y->right;
	if (y->right != NULL)
		y->right->left = y->left;

	f_heap->nodes.erase(f_heap->nodes.begin() + i);

	if (to_extract == to_extract->right)
		f_heap = NULL;
	else {	
		f_heap->min = to_extract->right;
		consolidate(f_heap);
	}
	f_heap->n--;
	return to_extract;
}

void Fibonacci_heap::decrease_key(Heap *f_heap, tree_node *x, int data)
{
	if (data > x->key) {
		cout << "ERROR!" << endl;
		return;
	}
	x->key = data;
	tree_node *y = x->parent;
	if ((y != NULL) && (x->key < y->key)) {
		cut(f_heap, x, y);
		cascading_cut(f_heap, y);
	}
	if (x->key < f_heap->min->key)
		f_heap->min = x;
}


