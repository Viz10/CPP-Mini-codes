#include "QS.h"

static int partition(Edge** EdgeArray, int st, int dr) {

	int i = st, j = dr;

	int random_index = st + (rand() % (dr - st + 1)); /// get randomly an index for pivot

	if (random_index != dr) std::swap(EdgeArray[random_index], EdgeArray[dr]); /// send it to the end

	Edge* pivot = EdgeArray[dr];

	while (i < j) {

		while (i < j && EdgeArray[i]->val <= pivot->val) {
			i++;
		}

		while (i < j && EdgeArray[j]->val >= pivot->val) {
			j--;
		}	

		if (i < j) {
			std::swap(EdgeArray[i], EdgeArray[j]);
		}

	}
	std::swap(EdgeArray[dr], EdgeArray[i]);
	return i;
}

static void Q_S(Edge** EdgeArray, int st, int dr) {
	if (st < dr) 
	{
		int pivot = partition(EdgeArray, st, dr); 
		Q_S(EdgeArray, st, pivot - 1);
		Q_S(EdgeArray, pivot + 1, dr);
	}
}

void Q_S(Edge** EdgeArray, int e) {
	/// wrapper function for QS 
	Q_S(EdgeArray, 0, e - 1);
}