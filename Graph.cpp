#include "Graph.h"
#include <iomanip>  

//	Constructor - Allocates memory for the adjacency matrix and initializes all values to 0.
Graph::Graph(int num) : num_vertical(num) {

	adjacency_matrix = new float*[num_vertical];
	names = nullptr;

	for (int i = 0; i < num_vertical; i++) {
		adjacency_matrix[i] = new float[num_vertical];
	}

	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			adjacency_matrix[i][j] = 0;
		}
	}
}

//	Sets the names for the vertices.
void Graph::setNames(string *arr_names) {
	names = arr_names;
}

//	Destructor - Free dynamically allocated memory for the adjacency matrix.
Graph::~Graph() {

	for (int i = 0; i < num_vertical; i++) {
		delete[] adjacency_matrix[i];
	}
	delete[] adjacency_matrix;
}

//	Prints the graph: both the names of the vertices and the adjacency matrix.
void Graph::printGraph() {

	for (int i = 0; i < num_vertical; i++) {
		cout << " " << i << " : " << names[i] << endl;
	}
	cout << endl;

	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			cout << fixed << setprecision(2) << adjacency_matrix[i][j] << " ";
		}
		cout << endl;
	}
}

//	Adds a directed edge from 'start_vertex' to 'end_vertex' with the given weight.
void Graph::addEdge(int start_vertex, int end_vertex, float weight) {
	
	if (start_vertex >= 0 && end_vertex >= 0 && start_vertex < num_vertical && end_vertex < num_vertical) {
		adjacency_matrix[start_vertex][end_vertex] = weight;
	}
}

//	Removes the directed edge from 'start_vertex' to 'end_vertex' (sets weight to 0).
void Graph::removeEdge(int start_vertex, int end_vertex) {

	if (start_vertex >= 0 && end_vertex >= 0 && start_vertex < num_vertical && end_vertex < num_vertical) {
		adjacency_matrix[start_vertex][end_vertex] = 0;
	}
}

//	Adds a new vertex to the graph by reallocating and expanding the adjacency matrix.
void Graph::addVertex() {
	num_vertical++;
	float** new_adjacency_matrix = new float* [num_vertical];

	for (int i = 0; i < num_vertical; i++) {
		new_adjacency_matrix[i] = new float[num_vertical];
	}

	//	Initialize the new matrix with 0.
	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			new_adjacency_matrix[i][j] = 0;
		}
	}

	//	Copy old matrix into the new, larger matrix.
	for (int i = 0; i < num_vertical - 1; i++) {
		for (int j = 0; j < num_vertical - 1; j++) {
			new_adjacency_matrix[i][j] = adjacency_matrix[i][j];
		}
	}

	//	Free old matrix memory.
	for (int i = 0; i < num_vertical - 1; i++) {
		delete[] adjacency_matrix[i];
	}
	delete[] adjacency_matrix;

	adjacency_matrix = new_adjacency_matrix;
}

//	Removes a vertex from the graph by reallocating and shrinking the adjacency matrix.
void Graph::removeVertex(int vertex) {
	if (vertex >= 0 && vertex < num_vertical) {
		num_vertical--;
	}

	float** new_adjacency_matrix = new float* [num_vertical];

	for (int i = 0; i < num_vertical; i++) {
		new_adjacency_matrix[i] = new float[num_vertical];
	}

	//	Initialize the new matrix with 0.
	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			new_adjacency_matrix[i][j] = 0;
		}
	}

	//	Copy all values except the removed vertex row/column.
	int new_row = 0;
	for (int i = 0; i < num_vertical + 1; i++) {
		if (i != vertex) {
			int new_column = 0;
			for (int j = 0; j < num_vertical + 1; j++) {
				if (j != vertex) {
					new_adjacency_matrix[new_row][new_column] = adjacency_matrix[i][j];
					new_column++;
				}
			}
			new_row++;
		}
	}

	//	Free old matrix memory.
	for (int i = 0; i < num_vertical - 1; i++) {
		delete[] adjacency_matrix[i];
	}
	delete[] adjacency_matrix;

	adjacency_matrix = new_adjacency_matrix;
}

//	Returns the index of a vertex by its name, or -1 if not found.
int Graph::getVertexByName(string& name) {
	for (int i = 0; i < num_vertical; i++) {
		if (names[i] == name) {
			return i;
		}
	}
	return -1;
}

//	Increases the weight of the edge between 'start_vertex' and 'end_vertex' by 0.1, but caps it at 1.0.
void Graph::likeAction(int start_vertex, int end_vertex) {

	float weight = adjacency_matrix[start_vertex][end_vertex];

	weight = weight + 0.10;

	if (weight > 1.00) {
		weight = 1.00;
	}
	adjacency_matrix[start_vertex][end_vertex] = weight;
}

//	Performs an iterative Depth-First Search (DFS) to compute finishing times for Kosaraju's algorithm.
void Graph::iterativeSearchDFS(int start_vertex, bool* visit, int* stack, int& top, int* finish_stack, int& finish_top) {

	//	Marks nodes currently on the stack to avoid pushing the same node multiple times.
	bool* in_stack = new bool[num_vertical]();
	stack[++top] = start_vertex;
	in_stack[start_vertex] = true;

	while (top >= 0) {
		int vertex = stack[top];
		if (!visit[vertex]) {
			visit[vertex] = true;
		}

		bool found_unvisited = false;
		//	Explore neighbors
		for (int i = 0; i < num_vertical; i++) {
			if (adjacency_matrix[vertex][i] != 0 && !visit[i] && !in_stack[i]) {
				stack[++top] = i;
				in_stack[i] = true;
				found_unvisited = true;
				break;
			}
		}

		//	If no unvisited neighbors found, finish the current node and pop from stack
		if (!found_unvisited) {
			finish_stack[++finish_top] = vertex;
			in_stack[vertex] = false;
			top--;
		}
	}
	//	Free dynamically allocated memory.
	delete[] in_stack;
}

//	Performs iterative DFS on the transposed graph to collect strongly connected components.
void Graph::transposedIterativeSearchDFS(int start_vertex, bool* visit, int* stack, int& top, float** transposed_matrix, int& current_size, string& current_line) {

	stack[++top] = start_vertex;

	while (top >= 0) {
		int vertex = stack[top--];

		if (!visit[vertex]) {
			visit[vertex] = true;
			current_size++; // Count the size of the current component
			current_line += names[vertex] + "--"; // Collect names for output

			//	Push all unvisited neighbors from the transposed graph onto the stack
			for (int i = 0; i < num_vertical; i++) {
				if (transposed_matrix[vertex][i] != 0 && !visit[i]) {
					stack[++top] = i;
				}
			}
		}
	}
}

//	Constructs the transposed graph by reversing the direction of all edges.
void Graph::transposeGraph(float** transposed_matrix) {
	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			transposed_matrix[j][i] = adjacency_matrix[i][j];
		}
	}
}

//	Kosaraju's algorithm for finding Strongly Connected Components (SCC) in a directed graph.
//	Uses two passes of DFS: one to compute finishing order, and one on the transposed graph to collect SCCs.
void Graph::kosarajuSCC() {

	int* stack = new int[num_vertical];      // DFS stack
	int top = -1;
	int* finish_stack = new int[num_vertical]; // Stores nodes in order of decreasing finishing time
	int finish_top = -1;
	bool* visit = new bool[num_vertical]();  // Tracks visited nodes during DFS

	//	First DFS pass: record nodes in finish_stack in the order of completion
	for (int i = 0; i < num_vertical; i++) {
		if (!visit[i]) {
			iterativeSearchDFS(i, visit, stack, top, finish_stack, finish_top);
		}
	}

	//	Create and compute the transposed graph (reverse all edges)
	float** transposed_matrix = new float* [num_vertical];
	for (int i = 0; i < num_vertical; i++) {
		transposed_matrix[i] = new float[num_vertical]();
	}
	transposeGraph(transposed_matrix);

	//	Reset visit array for the second DFS pass
	for (int i = 0; i < num_vertical; i++) {
		visit[i] = false;
	}

	int max_size = 0;
	string largest_line = "";

	//	Second DFS pass: find all SCCs by processing nodes in reverse finish order
	for (int i = finish_top; i >= 0; i--) {
		int vertex = finish_stack[i];

		if (!visit[vertex]) {
			int current_size = 0;
			string current_line = "";

			top = -1;
			transposedIterativeSearchDFS(vertex, visit, stack, top, transposed_matrix, current_size, current_line);

			//	Keep track of the largest SCC found
			if (current_size > max_size) {
				max_size = current_size;
				largest_line = current_line;
			}
		}
	}

	//	Remove trailing "--" from the output string
	if (!largest_line.empty()) {
		largest_line = largest_line.substr(0, largest_line.size() - 2);
	}

	//	Output the largest strongly connected component
	cout << "Largest component in the graph: " << largest_line << endl;

	//	Free dynamically allocated memory.
	delete[] visit;
	delete[] stack;
	delete[] finish_stack;
	for (int i = 0; i < num_vertical; i++) {
		delete[] transposed_matrix[i];
	}
	delete[] transposed_matrix;
}

//	This is similar to Dijkstra's algorithm but maximizes the product instead of minimizing the sum.
void Graph::mostProbablePath(int start_vertex, int end_vertex) {

	//	Initialize arrays for probabilities, previous nodes in path, and visited nodes.
	float* probabilities = new float[num_vertical];
	int* previous = new int[num_vertical];
	bool* visit = new bool[num_vertical]();

	for (int i = 0; i < num_vertical; i++) {
		probabilities[i] = 0;
		previous[i] = -1;
	}
	probabilities[start_vertex] = 1;

	//	Find the most probable paths to all vertices from the start vertex.
	for (int i = 0; i < num_vertical - 1; i++) {
		int u = -1;
		float max_probability = 0;

		//	Find the unvisited vertex with the current maximum probability.
		for (int j = 0; j < num_vertical; j++) {
			if (!visit[j] && probabilities[j] > max_probability) {
				max_probability = probabilities[j];
				u = j;
			}
		}

		if (u == -1) {
			break;
		}
		visit[u] = true;

		//	Update probabilities for neighboring vertex.
		for (int v = 0; v < num_vertical; v++) {
			if (adjacency_matrix[u][v] != 0 && !visit[v]) {
				float new_probability = probabilities[u] * adjacency_matrix[u][v];

				if (new_probability > probabilities[v]) {
					probabilities[v] = new_probability;
					previous[v] = u;
				}
			}
		}
	}

	//	Print the most probable path.
	if (probabilities[end_vertex] == 0) {
			cout << "There is no path from " << names[start_vertex] << " to " << names[end_vertex] << endl;
			return;
	}

	cout << "The most probable path from " << names[start_vertex] << " to " << names[end_vertex] << " with probability "  << probabilities[end_vertex] << ":" << endl;

	//	Reconstruct the path from end_vertex to start_vertex by following 'previous'.
	int current = end_vertex;
	int* path = new int[num_vertical];
	int path_size = 0;
	while (previous[current] != -1) {
		path[path_size++] = current;
		current = previous[current];
	}
	path[path_size++] = start_vertex;

	//	Print the reconstructed path in the correct order.
	int prev_current;
	bool first = true;

	for (int i = path_size-1; i >= 0; i--) {
		current = path[i];
		if (!first) {
			cout << " - (" << adjacency_matrix[prev_current][current] << ") -> ";
		}
		cout << names[current];
		prev_current = current;
		first = false;
	}
	cout << endl;

	//	Free dynamically allocated memory.
	delete[] probabilities;
	delete[] visit;
	delete[] path;
	delete[] previous;
}

string Graph::biggestInfluence(int k) {
	
	string biggest_name = "";

	//	Validate input: k must be between 1 and the number of vertices.
	if (k < 1 || k > num_vertical) {
		return "Invalid value for k.";
	}

	//	Allocate memory for a copy of the adjacency matrix.
	float** copy_matrix = new float* [num_vertical];
	for (int i = 0; i < num_vertical; i++) {
		copy_matrix[i] = new float[num_vertical];
	}

	//	Initialize influence array (accumulated influence for each vertex).
	float* influence = new float[num_vertical];
	for (int i = 0; i < num_vertical; i++) {
		influence[i] = 1.0;
	}

	//	Copy original adjacency matrix to preserve it for future use.
	for (int i = 0; i < num_vertical; i++) {
		for (int j = 0; j < num_vertical; j++) {
			copy_matrix[i][j] = adjacency_matrix[i][j];
		}
	}

	//	Floyd-Warshall's algorithm to find the strongest (most probable) paths between all pairs of vertices.
	for (int k = 0; k < num_vertical; k++) {
		for (int i = 0; i < num_vertical; i++) {
			for (int j = 0; j < num_vertical; j++) {
				if (copy_matrix[i][j] < copy_matrix[i][k] * copy_matrix[k][j]) {
					copy_matrix[i][j] = copy_matrix[i][k] * copy_matrix[k][j];
				}
			}
		}
	}

	//	Calculate the total influence for each vertex as the product of all non-zero values in its column.
	//	If all values in a column are zero, set the influence to 0.
	for (int j = 0; j < num_vertical; j++) {
		bool found_nonzero = false;

		for (int i = 0; i < num_vertical; i++) {
			if (copy_matrix[i][j] != 0) {
				influence[j] *= copy_matrix[i][j];
				found_nonzero = true;
			}
		}
		if (!found_nonzero) {
			influence[j] = 0.0;
		}

	}

	//	Create a copy of the influence array for sorting purposes.
	float* copy_influence = new float[num_vertical];
	for (int i = 0; i < num_vertical; i++) {
		copy_influence[i] = influence[i];
	}

	//	Sort the influence values in descending order.
	for (int i = 0; i < num_vertical; i++) {
		for (int j = i + 1; j < num_vertical; j++) {
			if (copy_influence[i] < copy_influence[j]) {
				float tmp = copy_influence[i];
				copy_influence[i] = copy_influence[j];
				copy_influence[j] = tmp;
			}
		}
	}

	//	Find the vertex whose influence matches the k-th largest influence.
	k = k - 1; 
	for (int i = 0; i < num_vertical; i++) {
		if (influence[i] == copy_influence[k]) {
			biggest_name = names[i];
			break;
		}
	}

	//	Free dynamically allocated memory.
	for (int i = 0; i < num_vertical; i++) {
		delete[] copy_matrix[i];
	}
	delete[] copy_matrix;
	delete[] influence;
	delete[] copy_influence;

	return biggest_name;
}