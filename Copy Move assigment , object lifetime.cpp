#include <iostream>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

class Array1 {

private:
	int size;
	int* array;
public:
	
	/// constr base allocation
	Array1(int size=0):size(size),array(new int[size]{0}) { printf("\nconstr  executed %d\n",this->size); }
	
	/// copy constr used for init with another built class , pass by value , return temp class into new init class
	Array1(const Array1& other) : size(other.size), array(new int[other.size]) {
		memcpy(array, other.array, sizeof(int) * other.size);
		printf("\nconstr copy executed %d\n", this->size);
	}
	
	/// move constr used for getting atributes from one class to another leaving the right one empty
	Array1(Array1&& other) noexcept : size(other.size) {
		array = other.array;
		other.size = 0;
		other.array = nullptr;
		printf("\nconstr move executed ");
	}

	void print() const{
		for (int i = 0; i < size; ++i) {
			std::cout << array[i] << " ";
		}
		std::cout << "\n";

	}

	void set(int a) {
		for (int i = 0; i < size; ++i)
			array[i] = a;
	}

	~Array1() {
		if(array)
		delete [] array;
	}

	Array1 operator+(const Array1& other) {
		Array1 aux(size+other.size);

		for (int i = 0; i < size; ++i)
			aux.array[i] = array[i];

		for(int j=0;j<other.size;++j)
			aux.array[size+j] = other.array[j];

		return aux;
	}

	/*
	h=f=g 
	 Essentially: MyClass& x = (f = g); // f = g is now an alias for f after the function changed f , assigned to x => h=x 
	*/
	Array1& operator=(const Array1& other) {
		printf("\nassigment executed ");
		if (this != &other) {
			
				delete[] array;
				array = new int[other.size];
				memcpy(array, other.array, other.size * 4);
				size = other.size;
				
		}
		return *this;
	}
	Array1& operator=(Array1&& other) noexcept {
		printf("\nassigment move executed ");
		if (this != &other) {
			
				delete[] array;
				size = other.size;
				array = other.array;
				other.array = nullptr;
		}
		return *this;
	}

	friend void print_value(Array1 arr);
	friend void print_reff(Array1&arr);
	friend void print_pointer(Array1* arr);
};

void print_value(Array1 arr) { /// pass by copy (value) constr copy is executed
	for (int i = 0; i < arr.size; ++i) {
		std::cout << arr.array[i] << " ";
	}
	std::cout << "\n";
}

void print_reff(Array1& arr) { /// pass by reff constr copy is not executed
	for (int i = 0; i < arr.size; ++i) {
		std::cout << arr.array[i] << " ";
	}
	std::cout << "\n";
}

void print_pointer(Array1* arr) { /// pass by pointer constr copy is not executed
	for (int i = 0; i < arr->size; ++i) {
		std::cout << arr->array[i] << " ";
	}
	std::cout << "\n";
}

Array1 createObject1(int value,Array1*&n) { /// n is ref to pointer in main for clean-up
    Array1* temp = new Array1(value);
	temp->set(1);
	n = temp;
    //return std::move(*temp); // This would invoke the copy constructor/move constr *temp e salvat ca un fresh temp in memorie sa fie asignat
	return *temp;
}

//Without RVO : Copy constructor is called twice—once for the temporary rvalue(return) and once for obj2(main).

Array1 createObject2(int value) {
    Array1 temp(value);
    //return std::move(temp); // This would invoke the move constructor for return variable who gets it
    return temp; //rvo
}
/*
Since *temp is an lvalue expression (it's the dereferenced object),
returning it requires either a copy constructor or a move constructor (if RVO does not apply).
*/

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Array1 a(10);
	a.set(1);
	printf("array a:\n");
	a.print();

	Array1*b=new Array1(10); /// b created with base constructor 
	b->set(2);
	printf("\narray b:\n");
	print_value(*b);

	Array1* c = new Array1(a); /// copy constr
	printf("\narray c:\n");
	print_reff(*c);

	Array1* d = new Array1(*c);
	printf("\narray d:\n");
	print_pointer(d);


	std::cout << "for e\n";
	Array1* n;
	Array1 e = createObject1(5,n); /// here constr in funct + copy constr is used for temp val (lvalue returned as rval), + rvo creates it in func 
	//Array1 e = std::move(createObject1(5,n)); /// constr in fc + move constr is used of rvalue returned
	//Array1 e = createObject2(5); // RVO by compiler , only normal constr is executed 
	//Array1 e = Array1(6); // same as above constr 6
	//Array1 e = std::move(createObject2(5)); // this calls constr move because I cast it, I force it into rvalue so that RVO does not intervene and constr from the function is still called, copy temp no longer does it directly does constr move in memory
	printf("\n");
	e.print();
	printf("\n");
	//delete n;

	printf("\n|||||||||||||||||||||||||||||||||||||||||||||\n");


	/*
	When you write Array1 aux = *c + *b; ,
	if RVO occurs, aux is constructed directly in place, which may not invoke the move constructor.
	If RVO was not applied, the copy constructor is invoked again to initialize obj2 with the temporary object created during the return.
	*/
	printf("for aux:\n");
	Array1 aux=std::move(a); /// constr move executed
	//Array1 aux=a; /// constr copy executed
	printf("\n");
	aux.print();
	aux = *c + *b; constr in op. overload func + asg move
	//aux = *c; ///normal assigment 
	//aux = std::move(*c); /// assigment move 
	printf("\n");
	aux.print();
	a.print();
	printf("\n");

	/*
	In the assignment aux = *c + *b :
	the temporary returned from operator+ is assigned to aux, which triggers the assignment operator.
	If the assignment operator is defined to use move semantics correctly,
	it should use the move assignment operator if applicable.
	*/


	printf("for x:\n");
	Array1 x(10); /// constr
	Array1* m;
	printf("here:\n");
	x = createObject1(20,m); /// constr in fucnt + assigment move , doesn`t copy anymore because it's not an lvalue inside the fucntion
	printf("\n");
	x.print(); 

	/*
	-Copy constructor: When returning *temp by value. *temp is an lvalue, which means it is treated as a specific named instance of Array1.
	-Returning an lvalue by value normally invokes the copy constructor, as C++ requires a fresh copy to be created for the return (lvalue goes out of scope).
	-To avoid the copy constructor here, it can return temp directly as a pointer
	-Just like a C struct returned by value


	-Compiler Optimizations and RVO:
	-Return Value Optimization (RVO) and Named Return Value Optimization (NRVO) can eliminate the need for the move constructor.
	-The compiler may optimize the code in such a way that the temporary object is constructed directly in the location of the variable being initialized,
	hence avoiding the move constructor call.
	*/

	delete b;
	delete c;
	delete d;
	delete m;

	return 0;
}