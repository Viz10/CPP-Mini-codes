#include <iostream>
#include <time.h>
#include <crtdbg.h>
#include "Profiler.h"
#define _CRTDBG_MAP_ALLOC

Profiler profiler("lab7");

struct Node {
	int val;
	int size;
	struct Node* left;
	struct Node* right;
};

Node* nodecreate(int val, int size) {
	Node* aux = (Node*)malloc(sizeof(Node));
	aux->left = NULL;
	aux->right = NULL;
	aux->val = val;
	aux->size = size;
	return aux;
}

Node* BUILD_TREE(int st, int dr, Operation* build_cnt = nullptr) { // 1,2,3...n
	if (st <= dr) {
		int mij = (st + dr) / 2;/// take mid point as root hence both the left and right subtrees of any node will have approximately the same number of elements
		Node* node = nodecreate(mij, dr - st + 1);
		if (build_cnt) build_cnt->count(2);
		node->left = BUILD_TREE(st, mij - 1, build_cnt);/// now , left subtree will have nodes less than root
		node->right = BUILD_TREE(mij + 1, dr, build_cnt);/// now , right subtree will have nodes hiegher than root
		return node; //they will be perfectly balanced also
	}
	return NULL;/// no more elements to process in the subarray
}

void deleting(Node* root) { /// deallocate the whole tree
	if (root != NULL) {
		deleting(root->left);
		deleting(root->right);
		free(root);
	}
}

void inorder(Node* root) {
	if (root != NULL) {
		inorder(root->left);
		std::cout << root->val << " ";
		inorder(root->right);
	}
}

void preorder(Node* root) {
	if (root != NULL) {
		std::cout << root->val << " ";
		preorder(root->left);
		preorder(root->right);
	}
}

void print_tree(Node* root, int space)  /// fancy print,DFS approach
{
	if (root == NULL)
		return;

	space += 5; /// how many times we go on a edge downwards, increase nr of spaces
	print_tree(root->right, space);

	std::cout << "\n";
	for (int i = 5; i < space; i++)
		std::cout << " "; /// when returning from recursion , print those spaces + the current node

	std::cout << "(" << root->val << "," << root->size << ")\n"; /// process node

	print_tree(root->left, space); 
}

Node* OS_SELECT(Node* root, int i, Operation* select_cnt = nullptr) {
	if (root == NULL) 
		return NULL;
		
	int rank = (root->left != NULL ? root->left->size : 0) + 1; /// if non-leaf its rank is is the nr of nodes before it(on its left) + itself
	if (select_cnt) select_cnt->count();
	/// if leaf , rank is 1
	if (rank == i) {
		if (select_cnt) select_cnt->count();
		return root; /// found
	}
	else {
		if (select_cnt) select_cnt->count();
		if (i < rank) {
			OS_SELECT(root->left, i, select_cnt); /// must be found in left subtree , with nodes smaller
		}
		else /// val is higher than rank , search the right subtree for the (val-rank)th element
			OS_SELECT(root->right, i - rank, select_cnt);
	}
}

Node* search(Node* root, int val, Operation* deletion_cnt = nullptr) {
	if (root == NULL)
		return NULL;

	if (root->val == val) {
		if (deletion_cnt) deletion_cnt->count();
		return root;
	}
	else if (root->val > val) {
		if (deletion_cnt) deletion_cnt->count();
		return search(root->left, val,deletion_cnt);

	}
	else if (root->val < val) {
		if (deletion_cnt) deletion_cnt->count();
		return search(root->right, val, deletion_cnt);
	}

}

/// there is only need to find the succesor/predecessor in the subtrees because while performing a deletion , all the nodes dont require the succesor/predecessor
/// which resides as one of an ancestor , as a normal succesor/predecessor funtion would also return

Node* depth_succesor(Node* root, int val, Operation* deletion_cnt = nullptr) {
	if (root == NULL) return NULL;

	Node* searched = search(root, val); /// get node whose succesor mus be found
	if (deletion_cnt) deletion_cnt->count(2);
	if (searched == NULL)
		return NULL;

	if (searched->right != NULL) /// if there exists a right subtree , succesor is the minimum there
	{
		if (deletion_cnt) deletion_cnt->count(2);
		Node* rez = searched->right;
		while (rez->left != NULL) {
			if (deletion_cnt) deletion_cnt->count();
			rez = rez->left;
		}
		return rez;
	}
}

Node* depth_predecesor(Node* root, int val,Operation* deletion_cnt = nullptr) {
	if (root == NULL) return NULL;

	if (deletion_cnt) deletion_cnt->count(2);
	Node* searched = search(root, val); /// get node whose predecessor mus be found
	if (searched == NULL)
		return NULL;

	if (searched->left != NULL) /// if there exists a left subtree , succesor is the max there
	{
		if (deletion_cnt) deletion_cnt->count(2);
		Node* rez = searched->left;
		while (rez->right != NULL) {
			if (deletion_cnt) deletion_cnt->count();
			rez = rez->right;
		}
		return rez;
	}
}

void OS_DELETE(Node*& root, int val, Operation* deletion_cnt = nullptr) {
	if (root == NULL) {
		return;
	}
	else if (root->val > val) { // check left subtree
		root->size--; /// recalivrate size , each node traversed will have 1 less 
		if (deletion_cnt) deletion_cnt->count(2);
		OS_DELETE(root->left, val, deletion_cnt);
	}
	else if (root->val < val) { // check right subtree
		root->size--;
		if (deletion_cnt) deletion_cnt->count(2);
		OS_DELETE(root->right, val, deletion_cnt);
	}
	else { // found it!
		if (root->left == NULL && root->right == NULL) {
			if (deletion_cnt) deletion_cnt->count();
			root->size--;
			free(root);
			root = NULL;
		}
		else if (root->right != NULL) { // there is a right child or both children, exchange with successor
			if (deletion_cnt) deletion_cnt->count(2);
			Node* aux = depth_succesor(root, root->val);
			root->val = aux->val; // change it with successor
			root->size--;
			if (deletion_cnt) deletion_cnt->count(2);
			OS_DELETE(root->right, aux->val, deletion_cnt); // delete successor
		}
		else { // there is just a left child
			if (deletion_cnt) deletion_cnt->count(2);
			Node* aux = depth_predecesor(root, root->val);
			root->val = aux->val;
			root->size--;
			if (deletion_cnt) deletion_cnt->count(2);
			OS_DELETE(root->left, aux->val, deletion_cnt);
		}
	}
}

void perf() {

	for (int l = 0; l < 5; ++l)
		for (int n = 100; n <= 10000; n = n + 100)
		{
			Operation build = profiler.createOperation("CNT_BUILD", n);
			Operation select = profiler.createOperation("CNT_SELECT", n);
			Operation deletion = profiler.createOperation("CNT_DELETING", n);

			Node* root = BUILD_TREE(1, n, &build);
			
			for (int i = 0; i < n; ++i) { /// n times perform os_select and deletion on random chosen node number
				
				int value = OS_SELECT(root, rand()%(n-i)+1, &select)->val; /// decrement n after each selection 
				OS_DELETE(root, value,&deletion); /// delete th i-th smallest element

			}
			deleting(root);
		}
		
	profiler.divideValues("CNT_BUILD", 5);
	profiler.divideValues("CNT_SELECT", 5);
	profiler.divideValues("CNT_DELETING", 5);
	profiler.createGroup("Operations", "CNT_BUILD", "CNT_SELECT", "CNT_DELETING");

	profiler.showReport();
}

#define N 11
void demo() {
	srand(time(NULL));
	Node* root = BUILD_TREE(1, N);

	std::cout << "BASE TREE AND TRAVERSALS:\n";
	print_tree(root, 0);
	std::cout << "\ninorder:  ";
	inorder(root);
	std::cout << "\npreorder: ";
	preorder(root);
	std::cout << "\n";

	int cn = N;
	int*a = (int*)calloc((cn + 1),sizeof(int));
	for (int i = 1; i <= N; ++i)
	{
		a[i] = i;
	}
	for (int i = 0; i < 3; ++i) {

		int rez = rand() % cn + 1; /// select a random i-th index
		int index = a[rez];

		std::cout<<"\nthe "<<index<<" smallest element is: "<< OS_SELECT(root, index)->val << "";
	
		for (int j = rez; j < cn - 1; ++j) {
			a[j] = a[j + 1]; /// must choose another valid distinct index
		}
		cn--;
	}
	free(a);
	std::cout << "\n\n----------------------------------------------\n";
	
	for (int i = 0; i < 3; ++i) { 

		int index = rand() % (N - i) + 1;
		int value = OS_SELECT(root, index)->val;
		std::cout << "\nthe " << index << " smallest element is: " << value;
		std::cout << "\nafter deleting " << value <<":";
		OS_DELETE(root,value); /// delete th i-th smallest element
		std::cout << "\n";
		inorder(root);
		std::cout << "\n";
		preorder(root);
		std::cout << "\n";
		print_tree(root, 0);
		std::cout << "\n";
	}

	if (root == NULL)
		std::cout << "empty tree\n";

	deleting(root); 
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	demo();
	//perf();
	return 0;
}