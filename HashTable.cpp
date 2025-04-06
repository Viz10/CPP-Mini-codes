#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <time.h>

#include "Profiler.h"

/*
For Insert,Search and Delete the best running time is O(1) constant as the key is mapped to a desired cell , no other verifications needed
the average case is O(1/1-alfa) ,where alfa represents the load factor of the array in (0,1)
the worst case is O(n) because the entire array must be checked if the table is near full or full

if the keys have a common factor and the array size is a multiple of that factor
this can result in clustering because all keys with that factor will map to a smaller range of slots => we use prime numbers as array size to ensure
no common factor can devide the two , the hash function is more likely to produce values that cover the entire range of indices.

by using open adressing and quadratic probing if the maped key slot is ocupied we traverse the array in search of the next free slot but rather more efficient
than linear probing which checks adj. cells , quadratic ensures spreading the probed slots more uniformly across the table and reducing primary clustering and colisions.

*/

using std::cout;
using std::cin;
using std::swap;
#define DELETED_NODE (Entry*)(0xFFFFFFFFFFFFUL) /// special entry pointer value

Profiler profiler("lab5");

struct Entry {
	int id;
	char name[30];
};

Entry* Entry_create(int id, const char* name) {
	Entry* p = (Entry*)malloc(sizeof(Entry));
	p->id = id;
	strncpy(p->name, name, 30);
	return p;
}

int h1(int id,int size) { /// multiplication method using Knuth`s constant
	double nr = 0.6180339887;
	double fractional = id * nr - (int)(id * nr);
	return (int)(size * fractional);
} /// id is the key and must convert into a valid index of array , with division hashing or multiplication

void Array_print(Entry**a,int size) { /// print id and name
	for (int i = 0; i < size; ++i)
	{
		cout << i << ".  ";
		if (a[i] == NULL)
			cout << "---\n";
		else if(a[i]==DELETED_NODE)
			cout << "deleted\n";
		else
			cout << a[i]->id << " " << a[i]->name << "\n";
	}
}

void Array_insert(Entry** a, int id, const char* name,int size, int* current_elements) { /// insert both id and name
	if (id < 0)
		cout << "id cannot be negative\n";
	if (id > INT_MAX)
		cout << "id too large\n";

	int key = h1(id, size); /// the key in array that the hash func in 1st instance gives us , gives us the index into array
	int try_index = 0;

	int i = 0;
	for (; i < size; ++i) {
		try_index = (key + i + i * i) % size; /// we loop through possible positions 
		if (a[try_index] == NULL || a[try_index]==DELETED_NODE) { /// valid position
			a[try_index] = Entry_create(id, name);
			(*current_elements)++;
			return;
		}
		/// else someone different is there in try_index
	}
	if (i == size) /// we got back where we started
		cout << "array is full!\n";
}

void Array_delete(Entry** a, int id,int size,int*current_elements) { /// delete by id
	int key = h1(id, size); /// where it should be first
	int try_index = 0;

	int i = 0;
	for (; i < size; ++i) {
		try_index = (key + i + i * i) % size;
		if (a[try_index] == NULL) { /// when we reach a possible valid pos that is NULL that means it should have been there => not found
			return;
		}
		else if(a[try_index] != NULL && a[try_index] != DELETED_NODE){
			if (a[try_index]->id == id) {
				Entry* aux = a[try_index];
				free(aux);
				a[try_index] = DELETED_NODE;
				(*current_elements)--;
			}
		}
	}
}

Entry* Array_search(Entry** a, int id,int size,int*operations) { /// search by id
	int key = h1(id, size);
	int try_index = 0;

	int i = 0;
	for (; i < size; ++i) {
		try_index = (key + i + i * i) % size;
		if (a[try_index] == NULL) { /// when we reach a possible valid pos that is NULL that means it should have been there => not found
			if(operations) (*operations)++;
			return NULL;
		}/// if it encoundered a deleted node , contiune
		else if (a[try_index] != NULL && a[try_index] != DELETED_NODE) { /// ocupied cell
			if (operations) (*operations)++;
			if (a[try_index]->id == id) {
				return a[try_index];
			}
		}
	}
	return NULL; /// we`ve looped through the entire array , array is full or almost
}

void Array_dealloc(Entry** a,int size) {
	for (int i = 0; i < size; ++i) {
		if(a[i]!=NULL && a[i] != DELETED_NODE)
			free(a[i]);
	}
}

void Array_set(Entry** array, int size) {
	for (Entry** a = array; a < array + size; ++a)
		*a = NULL;
}

#define local_nr 17
void demo() {

	int current_elements = 0;
	int size = local_nr;
	Entry** array=(Entry**)malloc(sizeof(Entry*)*size);

	Array_set(array, size);

	Array_insert(array, 91467, "Nenea1", size,&current_elements);
	Array_insert(array, 14267, "Nenea2", size, &current_elements);
	Array_insert(array, 100, "Nenea3", size, &current_elements);
	Array_insert(array, 184671, "Nenea4", size, &current_elements);
	Array_insert(array, 0, "Nenea5", size, &current_elements);
	Array_insert(array, 694247, "Nenea6", size, &current_elements);
	Array_insert(array, 13, "Nenea7", size, &current_elements);
	Array_insert(array, 6921467, "Nenea8", size, &current_elements);
	Array_insert(array, 1, "Nenea9", size, &current_elements);

	cout << "Array with " << current_elements << " entries inside\n";
	Array_print(array, size);

	cout << "\n";
	cout << "\n";

	Array_delete(array, 91467, size, &current_elements);
	Array_delete(array, 0, size, &current_elements);

	cout << "Array with " << current_elements << " entried inside\n";
	Array_print(array, size);

	cout << "\n";
	cout << "\n";

	Entry* rez;
	rez = Array_search(array, 184671, size,nullptr);
	if (rez)
		cout << "Found " << rez->name << "\n";
	else { cout << "Not found!\n"; }

	rez = Array_search(array, 0, size,nullptr);
	if (rez)
		cout << "Found: " << rez->name << "\n";
	else
		cout << "Not found!\n";

	Array_dealloc(array, size);
	free(array);
}

#define N 10007 /// filling factor is nr_elements_added / N , 0<nr<1
void average_search() {
	int cnt = 0;
	Entry** array = (Entry**)malloc(sizeof(Entry*) * N);


	///////////////////////////////////////////////// 80%
	Array_set(array, N);
	int elements_added1 = 0;
	double a1 = 0.80;
	int nr = (int)(a1 * N);

	int total_operations_found1 = 0;
	int max_total_operations_found1 = 0;
	int total_operations_not_found1 = 0;
	int max_total_operations_not_found1 = 0;
	for (int l = 0; l < 5; ++l) {

		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0); /// id s that will be in the array

		for (int i = 0; i < nr; ++i) {
			Array_insert(array, aux[i], "", N, &elements_added1); /// insert entities with id s
		}

		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations); /// search from those inserted (valid)
			max_total_operations_found1 = max(max_total_operations_found1, operations);
			total_operations_found1 += operations;
		}
		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations); /// search invalid ids , as 50000 is max iq , 50000+i will go past
			max_total_operations_not_found1 = max(max_total_operations_not_found1, operations);
			total_operations_not_found1 += operations;
		}


		Array_dealloc(array, N);
		free(aux);
	}
	total_operations_found1 /= 1500;
	total_operations_not_found1 /= 1500;

	max_total_operations_found1 /= 5;
	max_total_operations_not_found1 /= 5;
	total_operations_found1 /= 5;
	total_operations_not_found1 /= 5;

	cout << "Filling Factor | Avg. Effort found | Max Effort found |Avg. Effort not-found | Max Effort not-found\n";
	std::cout << std::setw(10) << a1 << std::setw(20) << total_operations_found1 << std::setw(20) << max_total_operations_found1 << std::setw(20) << total_operations_not_found1 << std::setw(20) << max_total_operations_not_found1 << "\n";

	///////////////////////////////////////////////// 85%
	Array_set(array, N);
	int elements_added2 = 0;
	double a2 = 0.85;
	nr = (int)(a2 * N);

	int total_operations_found2 = 0;
	int max_total_operations_found2 = 0;
	int total_operations_not_found2 = 0;
	int max_total_operations_not_found2 = 0;

	for (int l = 0; l < 5; ++l) {

		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0); /// id s that will be in the array

		for (int i = 0; i < nr; ++i) {
			Array_insert(array, aux[i], "", N, &elements_added1); /// insert entities with id s
		}


		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations);
			max_total_operations_found2 = max(max_total_operations_found2, operations);
			total_operations_found2 += operations;
		}
		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations);
			max_total_operations_not_found2 = max(max_total_operations_not_found2, operations);
			total_operations_not_found2 += operations;
		}


		Array_dealloc(array, N);
		free(aux);
	}
	total_operations_found2 /= 1500;
	total_operations_not_found2 /= 1500;

	max_total_operations_found2 /= 5;
	max_total_operations_not_found2 /= 5;
	total_operations_found2 /= 5;
	total_operations_not_found2 /= 5;

	std::cout << std::setw(10) << a2 << std::setw(20) << total_operations_found2 << std::setw(20) << max_total_operations_found2 << std::setw(20) << total_operations_not_found2 << std::setw(20) << max_total_operations_not_found2 << "\n";


	///////////////////////////////////////////////// 90%
	Array_set(array, N);
	int elements_added3 = 0;
	double a3 = 0.90;
	nr = (int)(a3 * N);

	int total_operations_found3 = 0;
	int max_total_operations_found3 = 0;
	int total_operations_not_found3 = 0;
	int max_total_operations_not_found3 = 0;

	for (int l = 0; l < 5; ++l) {

		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0); /// id s that will be in the array


		for (int i = 0; i < nr; ++i) {
			Array_insert(array, aux[i], "", N, &elements_added1); /// insert entities with id s
		}

		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations);
			max_total_operations_found3 = max(max_total_operations_found3, operations);
			total_operations_found3 += operations;
		}
		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations);
			max_total_operations_not_found3 = max(max_total_operations_not_found3, operations);
			total_operations_not_found3 += operations;
		}


		Array_dealloc(array, N);
		free(aux);
	}
	total_operations_found3 /= 1500;
	total_operations_not_found3 /= 1500;

	max_total_operations_found3 /= 5;
	max_total_operations_not_found3 /= 5;
	total_operations_found3 /= 5;
	total_operations_not_found3 /= 5;

	std::cout << std::setw(10) << a3 << std::setw(20) << total_operations_found3 << std::setw(20) << max_total_operations_found3 << std::setw(20) << total_operations_not_found3 << std::setw(20) << max_total_operations_not_found3 << "\n";


	///////////////////////////////////////////////// 95%
	Array_set(array, N);
	int elements_added4 = 0;
	double a4 = 0.95;
	nr = (int)(a4 * N);

	int total_operations_found4 = 0;
	int max_total_operations_found4 = 0;
	int total_operations_not_found4 = 0;
	int max_total_operations_not_found4 = 0;

	for (int l = 0; l < 5; ++l) {

		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0); /// id s that will be in the array

		for (int i = 0; i < nr; ++i) {
			Array_insert(array, aux[i], "", N, &elements_added1); /// insert entities with id s
		}


		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations);
			max_total_operations_found4 = max(max_total_operations_found4, operations);
			total_operations_found4 += operations;
		}
		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations);
			max_total_operations_not_found4 = max(max_total_operations_not_found4, operations);
			total_operations_not_found4 += operations;
		}

		Array_dealloc(array, N);
		free(aux);
	}
	total_operations_found4 /= 1500;
	total_operations_not_found4 /= 1500;

	max_total_operations_found4 /= 5;
	max_total_operations_not_found4 /= 5;
	total_operations_found4 /= 5;
	total_operations_not_found4 /= 5;
	std::cout << std::setw(10) << a4 << std::setw(20) << total_operations_found4 << std::setw(20) << max_total_operations_found4 << std::setw(20) << total_operations_not_found4 << std::setw(20) << max_total_operations_not_found4 << "\n";


	///////////////////////////////////////////////// 99%
	Array_set(array, N);
	int elements_added5 = 0;
	double a5 = 0.99;
	nr = (int)(a5 * N);

	int total_operations_found5 = 0;
	int max_total_operations_found5 = 0;
	int total_operations_not_found5 = 0;
	int max_total_operations_not_found5 = 0;

	for (int l = 0; l < 5; ++l) {

		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0); /// id s that will be in the array

		for (int i = 0; i < nr; ++i) {
			Array_insert(array, aux[i], "", N, &elements_added1); /// insert entities with id s
		}


		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations);
			max_total_operations_found5 = max(max_total_operations_found5, operations);
			total_operations_found5 += operations;
		}
		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations);
			max_total_operations_not_found5 = max(max_total_operations_not_found5, operations);
			total_operations_not_found5 += operations;
		}


		Array_dealloc(array, N);
		free(aux);
	}
	total_operations_found5 /= 1500;
	total_operations_not_found5 /= 1500;

	max_total_operations_found5 /= 5;
	max_total_operations_not_found5 /= 5;
	total_operations_found5 /= 5;
	total_operations_not_found5 /= 5;

	std::cout << std::setw(10) << a5 << std::setw(20) << total_operations_found5 << std::setw(20) << max_total_operations_found5 << std::setw(20) << total_operations_not_found5 << std::setw(20) << max_total_operations_not_found5 << "\n";

	//////////////////////////////////////////// DELETION

	Array_set(array, N);
	int elements_added = 0;
	double a = 0.99;
	nr = (int)(a * N);
	int cn = nr;
	int total_operations_found = 0;
	int max_total_operations_found = 0;
	int total_operations_not_found = 0;
	int max_total_operations_not_found = 0;

	for (int l = 0; l < 5; ++l) {
		nr = cn;
		elements_added = 0;
		Array_set(array, N);

		int* aux = (int*)malloc(sizeof(int) * nr);
		FillRandomArray(aux, nr, 0, 50000, true, 0);

		for (int i = 0; i < nr; ++i) {/// now the arrat should be 99% full
			Array_insert(array, aux[i], "", N, &elements_added);
		}
		
		while (1) {
			int poz = rand() % nr;
			Array_delete(array, aux[poz], N, &elements_added); /// delete random elements until it reaches 80% capacity
			for (int j = poz; j < nr - 1; ++j) { // delete id from aux which contains all the different ids added 
				aux[j] = aux[j + 1];
			}
			nr--;
			if (elements_added == (int)(0.8 * N)) { //// when we deleted enough entries to make the hash table 80% stop
				break;
			}
		}

		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, aux[rand() % nr], N, &operations);
			max_total_operations_found = max(max_total_operations_found, operations);
			total_operations_found += operations;
		}

		for (int i = 0; i < 1500; ++i) {
			int operations = 0;
			Array_search(array, 50001 + i, N, &operations);
			max_total_operations_not_found = max(max_total_operations_not_found, operations);
			total_operations_not_found += operations;
		}

		Array_dealloc(array, N);
		free(aux);
	}

	total_operations_found /= 1500;
	total_operations_not_found /= 1500;

	max_total_operations_found /= 5;
	max_total_operations_not_found /= 5;
	total_operations_found /= 5;
	total_operations_not_found /= 5;

	std::cout << std::setw(10) << "deleting" << std::setw(20) << total_operations_found << std::setw(20) << max_total_operations_found << std::setw(20) << total_operations_not_found << std::setw(20) << max_total_operations_not_found << "\n";

	free(array);
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(time(NULL));

	//demo();
	average_search();
	return 0;
}