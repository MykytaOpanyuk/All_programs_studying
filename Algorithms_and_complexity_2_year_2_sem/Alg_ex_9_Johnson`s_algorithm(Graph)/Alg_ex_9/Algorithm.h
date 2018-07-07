#pragma once
#pragma once

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <forward_list>
#include <string>

using namespace std;

struct Graph {					// oriented sparse weighed graph

	struct Vertex {				// graph's vertex structure
		int d;					// shortest path's weight from origin
		Vertex *pi;				// vertex's predecessor
		string name;				// letter attached to each vertex
		unsigned int number;	// sequence number of vertex used in adjacency list
		string group;
		// New vertex
		Vertex(string name, unsigned int number, string group) : d(0), name(name), number(number), group(group) {}
	};

	struct Edge {				// graph's edge structure
		Vertex *v;				// target vertex
		int w;					// edge's weight
		// New edge
		Edge(Vertex *v_, int w_) : v(v_), w(w_) {}
	};

	vector<forward_list<Edge>> Adj;	// adjacency list
	vector<Vertex> gr;			// contains all vertices of graph
	vector<vector<unsigned int>> matr;	// matrix with shortest paths' weights between all vertices

	// New graph by number of vertices
	Graph(unsigned int n) {
		Adj.resize(n);
		matr.resize(n - 1, vector<unsigned int>(n - 1));
	}

	// Assigns initial shortest paths' weights from origin and predecessors
	void Initialize_Single_Sourse(Graph&, Vertex&);

	// Checks if it is possible to make shortest paths' weight better
	void Relax(Vertex, Vertex*&, int);

	// Determines shortest paths' weights from origin if there are no cycles with negative weight
	bool Belman_Ford(Graph&, Vertex&);

	// Determines shortest paths' weights from origin
	vector<unsigned int> Dijkstra(Graph&, Vertex&);

	// Determines matrix with shortest paths' weights between all vertices
	vector<vector<unsigned int>> *Johnson(Graph&);

	// Prints out graph's adjacency li
	void print(Graph);

};