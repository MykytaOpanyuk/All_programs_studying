#pragma once
#include "Algorithm.h"

extern const double fi;



// Three possible states of vertex by Dijkstra

enum State {
	LABELED, UNLABELED, SCANNED
};

struct FH {								// Fibonacci Heap

	struct RTN {					// Rooted Tree Node
		Graph::Vertex *key;			// vertex pointer stored within node
		RTN *p,						// parent pointer
			*child,					// first child pointer
			*left,					// left neighbour pointer
			*right;					// right
		unsigned int degree;		// contains number of children
		bool mark;					// logic variable
		State state;				// current state of vertex by Dijkstra

		RTN(Graph::Vertex *k) : key(k) {}
	};
	RTN *min;						// node with minimum key within heap
	unsigned int n;						// total number of heap's nodes

// Allocates memory for empty heap

	FH *Make()
	{
		FH *H = new FH;
		H->n = 0;
		H->min = NULL;
		return H;
	}

	// Inserts node with specific key
	void Insert(FH *&H, RTN *x)
	{

		x->degree = 0;
		x->p = x->child = NULL;
		x->mark = false;

		if (!H->min) {					// heap is empty
			x->left = x->right = x;
			H->min = x;
		}
		else {							// otherwise node becomes left child of minimum node
			RTN *temp = H->min->left;
			H->min->left = x;
			x->right = H->min;
			x->left = temp;
			temp->right = x;

			if (x->key->d < H->min->key->d)	// reorganises minimum node if needed
				H->min = x;
		}
		H->n++;
	}

	// Connects two nodes
	void Link(FH *&H, RTN *y, RTN *x)
	{
		y->left->right = y->right;
		y->right->left = y->left;
		y->left = y->right = NULL;
		y->p = x;

		// Transfers children
		if (!x->child) {
			x->child = y;
			y->left = y->right = y;
		}
		else {
			RTN *temp = x->child->left;

			x->child->left = y;
			y->right = x->child;
			y->left = temp;
			temp->right = y;
		}

		x->degree += 1;
		y->mark = false;
	}

	// Makes heap's roots denser

	void Consolidate(FH *&H)
	{
		int D = log(H->n) / log(fi);		// maximum degree of any heap's node
		vector <RTN *> A(D);			// helper vector

		RTN *w = H->min;

		do {
			RTN* x = w;					// current node
			unsigned int d = x->degree;		// its dergree

			while (A[d]) {					// if vector contains node with same degree
				RTN* y = A[d];			// points it out

				if (x->key->d > y->key->d)	// determines larger key
					swap(x, y);
				else if (y == H->min)		// avoids minimum node being someone's child
					H->min = x;

				w = x;						// same for current node pointer

				Link(H, y, x);
				A[d] = NULL;
				d++;
			}
			A[d] = x;
			w = w->right;
		} while (w != H->min);				// stops cycle

		H->min = NULL;

		// Copies data from vector to new list
		for (unsigned int i = 0; i < D; i++) {
			if (A[i])
				if (!H->min) {
					H->min = A[i];
					H->min->left = H->min->right = H->min;
				}
				else {
					RTN *temp = H->min->left;
					H->min->left = A[i];
					A[i]->right = H->min;
					A[i]->left = temp;
					temp->right = A[i];

					if (A[i]->key->d < H->min->key->d)
						H->min = A[i];
				}
		}
	}

	// Deletes minimum node and returns it

	RTN *Extract_Min(FH *&H)
	{
		RTN *z = H->min;

		if (z) {							// list isn't empty
			if (z->child) {
				RTN *k = z->child;		// saves child pointer
				vector <RTN*> temp;

				do {						// all children are saved into vector
					temp.push_back(k);
					k = k->right;
				} while (k != z->child);

				for (unsigned int i = 0; i < temp.size(); i++) {	// other children become roots
					RTN *tmp = H->min->left;
					H->min->left = temp[i];
					temp[i]->right = H->min;
					temp[i]->left = tmp;
					temp[i]->p = NULL;
					tmp->right = temp[i];
				}
			}

			if (z == z->right)				// if minimum node is only node
				H->min = NULL;
			else {							// else deletes it
				z->left->right = z->right;
				z->right->left = z->left;
				H->min = z->right;
				Consolidate(H);
			}
			H->n--;
		}
		return z;
	}

	// Cuts node and makes it root

	void Cut(FH *&H, RTN *x, RTN *y)
	{
		// Stops being child

		if (x != x->right) {
			x->left->right = x->right;
			x->right->left = x->left;
			x->left = x->right = NULL;
		}
		else
			y->child = NULL;
		y->degree--;

		// This node becomes left neighbour of minimum node
		RTN *temp = H->min->left;

		H->min->left = x;
		x->right = H->min;
		x->left = temp;
		temp->right = x;

		if (x->key->d < H->min->key->d)
			H->min = x;

		x->p = NULL;
		x->mark = false;
	}

	// Recursively traverses on top until root or unmarked node
	void Cascading_Cut(FH *&H, RTN *y) {
		RTN *z = y->p;
		if (z) {
			if (y->mark == false)
				y->mark = true;
			else {
				Cut(H, y, z);
				Cascading_Cut(H, z);
			}
		}
	}


	void Decrease_Key(FH *&H, RTN *x, int k) {
		if (k > x->key->d)
			throw runtime_error("new key is greater than current");

		x->key->d = k;

		RTN *y = x->p;

		if (y && x->key->d < y->key->d) {
			Cut(H, x, y);
			Cascading_Cut(H, y);
		}

		if (x->key->d < H->min->key->d)
			H->min = x;
	}
};