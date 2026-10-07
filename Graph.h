#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Graph
{
public:

	Graph(int num_vertical);

	~Graph();

	void addEdge(int start_vertex, int end_vertex, float weight);

	void removeEdge(int start_vertex, int end_vertex);

	void addVertex();

	void removeVertex(int vertex);

	int getVertexByName(string& name);

	void likeAction(int start_vertex, int end_vertex);

	void iterativeSearchDFS(int start_vertex, bool* visit, int* stack, int& top, int* finish_stack, int& finish_top);

	void transposedIterativeSearchDFS(int start_vertex, bool* visit, int* stack, int& top, float** transposed_matrix, int& current_size, string& current_line);

	void transposeGraph(float** transposed_matrix);

	void kosarajuSCC();

	string biggestInfluence(int k);

	void mostProbablePath(int start_vertex, int end_vertex);

	void printGraph();

	void setNames(string* arr_names);

private:

	float** adjacency_matrix;
	int num_vertical;
	string* names;
};
#endif