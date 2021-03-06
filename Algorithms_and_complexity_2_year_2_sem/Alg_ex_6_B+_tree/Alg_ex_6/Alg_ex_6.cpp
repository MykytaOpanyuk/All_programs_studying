// Alg_ex_6.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "market.h"

using namespace std;
#define NMAX 2 //num of keys in node 
#define INT_MAX 1000000

struct Bp_tree_node {
	vector<product_element *> data;
	Bp_tree_node *parent_block;
	vector<Bp_tree_node *> child_blocks; // array of child
	bool is_leaf;
	int n; // num of element
};


class B_plus_tree {

public:
	Bp_tree_node * root;
	Bp_tree_node *new_p;
	int num_to_split;
	product_element *null_node;

	B_plus_tree();
	~B_plus_tree();
	Bp_tree_node *new_node();
	Bp_tree_node *find_data(string name, Bp_tree_node *cur_element, int &pos_of_data, int &pos_of_cur);
	void shift_to_left(Bp_tree_node *cur_element, int pos);
	void shift_to_right(Bp_tree_node *cur_element, int pos);
	void insert(Bp_tree_node *cur_element, product_element *data);
	void merge_blocks(Bp_tree_node *left_element, Bp_tree_node *right_element, int pos_to_merge);
	bool delete_data(string data);
	void split_leaf(Bp_tree_node *cur_element);
	void split_non_leaf(Bp_tree_node *cur_element);
	void redistribution_left(Bp_tree_node * left_element, Bp_tree_node *right_element, int pos_of_left);
	void redistribution_right(Bp_tree_node * left_element, Bp_tree_node *right_element, int pos_of_left);
	void cout_tree(Bp_tree_node *cur_element);
};

B_plus_tree::B_plus_tree()
{
	int n;
	int check;

	null_node = new product_element;
	null_node->group = "NULL";
	null_node->name = "NoName is mean no name. It is must be long string...";
	null_node->price = 0;
	null_node->key = INT_MAX;

	root = new_node(); //initialize a root of tree

	num_to_split = NMAX;

	for (int i = 0; i < (int)main_data->size_of_f; i++) { //read data from file and insert it in tree
		insert(root, main_data->array_of_products[i]);
	}
	cout << "B+ tree is ready to use!" << endl;
	cout_tree(root);
	do {
		cout << endl;
		cout << "Chose what to do :" << endl;
		cout << "1. Add new product." << endl;
		cout << "2. Delete product from tree." << endl;
		cout << "3. Cout tree." << endl;
		cout << "4. Check product in tree." << endl;
		cout << "5. Exit!" << endl;
		cin >> check;
		if (check == 1) {
			product_element *new_element = main_data->new_data();
			insert(root, new_element);
		}
		if (check == 2) {
			string to_delete = main_data->delete_data();
			delete_data(to_delete);
		}
		if (check == 3)
			cout_tree(root);
		if (check == 4) {
			string name_of_product;
			cout << "Write a name of product :" << endl;
			cin >> name_of_product;
			int pos_data, pos_cur;
			if (find_data(name_of_product, root, pos_data, pos_cur))
				cout << "Yes, in data " << pos_data << ", position in data is : " << pos_cur << endl;
		}
		if (check == 5)
			main_data->save_changing();
	} while (check != 5);
}

B_plus_tree::~B_plus_tree()
{

}

Bp_tree_node *B_plus_tree::new_node()
{
	new_p = new Bp_tree_node;

	for (int i = 0; i <= NMAX; i++)
		new_p->data.push_back(null_node);
	new_p->is_leaf = true;
	new_p->n = 0;
	new_p->parent_block = NULL;
	for (int i = 0; i <= NMAX + 1; i++)
		new_p->child_blocks.push_back(NULL);

	return new_p;
}

Bp_tree_node *B_plus_tree::find_data(string data, Bp_tree_node *cur_element, int &pos_of_data, int &pos_of_cur)
{
	if (cur_element->is_leaf == true)
		for (int i = 0; i < cur_element->n; i++)
			if (cur_element->data[i]->name == data) {
				pos_of_data = i;
				return cur_element;
			}

	if (cur_element->is_leaf == false)
		for (int i = 0; i <= cur_element->n; i++)
			if (cur_element->child_blocks[i] != NULL) {
				pos_of_cur = i;
				cur_element = find_data(data, cur_element->child_blocks[i], pos_of_data, pos_of_cur);
				if (pos_of_data != INT_MAX)
					break;
			}
	if (pos_of_data != INT_MAX)
		return cur_element;
	else if (cur_element->parent_block != NULL)
		return cur_element->parent_block;
}

void B_plus_tree::shift_to_left(Bp_tree_node *cur_element, int pos)
{
	for (int i = pos; i <= cur_element->n; i++)
		cur_element->data[i - 1] = cur_element->data[i];

	cur_element->data[cur_element->n + 1] = null_node;

	if (!cur_element->is_leaf) {
		for (int i = pos; i <= cur_element->n + 1; i++)
			cur_element->child_blocks[i - 1] = cur_element->child_blocks[i];

		cur_element->child_blocks[cur_element->n + 1] = NULL;

		for (int i = 0; cur_element->child_blocks[i] != NULL; i++)
			cur_element->child_blocks[i]->parent_block = cur_element;
	}
	cur_element->n--;

}

void B_plus_tree::shift_to_right(Bp_tree_node *cur_element, int pos)
{
	cur_element->data[pos] = cur_element->data[pos - 1];

	for (int i = (cur_element->n - 1); i >= pos; i--)
		cur_element->data[i] = cur_element->data[i - 1];

	//cur_element->data[cur_element->n] = INT_MAX;
	if (!cur_element->is_leaf) {
		cur_element->child_blocks.push_back(cur_element->child_blocks[cur_element->n + 1]);

		for (int i = cur_element->n + 1; i >= pos; i--)
			cur_element->child_blocks[i] = cur_element->child_blocks[i - 1];

		//cur_element->child_blocks[cur_element->n + 1] = NULL;

		for (int i = 0; i <= cur_element->n + 1; i++)
			cur_element->child_blocks[i]->parent_block = cur_element;
	}
	cur_element->n++;
}

void B_plus_tree::insert(Bp_tree_node *cur_element, product_element *data)
{
	// for the first time all elements are INT_MAX
	for (int i = 0; i <= cur_element->n; i++) {
		if ((data->key < cur_element->data[i]->key) && (cur_element->child_blocks[i] != NULL)) {
			insert(cur_element->child_blocks[i], data);
			if (cur_element->n == (NMAX))
				split_non_leaf(cur_element);
			return;
		}
		else if ((data->key < cur_element->data[i]->key) && (cur_element->child_blocks[i] == NULL)) {
			swap(cur_element->data[i], data); // change ne value in block, save old value from block
			if (cur_element->n == i) {
				cur_element->n++;
				break;
			}
		}
	}

	if (cur_element->n == (2*NMAX - 1)) {
		split_leaf(cur_element);
		//cur_element->is_leaf = false;
	}
}

void B_plus_tree::merge_blocks(Bp_tree_node *left_element, Bp_tree_node *right_element, int pos_of_right)
{
	int j = 0, i;

	left_element->data[left_element->n] = left_element->parent_block->data[pos_of_right - 1];
	left_element->n++;

	i = left_element->n;

	while (j != right_element->n + 1) {
		if (left_element->data[left_element->n - 1] != right_element->data[j]) {
			left_element->data[i] = right_element->data[j];
		}
		else
			left_element->n--;
		left_element->child_blocks[i] = right_element->child_blocks[j];
		i++;
		j++;
	}

	left_element->n = left_element->n + right_element->n;
	j = pos_of_right;

	while (j != (left_element->parent_block->n + 1)) {
		left_element->parent_block->data[j - 1] = left_element->parent_block->data[j];
		left_element->parent_block->child_blocks[j] = left_element->parent_block->child_blocks[j + 1];
		j++;
	}

	left_element->parent_block->n--;

	j = 0;
	while (left_element->child_blocks[j] != NULL) {
		if (j <= left_element->n) {
			left_element->child_blocks[j]->parent_block = left_element;
			j++;
		}
		else
			break;
	}
}

bool B_plus_tree::delete_data(string data)
{
	int pos_of_data = INT_MAX, pos_of_cur;
	Bp_tree_node *cur_element = find_data(data, root, pos_of_data, pos_of_cur);

	if (cur_element != NULL) {
		if (cur_element->child_blocks[pos_of_data] == NULL) {
			shift_to_left(cur_element, (pos_of_data + 1));
			//return true;
		}

		if ((cur_element->is_leaf == true) && (cur_element->parent_block != NULL)) {
			if (pos_of_cur == 0) {
				Bp_tree_node *right_element = cur_element->parent_block->child_blocks[1];

				if ((right_element != NULL) && (right_element->n > num_to_split))
					redistribution_left(cur_element, right_element, 0);

				else if ((right_element != NULL) && ((cur_element->n + right_element->n) < NMAX))
					merge_blocks(cur_element, right_element, 1);
			}
			else {
				Bp_tree_node *left_element = cur_element->parent_block->child_blocks[pos_of_cur - 1];
				Bp_tree_node *right_element = cur_element->parent_block->child_blocks[pos_of_cur + 1];

				if ((left_element != NULL) && (left_element->n > num_to_split))
					redistribution_right(left_element, cur_element, (pos_of_cur - 1));

				else if ((right_element != NULL) && (right_element->n > num_to_split))
					redistribution_left(cur_element, right_element, pos_of_cur);

				else if ((left_element != NULL) && ((left_element->n + cur_element->n) < NMAX))
					merge_blocks(left_element, cur_element, pos_of_cur);

				else if ((right_element != NULL) && ((cur_element->n + right_element->n) < NMAX))
					merge_blocks(cur_element, right_element, (pos_of_cur + 1));
			}
		}
		else if ((cur_element->is_leaf == false) && (cur_element->parent_block != NULL)) {
			if (pos_of_cur == 0) {
				Bp_tree_node *right_element = cur_element->parent_block->child_blocks[1];

				if ((right_element != NULL) && ((right_element->n) > num_to_split))
					redistribution_left(cur_element, right_element, 0);

				else if ((right_element != NULL) && ((right_element->n + cur_element->n) < NMAX))
					merge_blocks(cur_element, right_element, 1);
			}
			else {
				Bp_tree_node *left_element = cur_element->parent_block->child_blocks[pos_of_cur - 1];
				Bp_tree_node *right_element = cur_element->parent_block->child_blocks[pos_of_cur + 1];

				if ((left_element != NULL) && (left_element->n > num_to_split))
					redistribution_right(left_element, cur_element, (pos_of_cur - 1));

				else if ((right_element != NULL) && (right_element->n > num_to_split))
					redistribution_left(cur_element, right_element, pos_of_cur);

				else if ((left_element != NULL) && ((left_element->n + cur_element->n) < NMAX))
					merge_blocks(left_element, cur_element, pos_of_cur);

				else if ((right_element != NULL) && ((cur_element->n + right_element->n) < NMAX))
					merge_blocks(cur_element, right_element, (pos_of_cur + 1));
			}
		}

		Bp_tree_node *timetable = cur_element->parent_block;

		while (timetable != NULL) {
			for (int i = 0; i < timetable->n; i++)
				if (timetable->data[i]->name == data) {
					timetable->data[i] = cur_element->data[0];
					break;
				}
			timetable = timetable->parent_block;
		}

		return true;
		//if ((cur_element->parent_block == NULL) && (cur_element->child_blocks[0] == NULL))


	}
	else {
		cout << "There isn't any product like this." << endl;
		return false;
	}
}

void B_plus_tree::split_leaf(Bp_tree_node *cur_element)
{
	int j = 0, i = num_to_split; //middle of data array
	Bp_tree_node *right_element = new_node();

	cur_element->n = num_to_split; // it will be left blocks
	right_element->n = (2 * NMAX - 1) - num_to_split;

	while (i < (2*NMAX - 1)) {
		right_element->data[j] = cur_element->data[i]; // copy element
		cur_element->data[i] = null_node;
		j++;
		i++;
	}

	cur_element->child_blocks[(2 * NMAX)] = right_element;

	product_element *data = right_element->data[0];
	//shift_to_left(right_element, 1);

	//if the leaf itself is a parent then
	if (cur_element->parent_block == NULL) {
		cur_element->parent_block = new_node();

		cur_element->parent_block->is_leaf = false;
		cur_element->parent_block->n = 1;
		cur_element->parent_block->data[0] = data;
		cur_element->parent_block->child_blocks[0] = cur_element;
		cur_element->parent_block->child_blocks[1] = right_element;
		right_element->parent_block = cur_element->parent_block;
		root = cur_element->parent_block;
		return;
	}
	else {   //if the splitted leaf block is not rootBlock then

		cur_element = cur_element->parent_block;

		for (int k = 0; k <= cur_element->n; k++)
			if (data->key < cur_element->data[k]->key)
				swap(cur_element->data[k], data);

		cur_element->n++;

		for (int k = 0; k < cur_element->n; k++)
			if (right_element->data[0]->key < cur_element->child_blocks[k]->data[0]->key)
				swap(cur_element->child_blocks[k], right_element);

		cur_element->child_blocks[cur_element->n] = right_element;

		j = 0;

		while (j <= cur_element->n) {
			cur_element->child_blocks[j]->parent_block = cur_element;
			j++;
		}
	}
}

void B_plus_tree::split_non_leaf(Bp_tree_node *cur_element)
{
	int i = num_to_split, j = 0;
	Bp_tree_node *right_element = new_node();

	cur_element->n = num_to_split;
	right_element->n = (2 * NMAX - 1) - num_to_split;
	right_element->is_leaf = false;

	while (i < (2 * NMAX - 1)) {
		right_element->data[j] = cur_element->data[i];
		right_element->child_blocks[j] = cur_element->child_blocks[i];
		cur_element->data[i] = null_node;
		if (i != num_to_split)
			cur_element->child_blocks[i] = NULL;
		i++;
		j++;
	}

	cur_element->child_blocks[(2 * NMAX)] = right_element;

	product_element *data = right_element->data[0];

	shift_to_left(right_element, 1); // to have no repeat of the first element from array of data

	if (cur_element->parent_block == NULL) {
		cur_element->parent_block = new_node();

		cur_element->parent_block->is_leaf = false;
		cur_element->parent_block->n = 1;
		cur_element->parent_block->data[0] = data;
		cur_element->parent_block->child_blocks[0] = cur_element;
		cur_element->parent_block->child_blocks[1] = right_element;
		right_element->parent_block = cur_element->parent_block;
		root = cur_element->parent_block;
		return;
	}
	else {
		cur_element = cur_element->parent_block;

		Bp_tree_node *new_child = new_node();
		new_child = right_element;

		for (int k = 0; k <= cur_element->n; k++)
			if (data->key < cur_element->data[k]->key)
				swap(cur_element->data[k], data);

		cur_element->n++;

		for (int k = 0; k < cur_element->n; k++)
			if (new_child->data[0]->key < cur_element->child_blocks[k]->data[0]->key)
				swap(cur_element->child_blocks[k], new_child);

		cur_element->child_blocks[cur_element->n] = new_child;

		j = 0;
		while (cur_element->child_blocks[j] != NULL) {
			if (j <= cur_element->n) {
				cur_element->child_blocks[j]->parent_block = cur_element;
				j++;
			}
			else
				break;
		}
	}
}

void B_plus_tree::redistribution_left(Bp_tree_node *left_element, Bp_tree_node *right_element, int pos_of_left)
{
	//int right_data = right_element->data[0];

	if ((left_element->is_leaf == false) && (right_element->is_leaf == false)) {
		left_element->data[left_element->n] = left_element->parent_block->data[pos_of_left];
		left_element->child_blocks[NMAX] = right_element->child_blocks[0];
		left_element->n++;
		left_element->parent_block->data[pos_of_left] = right_element->data[0];
		shift_to_left(right_element, 1);
	}
	else {
		left_element->data[left_element->n] = right_element->data[0];
		left_element->n++;
		shift_to_left(right_element, 1);
		left_element->parent_block->data[pos_of_left] = right_element->data[0];
	}

}

void B_plus_tree::redistribution_right(Bp_tree_node * left_element, Bp_tree_node *right_element, int pos_of_left)
{
	//int right_data = right_element->data[0];

	if ((left_element->is_leaf == false) && (right_element->is_leaf == false)) {
		shift_to_right(right_element, 1);
		right_element->data[0] = left_element->parent_block->data[pos_of_left];
		right_element->child_blocks[0] = left_element->child_blocks[left_element->n];
		left_element->parent_block->data[pos_of_left] = left_element->data[left_element->n - 1];
		left_element->data[left_element->n - 1] = null_node;
		left_element->child_blocks[left_element->n] = NULL;
		left_element->n--;
	}
	else {
		shift_to_right(right_element, 1);
		right_element->data[0] = left_element->data[left_element->n - 1];
		left_element->data[left_element->n - 1] = null_node;
		left_element->n--;
		left_element->parent_block->data[pos_of_left] = right_element->data[0];
	}
}

void B_plus_tree::cout_tree(Bp_tree_node *cur_element)
{
	if (cur_element == NULL)
		return;
	for (int i = 0; i < cur_element->n; i++)
		cout << " " << cur_element->data[i]->name;

	bool check = false;

	for (int i = 0; i <= cur_element->n; i++)
		if (cur_element->child_blocks[i] != NULL)
			check = true;
	if (check == true)
		cout << endl;

	for (int i = 0; i <= cur_element->n; i++) {
		cout_tree(cur_element->child_blocks[i]);
	}
}

int main() {
	main_data = new market;
	B_plus_tree new_tree;
	//new_tree.cout_tree(new_tree.root);
	system("pause");
	return 0;
}


