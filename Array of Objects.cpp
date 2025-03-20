#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>

class MyClass {
	int a;
	int size;
	int* array;

public:

	using Iterator = int;

	MyClass() { /// base constructor
		a = 0;
		size = 0;
		array = nullptr;
		std::cout << "Base Constructed: " << a << std::endl;
	}

	MyClass(int a, int n):a(a),size(n),array(new int[size]{a}) /// parameter constructor
	{
		std::cout << "Parameter Constructed: " << a << std::endl;
	}

	MyClass(const MyClass& other) : a(other.a), size(other.size), array(new int[other.size]) /// copy constructor
	{  
		memcpy(array,other.array, other.size *4);
		std::cout << "Copy Constructed: " << a << std::endl;
	}

	MyClass(MyClass&& other) noexcept : a(other.a), size(other.size), array(other.array)  /// move constructor
	{
	other.array = nullptr; std::cout << "Move Constructed: " << a << std::endl;
	}

	MyClass& operator=(const MyClass& other) noexcept
	{
		if (&other != this) {
			delete[] array;
			a = other.a;
			size = other.size;
			array = new int[other.size];
			memcpy(array, other.array, other.size * 4);
			std::cout << "Copy Assigned: " << a << std::endl;
		}
		return *this;
	}

	MyClass& operator=(MyClass&& other) noexcept  /// move assigment
	{
		if (&other != this) {
			delete [] array;
			a=other.a;
			size=other.size;
			array = other.array;
			other.array = nullptr;
			std::cout << "Move Assigned: " << a << std::endl;
		}
		return *this;
	}

	void print() {
		for (int i = 0; i < size; ++i)
			std::cout << array[i] << " ";
		std::cout << "\n";
	}

	void printA() {
		std::cout <<a<<"\n";
	}

	~MyClass() {
		delete[] array;
	}
};

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	

	////////////////////////////////////////////////////// STATIC ,ARRAY DE POINTERI DE OBJ SAU OBJ

	///CASE1
	MyClass Array1[3]; /// static array of obj : creates in memory 3 obj which default constr is executed

	std::cout << "Array1:\n";
	for (auto&val : Array1) {
		val.printA();
	}

	///CASE2
	std::cout << "\n";
	MyClass Array2[3]{MyClass(1,2),MyClass(4,5)}; //Array2[0]...etc are directly constructed using MyClass(1, 2)...etc initialize with constructor calls inside memory
	std::cout << "Array2:\n";
	for (auto&val : Array2) { /// daca nu e & , face un obj copy pe care il printeaza
		val.printA();
	}

	std::cout << "\n=\n";
	/// IMPORTAT : WHEN USING "=" , THE LEFT MUST ALREADY BE INITIALIZED!!
	Array2[0] = MyClass(5, 6); /// constr rvalue MyClass(5, 6) ,  move it inside Array2[0]
	Array1[1] = MyClass(8, 9);
	std::cout << "\n\n";

	MyClass A(10, 11);   /// constr lvalue MyClass(10, 11) , copy it inside Array2[1]
	MyClass B(12, 13);
	MyClass*C= new MyClass(15, 16);
	Array2[1] = A;
	Array1[2] = *C;
	Array1[0] = std::move(B); /// move

	///CASE 3
	std::cout << "\n"; /// in memory construct the obj based on list
	MyClass Array3[4]{ *C,std::move(A),MyClass(20, 21) }; //Copy Constructed: 15, Move Constructed : 10, Constructed : 20, Base Constructed : 0
	for (auto&val : Array3) { /// vine compilatorul si pune MyClass(*C) , 
		val.printA();
	}

	std::cout << "\n";
	
	MyClass* Array9[3]; /// garbage values pointers
	for (MyClass* val : Array9) {
		std::cout << val<<"\n";
	}
	
	std::cout << "\n";
	///CASE 4 array of pointers to classes
	MyClass* Array4[3]{new MyClass(24,25),new MyClass(),C}; ///Constructed: 24 ,Base Constructed : 0 , si C e deja ptr ia numa adresa
	for (MyClass* val : Array4) {
		val->printA();
	}

	delete Array4[2];
	Array4[2] = new MyClass(30, 31);

	for (MyClass* val : Array4) {
		delete val;
	}

	////////////////////////////////////////////////////// Dynamic ,ARRAY DE POINTERI DE OBJ SAU OBJ
	std::cout << "\n\nDynamic1\n";
	/// CASE 1 , ARR OF OBJ ALC DINAMIC
	MyClass* AAA = new MyClass[3]; /// new calls constr for entire array
	for (int i = 0; i < 3;++i) {
		AAA[i].printA();
	}

	std::cout << "\n\nDynamic2\n";
	/// CASE 2 , ARR DE PTR DE OBJ ALOCAT DINAMIC
	MyClass** dyn1 = new MyClass * [5]; /// as they are ptrs , no constr called
	dyn1[0] = new MyClass(30, 31);
	dyn1[0]->printA();

	//MyClass::Iterator a;
	//a = 7;
	//std::cout << a;

	delete dyn1[0];
	delete [] dyn1;
	delete [] AAA;

	std::cout << "\n\nDynamic3\n";
	MyClass* tr = (MyClass*)::operator new(10 * sizeof(MyClass)); /// as malloc alloc raw mem without calling constr
	//MyClass* tr = (MyClass*)calloc(10, sizeof(MyClass)); /// 0s
	for (int i = 0; i < 3; ++i) {
		/// tr[i] = MyClass(1,2);  crash , tr[i] doesnt have a class inside to do copy assg
		new(tr + i) MyClass(i, 1); /// constr in place , already raw memory
		tr[i].printA();
	}
	for (int i = 0; i < 3; ++i) tr[i].~MyClass(); /// manually
	::operator delete(tr,10 * sizeof(MyClass)); /// none destructor call
	
	return 0;
}