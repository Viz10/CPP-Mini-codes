#include "Profiler.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <crtdbg.h>
#include <queue>
#define _CRTDBG_MAP_ALLOC

/*
Analyse the time and space efficiency of the two transformations. Did you achieve O(n)? Did you
use additional memory:

The 2 transformations have succeded in being implemented in O(N) time .

The first transformation , from parent array into a multiway tree with children array for each node
takes O(3N) one traversal to find for how many is the node parent , one traversal to construct the nodes and one traversal to add children into parent node.However it came with additional
memory , an array of pointers which establich each node beforehand , hence when traversing the array we can directly add the child into it

The second transformation uses 2 queues , one for traversing level by level the R2 tree , processing each node and creating another one , with same value but for the binary one
Whilst the second queue , parallel with the 1st at processing the exact same nodes , is useful because helps building the binary multiway tree level by level inserting on left if first
node in the row or to the right of it if on the same level , with the help of the queue each added node will be proces to find out because if this node from R2 has a layer of children , they will need to be 
added accordingly to R3`s representation 

*/

using std::cin;
using std::cout;
using std::swap;
using std::queue;

Profiler profiler("lab 6");

struct Node {
	int val;
	struct Node* st;
	struct Node* dr;
	struct Node* parent;
};

Node* NodeCreate(int val, Node* parent) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->val = val;
	node->dr = NULL;
	node->st = NULL;
	node->parent = parent;
	return node;
}

Node* createPBT(int* a, int st, int dr, Node* parent) {
	if (st <= dr) {
		int mid = (st + dr) / 2;
		Node* root = NodeCreate(a[mid],parent); /// take mid point as root hence both the left and right subtrees of any node will have approximately the same number of elements
		root->st = createPBT(a, st, mid - 1,root); /// now , left subtree will have nodes less than root
		root->dr = createPBT(a, mid+1, dr,root); /// now , left subtree will have nodes less than root
		return root; // this way , after recursion for both subtrees
					//they will be perfectly balanced also
	}
	return NULL; /// no more elements to process in the subarray
}

void recursive_print(Node* root, const char*order, Operation* op_recursive = nullptr) {
	if (root != NULL) {
		if (strcmp(order,"preorder")==0) {
			if (op_recursive) {
				op_recursive->count();
			}
			else {
				cout << root->val << " ";
			}
		}
		recursive_print(root->st,order, op_recursive);
		if (strcmp(order, "inorder") == 0) {
			if (op_recursive) {
				op_recursive->count();
			}
			else {
				cout << root->val << " ";
			}
		}
		recursive_print(root->dr,order, op_recursive);
		if (strcmp(order, "postorder") == 0) {
			if (op_recursive) {
				op_recursive->count();
			}
			else {
				cout << root->val << " ";
			}
		}
	}
}

void iterative_print(Node* root, int d,const char* order,Operation* op_iterative=nullptr) {
	Node* node = root;
	do {
		if (d == 1) {
			if (strcmp(order, "preorder") == 0) {
				if (op_iterative) {
					op_iterative->count();
				}
				else {
					cout << node->val << " ";
				}
			}
			if (node->st != NULL)
			{
				node = node->st;
			}
			else {
				d = 2;
			}
		}
		if (d == 2) {
			if (strcmp(order, "inorder") == 0) {
				if (op_iterative) {
					op_iterative->count();
				}
				else {
					cout << node->val << " ";
				}
			}
			if (node->dr != NULL) {
				node = node->dr;
				d = 1;
			}
			else {
				d = 3;
			}
		}
		if (d == 3) {
			if (strcmp(order, "postorder") == 0) {
				if (op_iterative) {
					op_iterative->count();
				}
				else {
					cout << node->val << " ";
				}
			}
			if (node->parent != NULL)
			{
				if (node == node->parent->st) {
					d = 2;
				}
				node = node->parent;
			}
			
		}
	} while(node!= root || d != 3); /// doesn not print the root in postorder traversal (last root to be visited)
	if (strcmp(order, "postorder") == 0) {
		if (node != NULL) {
			if (op_iterative) {
				op_iterative->count();
			}
			else {
				cout << node->val << " ";
			}
		}
	}
}

void dealloc1(Node* root) {
	if (root != NULL) {
		dealloc1(root->st);
		dealloc1(root->dr);
		free(root);
	}
}

void perf_traversals() {

	int* a = (int*)malloc(sizeof(int) * 10000);
	
	for (int l = 100; l <= 10000; l=l+100) {
		FillRandomArray(a, l, 10, 50000, false, ASCENDING);

		Node* root = createPBT(a, 0, l - 1, NULL);
		Operation op_iterative = profiler.createOperation("CNT_ITERATIV", l);
		Operation op_recursiv = profiler.createOperation("CNT_RECURSIV", l);

		recursive_print(root, "preorder", &op_recursiv);
		iterative_print(root, 1,"preorder", &op_iterative);
		
		dealloc1(root);
	}

	profiler.createGroup("Afisari", "CNT_ITERATIV", "CNT_RECURSIV");

	free(a);
	profiler.showReport();
}

#define N 11
void demo_traversals() {
	int* array = (int*)malloc(sizeof(int) * N);
	FillRandomArray(array, N, 1, 30, true, 1); /// inorder array , to ensure PBT applies

	Node* root = createPBT(array, 0, N - 1,NULL);

	const char* choice = "preorder";

	cout << "recursive print: ";
	recursive_print(root,choice);
	cout << "\n";
	cout << "iterative print: ";
	iterative_print(root,1,choice);

	dealloc1(root);
	free(array);
}

struct Node_multiway1 {
	int val;
	int count;
	struct Node_multiway1** array;
};

Node_multiway1* Node_multiway1Create(int val, int count) {

	Node_multiway1* node = (Node_multiway1*)malloc(sizeof(Node_multiway1));

	node->val = val;
	node->count = 0;

	if (count > 0){
		node->array = (Node_multiway1**)malloc(sizeof(Node_multiway1*) * count); /// create fixed array , based on how many children it has
	}
	else {
		node->array = NULL;
	}
	return node;
}

void addChild(Node_multiway1* parent, Node_multiway1* child) {
	parent->array[parent->count++] = child; /// add chilredn
}

Node_multiway1* T1(int* array, int size, Node_multiway1*** freed) {
	int root_nr = -1;
	int* frecventa = (int*)calloc(size, sizeof(int)); // array that holds each node`s nr of children
	Node_multiway1** nodes = (Node_multiway1**)calloc(size, sizeof(Node_multiway1*));
	*freed = nodes;

	for (int i = 0; i < size; ++i) {
		if (array[i] == -1) {
			root_nr = i+1;  // parent array is indexed from 0 but tree from 1
		}
		if (array[i] > 0) { // Prevent out of bounds access for -1 in parent array
			frecventa[array[i]-1]++;
		}
	} /// count how many times each node appears as parent to others

	for (int i = 0; i < size; ++i) {
		nodes[i] = Node_multiway1Create(i + 1, frecventa[i]); /// create node also allocating enough space for its children array
	}

	for (int i = 0; i < size; ++i) {
		if (array[i] > 0) {
			addChild(nodes[array[i]-1], nodes[i]);
		}
	}/// array[i]-1 gives the ith node from the nodes array to insert into  

	free(frecventa);
	return nodes[root_nr-1];
}

void PP2(Node_multiway1*root,int white_space) {
	for (int i = 0; i < white_space; ++i) /// set white spaces from left margin up to the root
		cout << " ";

	cout << root->val << "\n"; /// print root

	for (int i = 0; i < root->count; ++i) /// take every children from left to right , go deep recursevly in the tree ,then backtrack to next child nodes
		PP2(root->array[i], white_space + 3); /// increase nr os spaces
}

void freeTree(Node_multiway1* root) {
	if (root == NULL) {
		return; /// NULL pointer passed as root
	}
	for (int i = 0; i < root->count; ++i) { ///children processed first...
		freeTree(root->array[i]);
	}
	/// done with its children
	if (root->array != NULL) {
		free(root->array); /// array allocated for this node
	}
	free(root); /// free the node itself
}

struct Node_multiway2 {
	int val;
	struct Node_multiway2* st;
	struct Node_multiway2* dr;
};

Node_multiway2* Node_multiway2Create(int val) {
	Node_multiway2* node = (Node_multiway2*)malloc(sizeof(Node_multiway2));
	node->val = val;
	node->dr = NULL;
	node->st = NULL;
	return node;
}

/// instead of building the binary multi way , from the chilren array multiway using recursion and stack
/// where we would have gone deep into the tree process the left most node then go back to it`s brothers and to the same thing
/// here we process and build level by level , 1st node added from parent will be the start of chaining siblings but also (if it has children) top parent of those as well
Node_multiway2* T2(Node_multiway1* old_root) {
	if (old_root == NULL) 
		return NULL;
	
	Node_multiway2* binary_root = Node_multiway2Create(old_root->val); /// create root of binary tree rep. of the multi way

	queue<Node_multiway1*> q1; /// using STL queue
	queue<Node_multiway2*> q2; 

	/// using 2 queues to match the nodes added from the multi way with the ones created from them in the binary

	q1.push(old_root);
	q2.push(binary_root); /// the same nodw will be top in both queues at any time

	while (!q1.empty()) {

		Node_multiway1* aux_multi = q1.front(); 
		Node_multiway2* aux_binary = q2.front();
		Node_multiway2* prev_sibling=NULL; 

		for (int i = 0; i < aux_multi->count; ++i) { /// go through each multiway node`s childredn array and add them to the queue and also to the other tree
			
			Node_multiway2* new_child = Node_multiway2Create(aux_multi->array[i]->val);

			if (i == 0) { /// if its the 1st node from the array of children
				aux_binary->st = new_child; /// top , parent node will have it on its left 
			}
			else {
				prev_sibling->dr = new_child; /// else chain it to the first node added + eventually the next ones on its level before it
			}

			/// crucial to ensure new node will be added after last one in the row and update 
			prev_sibling = new_child;

			q1.push(aux_multi->array[i]); /// BFS adding of children
			q2.push(new_child); /// also the node with same number added to be procesed later in the binary tree
		}
		q1.pop(); /// when we added all the children of top , pop it , top will become the first children added ,so that we can go to the next level of nodes from left to right
		q2.pop(); /// also pop the top from binary ,because one set of sibling nodes were processed move deeper
	}

	return binary_root;
}

void PP3(Node_multiway2* root, int white_spaces) {
	for (int i = 0; i < white_spaces; ++i) 
		cout << " ";

	cout << root->val << "\n";

	if (root->st != NULL) { /// go through the deepest most left child if there exists 
		PP3(root->st, white_spaces + 3);
	}
	if (root->dr != NULL) {	/// if no other left child remaining , process the nodes that are siblings (same level) , in their recursion they will enter the second if statement
		PP3(root->dr, white_spaces);
	}
}

void dealloc2(Node_multiway2* root) {
	if (root != NULL) {
		dealloc2(root->st);
		dealloc2(root->dr);
		free(root);
	}
}

void PP1(int* a, int lenght, int current_root, int white_space) {
	
	for (int i = 0; i < white_space; ++i) { /// display spaces from margin , each level will increae by 3 white spaces
		cout << " ";
	}
	cout << current_root << "\n";

	for (int i = 0; i < lenght; ++i) {
		if (a[i] == current_root) { /// searches again the array and finds first child of current roots
			PP1(a, lenght, i + 1, white_space + 3); /// will try to go deep to find  the 1st most left child of the passed root , i+1 because parent array starts from 0
		}
		/// when it doesnt find children for current root`s first child , it will return the next found children of current root and search for it instead
	}
}

void demo_transformari() {

	int a[] = {2,7,5,2,7,7,-1,5,2 };
	int lenght = (sizeof(a) / 4);


	int root = -1;
	for (int i = 0; i < lenght; ++i) {
		if (a[i] == -1) {
			root = i + 1; 
			break;
		}
	}
	cout << "\nPP1:\n";
	PP1(a,lenght,root,0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Node_multiway1** freed=NULL;
	Node_multiway1*root1= T1(a, lenght,&freed);
	cout << "\nPP2:\n";
	PP2(root1,0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Node_multiway2*root2=T2(root1);
	cout << "\nPP3:\n";
	PP3(root2, 0);
	dealloc2(root2);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	freeTree(root1);
	free(freed); /// free the helper array of nodes , after all the nodes were also freed
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//perf_traversals();
	//demo_traversals();
	//demo_transformari();
	return 0;
}