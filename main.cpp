#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"

int main() {

	string file_name;
	ifstream file;

	//	Read file name
	while (true) {
		cout << "Enter file name: ";
		cin >> file_name;
		file.open(file_name, ios_base::in);

		if (file.is_open()) {
			break;
		}
		cout << "Error opening file!" << endl;
	}

	stringstream ss;
	string line;
	int vertex, edge;

	cout << endl;
	cout << " A graph implemented using an adjacency matrix: " << endl;

	//	Read num of vertex
	getline(file, line);
	ss.str(line);
	ss >> vertex;
	ss.clear();

	//	Read num of edge
	getline(file, line);
	ss.str(line);
	ss >> edge;
	ss.clear();

	//	Create graph 
	Graph* g = new Graph(vertex);

	//	Read all vertex
	getline(file, line);
	ss.str(line);
	string* names = new string[vertex];
	string* copy_names = new string[vertex];
	
	for (int i = 0; i < vertex; i++) {
		ss >> names[i];
	}
	ss.clear();
	g->setNames(names);
	
	//	Read all edges
	string first_name, second_name;  
	float weight;
	int fn, sn;
	for (int i = 0; i < edge; i++) {
		getline(file, line);
		ss.str(line);
		ss >> first_name >> second_name >> weight;
		fn = g->getVertexByName(first_name);
		sn = g->getVertexByName(second_name);
		g->addEdge(fn, sn, weight);
		ss.clear();
	}

	//	Print graph
	g->printGraph();
	ss.clear();
	
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << " Choose one of the options for working with the graph: " << endl;
	cout << " 0. End the program." << endl;
	cout << " 1. Add edge in graph." << endl;
	cout << " 2. Remove edge in graph." << endl;
	cout << " 3. Add vertex in graph." << endl;
	cout << " 4. Remove vertex in graph." << endl;
	cout << " 5. Like action." << endl;
	cout << " 6. Finding the largest component in a graph that represents a subset of users who are all interconnected." << endl;
	cout << " 7. Print all users on the most likely path from one given user to another given user, in the format: user1 - (probability)->user2 - (probability)->..." << endl;
	cout << " 8. Effective determination of the k-th person (k inputs by the user), with the greatest influence on the social network." << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "$ Your option [0-8]: ";

	string name1, name2;
	int index1, index2;
	int option;
	cin >> option;

	while (option) {

		switch (option)
		{
		case 1: {

			//	Option 1
			cin.ignore();
			cout << "Enter two names (string) and the weight (float) between them in the format: name1 name2 weight. A edge is represented as a path from name1 to name2: ";
			getline(cin, line);
			ss.str(line);
			ss >> name1 >> name2 >> weight;
			ss.clear();

			index1 = g->getVertexByName(name1);
			index2 = g->getVertexByName(name2);

			if (index1 == -1 && index2 == -1) {
				cout << "Non-name vertices!" << endl;
				break;
			}

			if (index1 == index2) {
				cout << "Vertex cannot have an edge to itself." << endl;
			}
			else {
				if (index1 >= 0 && index1 < vertex && index2 >= 0 && index2 < vertex) {

					if (weight > 0 && weight <= 1) {
						g->addEdge(index1, index2, weight);
						cout << endl;
						g->printGraph();
					}
					else {
						cout << "Weight must be in the interval (0,1]." << endl;
						break;
					}

				}
				else {
					cout << "Non-name vertex!" << endl;
					if (weight <= 0 || weight > 1) {
						cout << "Weight must be in the interval (0,1]." << endl;
					}
				}
			}
			break;
		};

		case 2: {

			//	Option 2
			cin.ignore();
			cout << "Enter two names (string) in the format: name1 name2. A edge is represented as a path from name1 to name2: ";
			getline(cin, line);
			ss.str(line);
			ss >> name1 >> name2;
			ss.clear();

			index1 = g->getVertexByName(name1);
			index2 = g->getVertexByName(name2);

			if (index1 == -1 && index2 == -1) {
				cout << "Non-name vertices!" << endl;
				break;
			}

			if (index1 == index2) {
				cout << "Vertex cannot have an edge to itself." << endl;
			}
			else {
				if (index1 >= 0 && index1 < vertex && index2 >= 0 && index2 < vertex) {
					g->removeEdge(index1, index2);
					cout << endl;
					g->printGraph();
				}
				else {
					cout << "Non-name vertex!" << endl;
				}
			}
			break;
		};

		case 3: {

			//	Option 3
			cin.ignore();
			copy_names = new string[vertex + 1];
			cout << "Enter one name (string) in the format: name1. New vertex will be represented by that name: ";
			cin >> name1;
			bool exists_name = false;

			for (int i = 0; i < vertex; i++) {
				if (names[i] == name1) {
					exists_name = true;
					cout << name1 << " already exists in graph." << endl;
				}
			}

			//	Add entered name in graph
			if (!exists_name) {
				for (int i = 0; i < vertex; i++) {
					copy_names[i] = names[i];
				}
				copy_names[vertex] = name1;
				g->setNames(copy_names);
				g->addVertex();
				vertex++;
				g->printGraph();

				delete[] names;
				names = copy_names;
			}
			break;
		};

		case 4: {

			//	Option 4
			cin.ignore();
			copy_names = new string[vertex - 1];
			cout << "Enter one name (string) in the format: name1. Vertex represented by that name will be deleted: ";
			cin >> name1;
			index1 = g->getVertexByName(name1);
			
			if (index1 == -1) {
				cout << "Name you entered is not in the graph, it is not possible to delete it." << endl;
				break;
			}

			//	Remove entered name from graph
			int j = 0;
			for (int i = 0; i < vertex; i++) {
				if (i != index1) {
					copy_names[j] = names[i];
					j++;
				}
			}
			g->setNames(copy_names);
			g->removeVertex(index1);
			vertex--;
			g->printGraph();
			
			delete[] names;
			names = copy_names;
			break;	
		};

		case 5: {

			//	Option 5
			cin.ignore();
			cout << "Enter two names(string) in the format : name1 name2. Person1 liked the post of person2: ";

			string like;
			getline(cin, like);
			ss.str(like);
			string first, second;
			ss >> first >> second;
			ss.clear();

			int f, s;
			f = g->getVertexByName(first);
			s = g->getVertexByName(second);

			if (f == -1 && s == -1) {
				cout << "Non-name vertices!" << endl;
				break;
			}

			if (f == s) {
				cout << "Vertex cannot have an edge to itself." << endl;
			}
			else {
				if (f == s && (f != -1 || s != -1)) {
					cout << "Vertex cannot have an edge to itself." << endl;
				}
				else {
					if (f >= 0 && f < vertex && s >= 0 && s < vertex) {
						g->likeAction(f, s);
						cout << endl;
						g->printGraph();
					}
					else {
						cout << "Non-name vertex!" << endl;
					}
				}
			}
			break;
		};

		case 6: {

			//	Option 6
			g->kosarajuSCC();
			break;
		};

		case 7: {

			//	Option 7
			cin.ignore();
			cout << "Enter two names (string) in the format: name1 name2. The resulting path represents the shortest path from name1 to name2: ";
			getline(cin, line);
			ss.str(line);
			ss >> name1 >> name2;
			ss.clear();

			index1 = g->getVertexByName(name1);
			index2 = g->getVertexByName(name2);

			if (index1 == -1 && index2 == -1) {
				cout << "Non-name vertices!" << endl;
				break;
			}

			if (index1 == index2) {
				cout << "Vertex cannot have an edge to itself." << endl;
			}
			else {
				if (index1 >= 0 && index1 < vertex && index2 >= 0 && index2 < vertex) {
					g->mostProbablePath(index1, index2);
				}
				else {
					cout << "Non-name vertex!" << endl;
				}
			}
			break;
		};

		case 8: {

			//	Option 8
			cin.ignore();
			cout << "Enter number k in the interval [1,vertex], vertex - number of vertex in graph: ";
			int k;
			cin >> k;
			string biggest;
			biggest = g->biggestInfluence(k);
			cout << k << "." << " with biggest influence " << biggest << endl;
			break;
		};

		default:

			//	Invalid option
			cout << "Invalid option. Try again." << endl;
			break;
		};

		//	New option input
		cout << "$ Your option [0-8]: ";
		cin >> option;
	}

	//	End of program
	cout << "End of program.";
	cout << endl;

	//	Free dynamically allocated memory.
	delete[] copy_names;
	delete g;
	return 0;
}