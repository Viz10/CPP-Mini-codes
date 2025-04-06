#include <iostream>
#include <stdio.h> 
#include "Profiler.h"

/*
For bubble sort iterative vs recursive the nr of operations between the 2 is aprox equal to each other , however
the runtime of the recursive one is higher that the iterative.This happens due to the fact that it uses the stack memory for each 
recursion  hence adding overhead to the stack memory and for deep recursion it can lead to nested calls.

for heap:
shifting an element upwards or downwards takes logn time , heap is a binary complete tree with height logn 
and the node shifts every time 1 level in the heap

for building a heap:

Heapify: as on each level there are 1,n/8,n/4 nd on the last level at most n/2 nodes
the cost of shifting down a node is proportional with the current level so the last ones require 0 , the above only 1 and so on
it works best because the last nodes (precisely half of the total ) are leafs  and require no correcion to heapify (only from the last non leaf must check corectness of subtree) and the one at top are fewer
the cost of heapify is the nr of nodes on the level * their cost to shift down => O(n)

Top up : start with empty heap , add nodes and if needed ,shift up the larger ones , each shift up takes on average logn and must be done n times => O(nlogn)

Heapsort:it requires 2 stept one , to build the heap and 2 to extract the last element
 extraction can be done by swapping the last one and the 1st one , then truncating the util heap margin , and so until the heap becomes emptyon placing element by element
 in the remaining last positions , there are n times nr of swaps and logn shift downs to maintain heap property => O(nlogn) time complexity

 GRAPHS:
 nr op operation for both bubble sorts are almost equal , as expected but the recursive one runtime exceedes the iterative one for higher input sizes more and more
 in the average case the nr of operation for top down and buttom up creation are slightly near in the beginning but for higher sized of input the difference is segnificantly bigger
 in the worst case when the array is sorted , every single element must be shifted up , except the root for top down approach , and from the 1st non leaf node also for heapify , downwards
 the buttom up (heapify) works extremly well compared to buttom up , from the graphs the nr of asg and comp being very low in comparison , especially with big input arrays
*/

namespace lab02 {

	Profiler profiler("Lab2");

	void bubble_sort_recursiv(int* a, int i, int n, Operation &cmp_bubble_recursiv, Operation & asg_bubble_recursiv) {
		if (i == n)
			return;
		else {
			for (int j = 0; j < n - i - 1; ++j)
			{
				cmp_bubble_recursiv.count();
				if (a[j] > a[j + 1])
				{
					std::swap(a[j], a[j + 1]);
					asg_bubble_recursiv.count(3);
				}
			}
		}
		bubble_sort_recursiv(a, i + 1, n,cmp_bubble_recursiv,asg_bubble_recursiv);
	}


	void wrapper_recursiv(int* a, int n) {

		Operation cmp_bubble_recursiv = profiler.createOperation("CMP_BUBBLE_RECURSIV", n);
		Operation asg_bubble_recursiv= profiler.createOperation("ASG_BUBBLE_RECURSIV", n);
		bubble_sort_recursiv(a, 0, n, cmp_bubble_recursiv, asg_bubble_recursiv);

	}


	void bubble_sort_iterativ(int* values, int n)
	{
		if (n == 0) {
			printf("array has 0 elements!\n");
			return;
		}

		Operation cmp_bubble_iterativ = profiler.createOperation("CMP_BUBBLE_ITERATIV", n);
		Operation asg_bubble_iterativ = profiler.createOperation("ASG_BUBBLE_ITERATIV", n);

		int ok = 0, i = 0;

		do {
			ok = 0;
			for (int j = 0; j < n - i - 1; ++j) { /// after 1st iteration max is last then so on , no need to traverse and swap until end of array

				cmp_bubble_iterativ.count();

				if (values[j] > values[j + 1]) { /// swap adj element if one of beiiger then the other , such way the max from the current arr will get to the end

					asg_bubble_iterativ.count(3);
					std::swap(values[j + 1], values[j]);
					ok = 1;
				}
			}

			i++;
		} while (ok); /// when no swaps happend , exit 
	}


	void perf_bubble() {
		
		int* a = (int*)malloc(sizeof(int) * 10000);
		if (a == NULL) exit(10);

		int* cp = (int*)malloc(sizeof(int) * 10000);
		if (cp == NULL) exit(11);

		for (int l = 0; l < 5; ++l) {
			for (int n = 100; n <= 10000; n += 100) {

				FillRandomArray(a, n, 10, 50000, false);

				memcpy(cp, a, n * sizeof(int));
				bubble_sort_iterativ(a, n);

				memcpy(a, cp, n * sizeof(int));
				wrapper_recursiv(a, n);

			}
		}

		profiler.divideValues("CMP_BUBBLE_ITERATIV", 5);
		profiler.divideValues("ASG_BUBBLE_ITERATIV", 5);

		profiler.divideValues("CMP_BUBBLE_RECURSIV", 5);
		profiler.divideValues("ASG_BUBBLE_RECURSIV", 5);

		profiler.addSeries("OPERATII_BUBBLE_ITERATIV", "CMP_BUBBLE_ITERATIV", "ASG_BUBBLE_ITERATIV");
		profiler.addSeries("OPERATII_BUBBLE_RECURSIV", "CMP_BUBBLE_RECURSIV", "ASG_BUBBLE_RECURSIV");

		profiler.createGroup("COMPARATII_BUBBLE_SORT", "CMP_BUBBLE_ITERATIV", "CMP_BUBBLE_RECURSIV");
		profiler.createGroup("ASIGNARI_BUBBLE_SORT", "ASG_BUBBLE_ITERATIV", "ASG_BUBBLE_RECURSIV");
		profiler.createGroup("NR_OF_OPERATIONS_BUBBLE_SORT", "OPERATII_BUBBLE_ITERATIV", "OPERATII_BUBBLE_RECURSIV");
		
		free(a);
		free(cp);
		
		int* x = (int*)malloc(sizeof(int) * 2000);
		if (x == NULL) exit(10);

		int* y = (int*)malloc(sizeof(int) * 2000);
		if (y == NULL) exit(11);

		for (int n = 100; n <= 2000; n+=100) {

			FillRandomArray(x, n, 10, 50000, false);
			memcpy(y, x, n * sizeof(int));

			profiler.startTimer("bubble_sort_iterativ", n);
			for (int j = 0; j < 1000; ++j) {
				memcpy(x, y, n * sizeof(int));
				bubble_sort_iterativ(x, n);
			}
			profiler.stopTimer("bubble_sort_iterativ", n);

			profiler.startTimer("bubble_sort_recursiv", n);
			for (int j = 0; j < 1000; ++j) {
				memcpy(x, y, n * sizeof(int));
				wrapper_recursiv(x, n);
			}
			profiler.stopTimer("bubble_sort_recursiv", n);
		}
		
		profiler.createGroup("runtime", "bubble_sort_iterativ", "bubble_sort_recursiv");
		
		free(x);
		free(y);

		profiler.showReport();
	}

	/////////////////////////////////////////////////////////////////////////////// HEAP


	void bring_up(int* a, int poz, Operation &cmp_TOP_DOWN, Operation &asg_TOP_DOWN) {///O(logn)

		int i = poz; /// i hold the current element poz which may go up
	
		if (i < 1)
			return;

		int parent_index = (poz - 1) / 2; /// compare with parent 

		while (i > 0 && a[parent_index] < a[i]) { /// if the element has become the top :exit
			cmp_TOP_DOWN.count();

			std::swap(a[parent_index], a[i]);
			asg_TOP_DOWN.count(3);

			i = parent_index; /// prepare for next comparison of the element
			parent_index = (i - 1) / 2;
		}
		if (i > 0) { /// if parent is higher,doesnt enter loop but we made a comp.
			cmp_TOP_DOWN.count();
		}
	}


	void create_top_down(int* array, int n) {///O(nlogn)

		if (n == 0) {
			printf("array has 0 elements!\n");
			return;
		}

		Operation cmp_TOP_DOWN = profiler.createOperation("CMP_TOP_DOWN", n);
		Operation asg_TOP_DOWN = profiler.createOperation("ASG_TOP_DOWN", n);

		for (int i = 1; i < n; ++i) {
			bring_up(array, i, cmp_TOP_DOWN, asg_TOP_DOWN); /// take "insert" each element from given array and shift up if needed 
		}
	}


	void bring_down(int* a, int poz, int n, Operation& cmp_BUTTOM_UP, Operation& asg_BUTTOM_UP) { ///O(logn)

		--n;   /// n is nr of elements and as it is from 0,  --n to get n be the last poz

		int max_poz = 0;
		int st = 2 * poz + 1; /// get children
		int dr = 2 * poz + 2;

		if (st > n) /// element is leaf , out of margin
			return;

		if (dr > n) /// if there is no right node (out of margin) , get left
			max_poz = st;
		else {
			cmp_BUTTOM_UP.count();
			if (a[st] > a[dr]) /// compare with max of children
				max_poz = st;
			else
				max_poz = dr;
		}


		while (a[poz] < a[max_poz]) { /// while current element is less then lower situated  
			cmp_BUTTOM_UP.count();

			std::swap(a[max_poz], a[poz]);
			asg_BUTTOM_UP.count(3);

			poz = max_poz;
			st = 2 * poz + 1;
			dr = 2 * poz + 2;

			if (st > n) ///  nodes are from left to right , if left doesnt exist that means its a leaf and we stop
				break;

			if (dr > n)
				max_poz = st;
			else {
				cmp_BUTTOM_UP.count();
				if (a[st] > a[dr]) /// prepare for next possible lower swap
					max_poz = st;
				else
					max_poz = dr;
			}
		}
	}

	
	void heapify(int* a, int n) { /// create buttom up O(n)

		if (n == 0) {
			printf("array has 0 elements!\n");
			return;
		}

		Operation cmp_BUTTOM_UP = profiler.createOperation("CMP_BUTTOM_UP", n);
		Operation asg_BUTTOM_UP = profiler.createOperation("ASG_BUTTOM_UP", n);

		for (int i = (n - 1) / 2; i >= 0; --i) /// start from the 1st node that has child , the before ones are leafs 
			bring_down(a, i, n,cmp_BUTTOM_UP,asg_BUTTOM_UP);
	}


	void bring_down_for_sort(int* a, int poz, int n) { /// same exact function , but for Heap Sort , without profiler operations

		--n;

		int max_poz = 0;
		int st = 2 * poz + 1;
		int dr = 2 * poz + 2;

		if (st > n)
			return;

		if (dr > n)
			max_poz = st;
		else {
			if (a[st] > a[dr])
				max_poz = st;
			else
				max_poz = dr;
		}

		while (a[poz] < a[max_poz]) {

			std::swap(a[max_poz], a[poz]);

			poz = max_poz;
			st = 2 * poz + 1;
			dr = 2 * poz + 2;

			if (st > n)
				break;

			if (dr > n)
				max_poz = st;
			else {
				if (a[st] > a[dr])
					max_poz = st;
				else
					max_poz = dr;
			}
		}
	}


	void heap_sort(int* arr, int n) { /// swap root with last element from the heap , then bring down the lower new root O(nlogn) because n times moves root downwards 

		if (n == 0) {
			printf("array has 0 elements!\n");
			return;
		}

		int last_poz = n - 1; /// current last poz 
		while (last_poz > 0) { /// when there is only one element , stop
			std::swap(arr[0], arr[last_poz]);
			last_poz--; /// resize array , decrese heap margin , like deleting the node 
			bring_down_for_sort(arr, 0, last_poz + 1); /// +1 because in the bring_downwards there already exists an decrement 
		}
	}


	bool verify_heap(int* a, int n) /// checks if given array is a heap max
	{ 

		if (n == 0) {
			printf("array has 0 elements!\n");
			return false;
		}

		int parent_poz = 0;
		for (int i = 1; i < n; ++i)
		{
			parent_poz = (i - 1) / 2;
			if (a[parent_poz] < a[i]) /// heap condition broken
				return false;
		}
		return true;
	}

	bool Sorted(int* arr, int size)
	{
		if (size == 0) {
			printf("array has 0 elements!\n");
			return false;
		}

		for (int i = 0; i < size - 1; ++i) {
			if (arr[i + 1] < arr[i]) {
				return false;
			}
		}
		return true;
	}

	void test_cases() 
	{
		int a[100], cpy[100];

		FillRandomArray(a, 100);
		memcpy(cpy, a, 100 * 4);

		create_top_down(a, 100);
		if (!verify_heap(a, 100))
			exit(1);

		memcpy(a, cpy, 100 * 4);
		heapify(a, 100);
		if (!verify_heap(a, 100))
			exit(2);

		heap_sort(a, 100); /// assume a is heapified by previous line
		if (!Sorted(a, 100))
			exit(3);


		////////////////////////////////////////////////////////////////////////

		FillRandomArray(a, 100, 10, 50000, true, ASCENDING);
		memcpy(cpy, a, 100 * 4);

		create_top_down(a, 100);
		if (!verify_heap(a, 100))
			exit(4);

		memcpy(a, cpy, 100 * 4);
		heapify(a, 100);
		if (!verify_heap(a, 100))
			exit(5);

		heap_sort(a, 100);
		if (!Sorted(a, 100))
			exit(6);

		//////////////////////////////////////////////////////////////////////

		FillRandomArray(a, 100, 10, 50000, false, DESCENDING);
		memcpy(cpy, a, 100 * 4);

		create_top_down(a, 100);
		if (!verify_heap(a, 100))
			exit(7);

		memcpy(a, cpy, 100 * 4);
		heapify(a, 100);
		if (!verify_heap(a, 100))
			exit(8);

		heap_sort(a, 100);
		if (!Sorted(a, 100))
			exit(9);

		std::cout << "succeded sortings!" << "\n";
	}

	void demonstrate_top_down()
	{
	int up_down[] = { 10,40,25,12,20,15,18 };
	int n1 = sizeof(up_down)/sizeof(int);

	create_top_down(up_down, n1);

	if (!verify_heap(up_down, n1))
		std::cout << "not a heap\n";

	for (int i = 0; i < n1;++i)
		std::cout << up_down[i] << " ";

	}

	void demonstrate_buttom_up()
	{
	int down_up[] = { 10,20,15,30,40 };
	int n2 = sizeof(down_up)/sizeof(int);

	heapify(down_up, n2);

	if (!verify_heap(down_up, n2))
		std::cout << "not a heap\n";

	for (int i = 0; i < n2;++i)
		std::cout << down_up[i] << " ";

	}

	void demonstrate_heapSort() 
	{
	int random_array[] = { -1000,2,1,3,4,-2,5,-8,7,-10,9,-55,1,222,-1000,-55,10,11 };
	int n_random = sizeof(random_array)/sizeof(int);

	heapify(random_array, n_random);
	if (!verify_heap(random_array, n_random))
		std::cout << "not a heap\n";
	
	heap_sort(random_array, n_random);
	if (!Sorted(random_array, n_random))
		std::cout << "not sorted\n";

	for (int i = 0; i < n_random;++i)
		std::cout << random_array[i] << " ";
	}


	void performance(const char*choice) {

		int* arr = (int*)malloc(sizeof(int) * 10000);
		if (arr == NULL) exit(10);

		int* cpy = (int*)malloc(sizeof(int) * 10000);
		if (cpy == NULL) exit(11);

		if (strcmp(choice, "worst") == 0) {

			for (int n = 100; n <= 10000; n += 100) {
				FillRandomArray(arr, n, 10, 50000, false, ASCENDING);
				memcpy(cpy, arr, n*sizeof(int));
					
				create_top_down(arr, n);

				memcpy(arr, cpy, n * sizeof(int));

				heapify(arr, n);
			}

			profiler.addSeries("OPERATII_BUTTOM_UP", "CMP_BUTTOM_UP", "ASG_BUTTOM_UP");
			profiler.addSeries("OPERATII_TOP_DOWN", "CMP_TOP_DOWN", "ASG_TOP_DOWN");

			profiler.createGroup("COMPARATII_WORST", "CMP_BUTTOM_UP", "CMP_TOP_DOWN");
			profiler.createGroup("ASIGNARI_WORST", "ASG_BUTTOM_UP", "ASG_TOP_DOWN");
			profiler.createGroup("OPERATII_WORST", "OPERATII_BUTTOM_UP", "OPERATII_TOP_DOWN");

			profiler.showReport();

		}
		else if (strcmp(choice, "average") == 0) {

			for (int i = 0; i < 5; ++i) {
				for (int n = 100; n <= 10000; n += 100) {
					FillRandomArray(arr, n, 10, 50000, false);
					memcpy(cpy, arr, n * sizeof(int));

					create_top_down(arr, n);

					memcpy(arr, cpy, n * sizeof(int));

					heapify(arr, n);
				}
			}

			profiler.divideValues("CMP_BUTTOM_UP", 5);
			profiler.divideValues("ASG_BUTTOM_UP", 5);
			profiler.divideValues("CMP_TOP_DOWN", 5);
			profiler.divideValues("ASG_TOP_DOWN", 5);

			profiler.addSeries("OPERATII_BUTTOM_UP", "CMP_BUTTOM_UP", "ASG_BUTTOM_UP");
			profiler.addSeries("OPERATII_TOP_DOWN", "CMP_TOP_DOWN", "ASG_TOP_DOWN");

			profiler.createGroup("COMPARATII_AVERAGE", "CMP_BUTTOM_UP", "CMP_TOP_DOWN");
			profiler.createGroup("ASIGNARI_AVERAGE", "ASG_BUTTOM_UP", "ASG_TOP_DOWN");
			profiler.createGroup("OPERATII_AVERAGE", "OPERATII_BUTTOM_UP", "OPERATII_TOP_DOWN");

			profiler.showReport();

		}
		else printf("wrong input!\n");

		free(arr);
		free(cpy);
	}


	void perf_heap() {

		printf("which case to select: worst,average\n");
		char a[20];
		fgets(a, 20, stdin);
		a[strlen(a) - 1] = 0;
		performance(a);

	}

}

/// st = 2*i + 1
/// dr= 2*i + 2;
/// parent = (i-1)/2

int main() {

	//lab02::test_cases();
	//lab02::demonstrate_top_down();
	//lab02::demonstrate_buttom_up();
	//lab02::demonstrate_heapSort();

	//lab02::perf_heap();
	lab02::perf_bubble();

return 0;

}