
#include "Profiler.h"
#include <crtdbg.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include "QS.h"

#define _CRTDBG_MAP_ALLOC
using std::cout;
using std::cin;
using std::swap;
using std::sort;
using std::vector;

Profiler profiler("lab8");


Node* MAKE_SET(int val,Operation* MAKE_SET_OP=nullptr) {
	Node* aux = (Node*)malloc(sizeof(Node));
	aux->parent = aux;
	aux->val = val;
	aux->rank = 0;
	if (MAKE_SET_OP) MAKE_SET_OP->count(4);
	return aux;
}

Node* FIND_SET(Node* current,Operation* FIND_SET_OP=nullptr) {
	if (current->parent != current) { /// its not  the representative 
		if (FIND_SET_OP) FIND_SET_OP->count(2);
		current->parent = FIND_SET(current->parent, FIND_SET_OP);
	}
	if (FIND_SET_OP) FIND_SET_OP->count();
	/// it will automatically link it to representative and return it , helps in the long-run 
	return current->parent; /// return repr (after path compression)
}

void LINK(Node* a, Node* b,Operation* UNION_OP = nullptr) {
	if (UNION_OP) UNION_OP->count();
	if (a->rank > b->rank) { /// root with higher rank becomes parent of smaller one , hence we dont increase height 
		b->parent = a;
		if (UNION_OP) UNION_OP->count();
	}
	else {
		if (UNION_OP) UNION_OP->count();
		if (a->rank < b->rank) {
			a->parent = b;
			if (UNION_OP) UNION_OP->count();
		}
		else { /// equal upper bound of their heights
			b->parent = a;
			a->rank++;
			if (UNION_OP) UNION_OP->count(2);
		}
	}
}

void UNION(Node* a, Node* b, Operation* UNION_OP=nullptr, Operation* FIND_SET_OP = nullptr) { /// union between 2 elements of distinct sets => merge representatives
	Node* rep_a = FIND_SET(a, FIND_SET_OP);
	Node* rep_b = FIND_SET(b, FIND_SET_OP);
	if (UNION_OP)  UNION_OP->count(3);
 	if (rep_a != rep_b) /// if from same set , do nothing
		LINK(rep_a, rep_b,UNION_OP);
}

void test_sets() {

	Node* one = MAKE_SET(1);
	Node* two = MAKE_SET(2);
	Node* three = MAKE_SET(3);
	Node* four = MAKE_SET(4);
	Node* five = MAKE_SET(5);
	Node* six = MAKE_SET(6);
	Node* seven = MAKE_SET(7);
	Node* eight = MAKE_SET(8);
	Node* nine = MAKE_SET(9);
	Node* ten = MAKE_SET(10);

	vector<Node*> array;
	array.push_back(one);
	array.push_back(two);
	array.push_back(three);
	array.push_back(four);
	array.push_back(five);
	array.push_back(six);
	array.push_back(seven);
	array.push_back(eight);
	array.push_back(nine);
	array.push_back(ten);

	cout << "Before:\n";
	for (const auto node : array)
		cout << node->val << " rank " << node->rank << " parent " << node->parent->val << "\n";

	/// these can be changed:
	UNION(six, seven);
	UNION(five, eight);
	UNION(five, six);
	UNION(one, two);
	UNION(two, six);
	UNION(ten, nine);

	/// FIND_SET(seven);
	/// FIND_SET(two);

	cout << "\nAfter:\n";
	for (const auto node : array)
		cout << node->val << " rank " << node->rank << " parent " << node->parent->val << "\n";

	int pus[11] = { 0 }; /// carac array 

	cout << "\nSETS:\n";

	for (int i = 0; i < array.size(); ++i) {

		if (pus[array[i]->val] == 1) /// visited
			continue;

		else { /// either 1 single set or part of a bigger set , display the set

			cout <<"{ "<<array[i]->val << " ";
			pus[array[i]->val] = 1;

			for (int j = 0; j < array.size(); ++j) {
				if (pus[array[j]->val] == 0 && FIND_SET(array[i]) == FIND_SET(array[j])) { /// are from the same group , display and mark as visited
					pus[array[j]->val] = 1;
					cout << array[j]->val << " ";
				}
			}
			cout << "}\n";
		}
	}
		
	cout << "\nFIND_SET:\n";

	cout << "find set for 2 : " << FIND_SET(two)->val << "\n";
	cout << "find set for 8 : " << FIND_SET(eight)->val << "\n";
	cout << "find set for 9 : " << FIND_SET(nine)->val << "\n";
	cout << "find set for 6 : " << FIND_SET(six)->val << "\n";
	cout << "find set for 3 : " << FIND_SET(three)->val << "\n";

	for (auto& node : array)
		free(node);

	array.clear();
}

Edge* CreateEdge(int val, Node* a, Node* b) {
	Edge* aux = (Edge*)malloc(sizeof(Edge));
	aux->val = val;
	aux->a = a;
	aux->b = b;
	return aux;
}

void generate_random_edges(Edge**& EdgeArray, int v, int e, Node** NodeArray) {

	if (e < v - 1) {
		cout << "not enough edges to be connected";
		exit(1);
	}
	if (e > (v * (v - 1)) / 2) {
		cout << "to many edges";
		exit(2);
	}

	vector<std::pair<int,int>> arr;

	int cnt = 0;
	EdgeArray = (Edge**)malloc(sizeof(Edge*) * e);
	int i = 0;
	for (; i < v-1; ++i) {
		EdgeArray[i] = CreateEdge(((rand() % 1000) + 1), NodeArray[i], NodeArray[i + 1]);
		arr.push_back(std::make_pair(i+1, i + 2)); /// add edges in order (1-2,2-3,3-4...until v-1 - v) to ensure that graph is conex
		cnt++;
	}

	int remain = e - cnt; /// remain represents how many more edges must add , after we ensured that the graf is connex
	for (int j = 0; j < remain; ++j) {

		int A, B;
		A = rand() % v; /// randomly generate ends
		B = rand() % v;

		/// check if there`s none already edge between them or to the same vertex
		while (A == B || (std::find(arr.begin(), arr.end(), std::make_pair(A+1, B+1)) != arr.end()) ||
			(std::find(arr.begin(), arr.end(), std::make_pair(B+1, A+1)) != arr.end())) { 
			A = rand() % v;
			B = rand() % v;
		}

		EdgeArray[i + j] = CreateEdge(((rand() % 1000) + 1), NodeArray[A], NodeArray[B]);
		arr.push_back(std::make_pair(A+1, B+1));

	}
	arr.clear();
}

void Kruskal(Edge **  EdgeArray,int V,int E,Operation* FIND_SET_OP = nullptr,Operation* UNION_OP = nullptr) {

	/// EdgeArray[1]->val = NULL; nu merge , e const edge
	/// EdgeArray = NULL; nu merge , e const edge EdgeArray ptr

	Edge** EdgeArrayResult = (Edge**)malloc(sizeof(Edge*) * V - 1); /// MST has v-1 edges
	Q_S(EdgeArray, E); /// sort the edges from the array

	int cnt = 0;
	for (int i = 0; i < E; ++i)
	{
		if (FIND_SET(EdgeArray[i]->a, FIND_SET_OP) != FIND_SET(EdgeArray[i]->b, FIND_SET_OP)) { /// margin vertices belong in different trees
			EdgeArrayResult[cnt++] = EdgeArray[i]; // add edge to rez
			UNION(EdgeArray[i]->a, EdgeArray[i]->b,UNION_OP, FIND_SET_OP); /// mark the 2 ends as in the same tree , not to make a cycle at further takings
		}
	}
	if (FIND_SET_OP == nullptr && UNION_OP == nullptr) {
		cout << "\nKruskal MST:\n";
		for (int i = 0; i < cnt; ++i)
			cout << "weight: " << std::setw(4) << std::left << EdgeArrayResult[i]->val << " (" << EdgeArrayResult[i]->a->val << ") ------- (" << EdgeArrayResult[i]->b->val << ")\n";
	}
	
	free(EdgeArrayResult);
	EdgeArrayResult = nullptr;
}

#define V 5
#define E 9

void test_kruskal() {

	Node** NodeArray = nullptr;
	Edge** EdgeArray = nullptr;

	/// set the nodes of graph as individual sets with 1 elem.
	NodeArray = (Node**)malloc(sizeof(Node*) * V);
	for (int i = 0; i < V; ++i)
		NodeArray[i] = MAKE_SET(i + 1); 

	cout << "Default sets:\n";
	for (int i = 0; i < V; ++i)
		cout << NodeArray[i]->val << " rank " << NodeArray[i]->rank << " parent " << NodeArray[i]->parent->val << "\n";

	cout << "\n";

	/// Generates randomly edges for a weighted graph, it also could have been manually added...
	generate_random_edges(EdgeArray, V, E, NodeArray);
	cout << "Edges Before Sort:\n";
	for (int i = 0; i < E; ++i)
		cout <<i+1<<". weight: "<<std::setw(4) << std::left<<EdgeArray[i]->val << " (" << EdgeArray[i]->a->val << ") ------- (" << EdgeArray[i]->b->val << ")\n";

	///// KRUSKAL
	Kruskal(EdgeArray, V, E);

	cout << "\nEdges After Sort:\n";
	for (int i = 0; i < E; ++i)
		cout << i + 1 << ". weight: " << std::setw(4) << std::left << EdgeArray[i]->val << " (" << EdgeArray[i]->a->val << ") ------- (" << EdgeArray[i]->b->val << ")\n";

	for (int i = 0; i < V; ++i)
		free(NodeArray[i]);
	for (int i = 0; i < E; ++i)
		free(EdgeArray[i]);

	free(EdgeArray);
	free(NodeArray);
}

void perf() {
	for (int n = 100; n <= 10000; n = n + 100) {
		Node** NodeArray = nullptr;
		Edge** EdgeArray = nullptr;

		Operation MAKE_SET_OP = profiler.createOperation("MAKE_SET_OP", n);
		Operation UNION_OP = profiler.createOperation("UNION_OP", n);
		Operation FIND_SET_OP = profiler.createOperation("FIND_SET_OP", n);

		NodeArray = (Node**)malloc(sizeof(Node*) * n);
		for (int i = 0; i < n; ++i)
			NodeArray[i] = MAKE_SET(i + 1,&MAKE_SET_OP);

		generate_random_edges(EdgeArray, n, 4*n, NodeArray);
		Kruskal(EdgeArray,n,4*n,&FIND_SET_OP, &UNION_OP);

		for (int i = 0; i < n; ++i)
			free(NodeArray[i]);
		for (int i = 0; i < n*4; ++i)
			free(EdgeArray[i]);

		free(EdgeArray);
		free(NodeArray);
	}
	profiler.createGroup("OPERATION","MAKE_SET_OP", "UNION_OP","FIND_SET_OP");
	profiler.showReport();
}

int main() {

	srand(time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//test_sets();
	//test_kruskal();
	//perf();

	return 0;
}