// Alg_ex_9.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Algorithm.h"
#include "market.h"

int main()

try {
	main_data = new market;

	cout << "== Johnson algorithm ==" << endl;
	Graph G3(main_data->array_of_products.size() + 1);
	
	G3.gr.push_back(Graph::Vertex("test", 0, "test"));

	for (unsigned int i = 1; i <= main_data->array_of_products.size(); i++) {
		G3.gr.push_back(Graph::Vertex(main_data->array_of_products[i - 1]->name, i, main_data->array_of_products[i - 1]->group));
		cout << i << ". " << main_data->array_of_products[i - 1]->name << "; ";
	}
	cout << endl;
	
	for (int i = 1; i < G3.gr.size(); i++)
		G3.Adj[0].push_front(Graph::Edge(&G3.gr[i], 0));
	
	unsigned int i = 0;
	int add_edge = 1;
	while ((i < main_data->array_of_products.size()) && (add_edge == 1)) {
		cout << "Add new edge? 1 - Yes, 2 - no" << endl;
		cin >> add_edge;
		if (add_edge == 1) {
			int from, to, weight;
			cout << "From :" << endl;
			cin >> from;
			cout << "To :" << endl;
			cin >> to;
			cout << "Weight :" << endl;
			cin >> weight;
			G3.Adj[from].push_front(Graph::Edge(&G3.gr[to], weight));
		}
		if (add_edge == 2) {
			cout << "Use example? 1 - yes; 2 - no" << endl;
			cin >> add_edge;
			if (add_edge == 1) {
				add_edge = 2;

				G3.Adj[1].push_front(Graph::Edge(&G3.gr[2], -5));

				G3.Adj[1].push_front(Graph::Edge(&G3.gr[3], 2));

				G3.Adj[1].push_front(Graph::Edge(&G3.gr[4], 3));

				G3.Adj[2].push_front(Graph::Edge(&G3.gr[3], 4));

				G3.Adj[3].push_front(Graph::Edge(&G3.gr[4], 1));

			}
		}
	}

	//G3.print(G3);
	G3.Johnson(G3);

	cout << "Matrix with shortest paths' weights between all vertices of graph 3:" << endl;
	cout << " | ";

	for (unsigned int i = 1; i <= G3.matr.size(); i++)
		cout << i << ' ';

	cout << endl << endl;

	for (unsigned int j = 0; j < G3.matr.size(); j++) {
		cout << j + 1 << "| ";
		for (unsigned int k = 0; k < G3.matr.size(); k++)
			cout << G3.matr[j][k] << ' ';
		cout << endl;
	}

	system("pause");
	return 0;
}
catch (exception &e) {
	cerr << "Error: " << e.what() << endl;
	system("pause");
	return 1;
}

catch (...) {
	cerr << "Unexpected error" << endl;
	system("pause");
	return 2;
}