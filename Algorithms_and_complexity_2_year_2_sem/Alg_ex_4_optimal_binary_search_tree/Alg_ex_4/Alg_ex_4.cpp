// Alg_ex_4.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "market.h"
#include <iomanip>

using namespace std;

#define INT_MAX 1000000

class optimal_product_tree {

public:
	vector<vector<double> > e;
	vector<vector<double> > w;
	vector<vector<int> > root;
	product_element *tree_root;

	void sort_type(vector<product_element *> &array_to_sort)
	{
		int i, j;
		for (i = 1; i < (int)array_to_sort.size(); i++)
			// Last i elements are already in place   
			for (j = 1; j < (int)array_to_sort.size() - i; j++)
				if (array_to_sort[j]->name > array_to_sort[j + 1]->name) {
					product_element *timetable = array_to_sort[j];
					array_to_sort[j] = array_to_sort[j + 1];
					array_to_sort[j + 1] = timetable;
				}
	}

	optimal_product_tree()
	{
		e.resize(main_data->size_of_f + 1);
		for (int i = 0; i < main_data->size_of_f + 1; i++)
			e[i].resize(main_data->size_of_f + 1);

		w.resize(main_data->size_of_f + 1);
		for (int i = 0; i < main_data->size_of_f + 1; i++)
			w[i].resize(main_data->size_of_f + 1);

		root.resize(main_data->size_of_f + 1);
		for (int i = 0; i < main_data->size_of_f + 1; i++)
			root[i].resize(main_data->size_of_f + 1);

		for (int i = 0; i < main_data->size_of_f + 1; i++)
			for (int j = 0; j < main_data->size_of_f + 1; j++) {
				e[i][j] = 0;
				w[i][j] = 0;
				root[i][j] = 0;
			}

		sort_type(main_data->main_array_of_products);

		build_optimal_search_tree(main_data->size_of_f);
		cout_matrix();
	}

	~optimal_product_tree()
	{
		w.clear();
		e.clear();
		root.clear();
	}

	void build_optimal_search_tree(int n)
	{
		for (int i = 1; i <= n; i++) {
			e[i][i - 1] = main_data->main_array_of_products[i - 1]->fact_probability;
			w[i][i - 1] = main_data->main_array_of_products[i - 1]->fact_probability;

		}
		for (int l = 1; l <= n; l++)
			for (int i = 1; i <= (n - l + 1); i++) {
				int j = i + l - 1;
				e[i][j] = INT_MAX;
				w[i][j] = w[i][j - 1] + main_data->main_array_of_products[j]->fact_probability
					+ main_data->main_array_of_products[j]->probability;
				// Try making all keys in interval keys[i..j] as root
				for (int r = i; r <= j; r++)
				{
					// c = cost when keys[r] becomes root of this subtree
					double c = w[i][j];
					if (r > i)
						c = c + e[i][r - 1];
					if (r < j)
						c = c + e[r + 1][j];
					if (c < e[i][j]) {
						e[i][j] = c;
						root[i][j] = r;
					}
				}
			}

	}

	void cout_matrix()
	{
		for (int i = 0; i < main_data->size_of_f + 1; i++) {
			for (int j = 0; j < main_data->size_of_f + 1; j++)
				cout << e[i][j] << " ";
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < main_data->size_of_f + 1; i++) {
			for (int j = 0; j < main_data->size_of_f + 1; j++)
				cout << root[i][j] << " ";
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < main_data->size_of_f + 1; i++) {
			for (int j = 0; j < main_data->size_of_f + 1; j++)
				cout << w[i][j] << " ";
			cout << endl;
		}
		tree_root = printOBST(1, main_data->size_of_f, tree_root);
		postorder(tree_root, 0);
		cout << "It is done!" << endl;
	}

	void postorder(product_element* p, int indent = 0)
	{
		cout << "(";

		if (p->left != NULL) {
			postorder(p->left);
			cout << ") ";
		}

		cout << p->name;

		if (p->right != NULL) {
			cout << " (";
			postorder(p->right);

		}

		cout << ")";
	}

	product_element *printOBST(int i, int j, product_element *elem)
	{
		if (i < j) {
			elem = main_data->main_array_of_products[root[i][j]];
			elem->left = printOBST(i, root[i][j] - 1, elem->left);
			elem->right = printOBST(root[i][j] + 1, j, elem->right);
		}
		else if (i == j) {
			elem = main_data->main_array_of_products[root[i][j]];
			elem->left = NULL;
			elem->right = NULL;
		}
		else elem = NULL;
		return elem;

	}
};

int main()
{
	main_data = new market;
	optimal_product_tree new_tree;
	system("pause");
	return 0;
}


