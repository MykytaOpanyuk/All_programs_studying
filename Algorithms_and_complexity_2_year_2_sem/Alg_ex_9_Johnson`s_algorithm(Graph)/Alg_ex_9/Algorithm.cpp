
#include "stdafx.h"
#include "Fibonacci_heap.h"
#include "Algorithm.h"

const double fi = (1 + sqrt(5)) / 2;	// constant needed to get D (for Fibonacci heap)



void Graph::Initialize_Single_Sourse(Graph &G, Vertex &s)
{
	for (Vertex &elem : G.gr) {
		elem.d = INT_MAX;
		elem.pi = 0;
	}
	s.d = 0;
}



void Graph::Relax(Vertex u, Vertex *&v, int w) 
{
	if (v->d > (u.d + w)) {
		v->d = u.d + w;
		v->pi = &u;
	}
}



bool Graph::Belman_Ford(Graph &G, Vertex &s)
{
	Initialize_Single_Sourse(G, s);

	// Relaxes some vertices
	for (unsigned int j = 0; j < G.gr.size() - 1; j++)
		for (unsigned int i = 0; i < G.gr.size(); i++)
			for (auto &elem : G.Adj[i])
				Relax(G.gr[i], elem.v, elem.w);

	// Checks if there are no cycles with negative weight
	for (unsigned int i = 0; i < G.gr.size(); i++)
		for (auto &elem : G.Adj[i])
			if (elem.v->d > G.gr[i].d + elem.w)
				return false;
	return true;
}



void Graph::print(Graph G)
{
	for (unsigned int i = 0; i < G.gr.size(); i++) {
		cout << '(' << G.gr[i].name << ", " << G.gr[i].d << ')';
		for (Edge elem : G.Adj[i])
			cout << " -(" << elem.w << ")-> (" << elem.v->name << ", " << elem.v->d << ')';
		cout << endl;

	}

	cout << endl;

}



vector<unsigned int> Graph::Dijkstra(Graph &G, Vertex &s) 
{

	FH *H = NULL;
	H = H->Make();
	vector<FH::RTN*> vertices;
	Graph::Initialize_Single_Sourse(G, s);

	for (Vertex &elem : G.gr) {
		FH::RTN* x = new FH::RTN(&elem);
		x->state = UNLABELED;
		vertices.push_back(x);
	}

	vertices[s.number]->state = LABELED;
	H->Insert(H, vertices[s.number]);

	do {
		FH::RTN* v = H->Extract_Min(H);
		v->state = SCANNED;

		for (auto elem : G.Adj[v->key->number]) {
			FH::RTN *headOfCurrentEdge = vertices[elem.v->number];
			if (headOfCurrentEdge->state != SCANNED) {
				if (headOfCurrentEdge->state == UNLABELED) {
					// Inserts a vertex with infinite key

					headOfCurrentEdge->state = LABELED;
					headOfCurrentEdge->key->pi = v->key;
					headOfCurrentEdge->key->d = v->key->d + elem.w;
					H->Insert(H, headOfCurrentEdge);
				}
				else if (headOfCurrentEdge->key->d > v->key->d + elem.w) {
					// Decreases the key of a vertex with finite key
					headOfCurrentEdge->key->pi = v->key;
					H->Decrease_Key(H, headOfCurrentEdge, v->key->d + elem.w);
				}

			}

		}

	} while (H->min);



	// Shortest paths' weights for vertex

	vector<unsigned int> f(vertices.size());

	for (unsigned int i = 0; i < vertices.size(); i++)

		f[i] = vertices[i]->key->d;

	return f;

}



vector<vector<unsigned int>> *Graph::Johnson(Graph &G1) {

	if (!Belman_Ford(G1, G1.gr[0]))

		throw runtime_error("graph contains cycle with negative weight!\n");



	// Changes weights to positive

	for (unsigned int i = 0; i < G1.gr.size(); i++)

		for (Edge &elem : G1.Adj[i])

			elem.w = elem.w + G1.gr[i].d - elem.v->d;



	// Calls Dijkstra algorithm for each vertex

	for (unsigned int i = 1; i < G1.gr.size(); i++) {
		vector <unsigned int> f = G1.Dijkstra(G1, G1.gr[i]);
		for (int j = 1; j < G1.gr.size(); j++)
			matr[i - 1][j - 1] = f[j];
	}
	return &matr;
}