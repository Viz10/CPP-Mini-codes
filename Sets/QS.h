#pragma once
#include <iostream>

struct Node {
	int val;
	struct Node* parent;
	int rank;
};

struct Edge {
	int val;
	Node* a; /// edge mergins
	Node* b;
};

void Q_S(Edge**, int);