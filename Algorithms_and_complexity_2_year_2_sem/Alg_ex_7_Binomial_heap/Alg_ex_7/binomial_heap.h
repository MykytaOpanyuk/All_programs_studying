#pragma once

#include <string>
#include <vector>
#include "market.h"

#define SMALL_NUM -9999
#define INT_MAX 9999

class Node {
public:
	int key;
	string name;
	string group;
	double price;
	unsigned int degree;
	Node *parent, *child, *sibling;
	bool inf;

	Node(product_element *val);
	~Node();
	void Swap(Node &node);
};

Node::Node(product_element *val)
{
	this->name = val->name;
	this->group = val->group;
	this->price = val->price;
	this->key = val->key;
	degree = 0;
	parent = child = sibling = NULL;
	inf = false;
}

Node::~Node() {

}

void Node::Swap(Node &node) {
	int t_key = node.key;
	bool t_inf = node.inf;
	string t_name = node.name;
	string t_group = node.group;
	double t_price = node.price;

	node.key = key;
	node.name = name;
	node.group = group;
	node.price = price;
	node.inf = inf;

	key = t_key;
	inf = t_inf;
	name = t_name;
	group = t_group;
	price = t_price;
}

class Binomial_heap {
public:
	Binomial_heap();
	Binomial_heap(Binomial_heap& orig);
	~Binomial_heap();

	// Adds a new key to binomilal heap. Returns a pointer on new node which needs to be saved for acces to this key
	Node *add_key(product_element *key);

	// Returns a pointer of minimal key in heap
	Node *min_key();

	// Extracts a node with minimal key from heap. Extracted node needs to be deleted separately
	Node *extract_min_key();

	// Decreases 'x' node`s key by replacing with a less key. Throws an exception if 'newKey' > 'x->key'
	void decrease_key(Node *x, int new_key);

	// Deletes 'x' node from heap
	void remove_key(Node *x);

	void operator=(Binomial_heap &&b);
private:
	// A list of binomial trees
	Node * head;

	// Extracts a node marked as infinity (used in 'RemoveKey' method)
	Node *extract_infinity_key();

	// Marks 'x' node as infinity (needs for 'RemoveKey' method)
	void mark_key(Node *x);

	// Unites two binomial trees T1 (Bk-1) and T2 (Bk-1): T1 <- T2 (Bk). T1 is a pointer on result of union
	static void unite_binom_trees(Node *T1, Node *T2);

	// Clears up a binomial tree with 'T' root
	static void clear_binom_tree(Node *T);

	// Copies a binomial tree with 'T' root
	static Node *copy_binom_tree(Node *T);

	// Merges two lists of roots of binomial trees
	friend Node *merge_heaps(Binomial_heap &H1, Binomial_heap &H2);

	friend void Heaps_editing(Binomial_heap &h);
	// Unites two heaps
	friend Binomial_heap unite_binom_heaps(Binomial_heap &, Binomial_heap &);
};

// Unites two heaps
Binomial_heap unite_binom_heaps(Binomial_heap &H1, Binomial_heap &H2);

Binomial_heap::Binomial_heap()
{
	head = NULL;
}

Binomial_heap::Binomial_heap(Binomial_heap & orig)
{
	head = orig.head;
	orig.head = NULL;
}

Binomial_heap::~Binomial_heap()
{
	while (head) {
		Node *tmp = head;
		head = head->sibling;
		clear_binom_tree(tmp);
	}
}

Node *Binomial_heap::add_key(product_element *key)
{
	Binomial_heap temp;
	Node *res = temp.head = new Node(key);
	temp = unite_binom_heaps(*this, temp);
	head = temp.head;
	temp.head = NULL;
	return res;
}

Node *Binomial_heap::min_key()
{
	Node *min = head, *curs = head;
	while (curs) {
		if (curs->key < min->key) min = curs;
		curs = curs->sibling;
	}
	return min;
}

Node *Binomial_heap::extract_min_key()
{
	if (head) {
		Node *min = head, *pmin = NULL, *curs = head->sibling, *pcurs = head;
		while (curs) {
			if (curs->key < min->key) {
				pmin = pcurs;
				min = curs;
			}
			pcurs = curs;
			curs = curs->sibling;
		}

		if (pmin) {
			// If minimum is not a head...
			pmin->sibling = min->sibling;
			Binomial_heap tmp;
			curs = min->child;

			// Splitting up a tree with minimal root
			while (curs) {
				curs->parent = NULL;
				Node *ncurs = curs->sibling;
				curs->sibling = tmp.head;
				tmp.head = curs;
				curs = ncurs;
			}

			// Reunion of heap
			tmp = unite_binom_heaps(*this, tmp);
			head = tmp.head;
			tmp.head = NULL;
		}
		else {
			// ...else
			head = min->sibling;
			Binomial_heap tmp;
			curs = min->child;

			// Splitting up a tree with minimal root
			while (curs) {
				curs->parent = NULL;
				Node *ncurs = curs->sibling;
				curs->sibling = tmp.head;
				tmp.head = curs;
				curs = ncurs;
			}

			// Reunion of heap
			tmp = unite_binom_heaps(*this, tmp);
			head = tmp.head;
			tmp.head = NULL;
		}
		min->child = min->sibling = NULL;
		return min;
	}
	else return NULL;
}

void Binomial_heap::decrease_key(Node *x, int new_key)
{
	if (new_key > x->key) {
		cout << "New key is bigger than current." << endl;
		return;
	}
	x->key = new_key;
	Node *y = x->parent;

	while (y && x->key < y->key) {
		y->Swap(*x);
		x = y;
		y = y->parent;
	}
}

void Binomial_heap::remove_key(Node *x)
{
	mark_key(x);
	Node *tmp = extract_infinity_key();
	if (tmp) delete tmp;
}

void Binomial_heap::operator=(Binomial_heap &&b)
{
	head = b.head;
	b.head = NULL;
}

Node * Binomial_heap::extract_infinity_key()
{
	if (head) {
		Node *min = head, *pmin = NULL, *curs = head->sibling, *pcurs = head;

		while (curs) {
			if (curs->inf) {
				pmin = pcurs;
				min = curs;
				break;
			}
			pcurs = curs;
			curs = curs->sibling;
		}

		if (pmin) {
			// If minimum is not a head...
			pmin->sibling = min->sibling;
			Binomial_heap tmp;
			curs = min->child;

			// Splitting up a tree with minimal root
			while (curs) {
				curs->parent = NULL;
				Node *ncurs = curs->sibling;
				curs->sibling = tmp.head;
				tmp.head = curs;
				curs = ncurs;
			}

			// Reunion of heap
			tmp = unite_binom_heaps(*this, tmp);
			head = tmp.head;
			tmp.head = NULL;
		}
		else {
			// ...else
			head = min->sibling;
			Binomial_heap tmp;
			curs = min->child;

			// Splitting up a tree with minimal root
			while (curs) {
				curs->parent = NULL;
				Node *ncurs = curs->sibling;
				curs->sibling = tmp.head;
				tmp.head = curs;
				curs = ncurs;
			}

			// Reunion of heap
			tmp = unite_binom_heaps(*this, tmp);
			head = tmp.head;
			tmp.head = NULL;
		}
		min->child = min->sibling = NULL;
		return min;
	}
	else 
		return NULL;
}

void Binomial_heap::mark_key(Node *x)
{
	x->inf = true;
	Node *y = x->parent;

	while (y) {
		y->Swap(*x);
		x = y;
		y = y->parent;
	}
}

void Binomial_heap::unite_binom_trees(Node *T1, Node *T2)
{
	T2->parent = T1;
	T2->sibling = T1->child;
	T1->child = T2;
	T1->degree++;
}

void Binomial_heap::clear_binom_tree(Node * T)
{
	if (T->child)
		clear_binom_tree(T->child);
	if (T->sibling)
		clear_binom_tree(T->sibling);
	delete T;
}

Node * Binomial_heap::copy_binom_tree(Node * T)
{
	if (T) {
		Node *newT = new Node(*T);

		newT->child = copy_binom_tree(T->child);

		if (newT->child)
			newT->child->parent = newT;

		newT->sibling = copy_binom_tree(T->sibling);

		if (newT->sibling)
			newT->sibling->parent = newT->parent;
		return newT;
	}
	return NULL;
}

Node * merge_heaps(Binomial_heap &H1, Binomial_heap &H2)
{
	Node *result;

	if ((H1.head) && (H2.head)) {
		Node *curs;

		if (H1.head->degree <= H2.head->degree) {
			result = curs = H1.head;
			H1.head = H1.head->sibling;
		}
		else {
			result = curs = H2.head;
			H2.head = H2.head->sibling;
		}
		while (H1.head && H2.head) {
			if (H1.head->degree <= H2.head->degree) {
				curs->sibling = H1.head;
				curs = curs->sibling;
				H1.head = H1.head->sibling;
			}
			else {
				curs->sibling = H2.head;
				curs = curs->sibling;
				H2.head = H2.head->sibling;
			}
		}

		if (H1.head)
			curs->sibling = H1.head;
		else
			curs->sibling = H2.head;
	}
	else if (H1.head)
		result = H1.head;
	else
		result = H2.head;

	H1.head = NULL;
	H2.head = NULL;

	return result;
}

void Heaps_editing(Binomial_heap &h)
{
	for (unsigned int i = 0; i < main_data->array_of_products.size(); i++)
		h.add_key(main_data->array_of_products[i]);

	cout << "Nodes from files are ready." << endl;
	
	vector<Node *> dataVault;

	for (unsigned int i = 0; i < main_data->array_of_products.size(); i++) {
		Node *new_element = new Node(main_data->array_of_products[i]);
		dataVault.push_back(new_element);
	}

	int item;
	do {
		for (unsigned int i = 0; i < main_data->array_of_products.size(); i++)
			cout << main_data->array_of_products[i]->name << " ";
		cout << endl;



		cout << "1. Add a new product." << endl;
		cout << "2. Remove a key." << endl;
		cout << "3. Get a minimal key." << endl;
		cout << "4. Extract a minimal key." << endl;
		cout << "5. Decrease key." << endl;
		cout << "6. Exit." << endl;
		cin >> item;

		Node *ptr = NULL;
		if (item == 1) {
			product_element *new_element = new product_element;

			cout << "Enter a name: ";
			cin.get();
			getline(cin, new_element->name);
			if (main_data->check_is_product(new_element->name) != -1) {
				delete new_element;
				cout << "This product is in heap." << endl;
				continue;
			}
			cout << "Enter a group: ";
			cin.get();
			getline(cin, new_element->group);
			cout << "Enter a price: ";
			cin >> new_element->price;
			new_element->key = string_to_key(new_element->name);
			dataVault.push_back(h.add_key(new_element));
			
			main_data->array_of_products.push_back(new_element);
		}
		if (item == 2) {
			string value;
			bool founded;

			cout << "Enter a key for deleting: ";
			cin >> value;

			founded = false;

			for (unsigned int i = 0; i < dataVault.size(); i++)
				if (dataVault[i]->name == value) {
					ptr = dataVault[i];
					founded = true;
					break;
				}
			if (founded) {
				h.remove_key(ptr);

				// ... and from 'dataVault'
				for (size_t i = 0; i < dataVault.size(); i++)
					if (dataVault[i] == ptr) {
						dataVault.erase(dataVault.begin() + i);
						break;
					}
				main_data->delete_data(value);
			}
			else 
				cout << "Key not found\n";
		}
		if (item == 3) {
			ptr = h.min_key();
			if (ptr) cout << "Minimal key: " << ptr->key << endl;
			else cout << "Heap is empty\n";
		}
		if (item == 4) {
			ptr = h.extract_min_key();
			if (ptr) {
				cout << "Minimal key '" << ptr->key << "' has been deleted...\n";

				// Searcing in 'dataVault'...
				for (size_t i = 0; i < dataVault.size(); i++)
					if (dataVault[i] == ptr) {
						// ... and deleting from it
						dataVault.erase(dataVault.begin() + i);
						break;
					}

				// Clearing up memory
				delete ptr;
			}
			else cout << "Heap is empty\n";
		}
		if (item == 5) {
			string value;

			cout << "Enter a key for decreasing: ";
			cout << "Enter a name: ";
			cin.get();
			getline(cin, value);

			bool founded = false;
			for (unsigned int i = 0; i < dataVault.size(); i++)
				if (dataVault[i]->name == value) {
					ptr = dataVault[i];
					founded = true;
					break;
					cout << dataVault[i]->key;
				}
			if (founded) {
				int new_key;
				
				cout << "Enter a new key less than : " << ptr->key << endl;
				cin >> new_key;

				h.decrease_key(ptr, new_key);
			}
			else
				cout << "Product not found.";
		}
	} while (item != 6);
	main_data->save_changing();
}

Binomial_heap unite_binom_heaps(Binomial_heap &H1, Binomial_heap &H2)
{
	Binomial_heap Union;
	Union.head = merge_heaps(H1, H2);
	if (Union.head) {
		Node *pcurs = NULL, *curs = Union.head, *ncurs = curs->sibling;

		while (ncurs) {
			if ((curs->degree != ncurs->degree) || (ncurs->sibling && (ncurs->sibling->degree == curs->degree))) {
				pcurs = curs;
				curs = ncurs;
			}
			else {
				if (curs->key <= ncurs->key) {
					curs->sibling = ncurs->sibling;
					Binomial_heap::unite_binom_trees(curs, ncurs);
				}
				else {
					if (pcurs) pcurs->sibling = ncurs;
					else Union.head = ncurs;
					Binomial_heap::unite_binom_trees(ncurs, curs);
					curs = ncurs;
				}
			}
			ncurs = curs->sibling;
		}
	}
	return Union;
}