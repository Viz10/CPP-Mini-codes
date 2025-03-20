//rvalue=temporary value
//lvalue=storaged value

#include <iostream>
#include <cstring>

// New class to be referenced
class RefClass {
public:
    std::string refName;
    RefClass(const std::string& name) : refName(name) {}
};

// InnerClass holds a reference to RefClass
class InnerClass {
    int value;
    std::string description;
    int* array;
    int arraySize;
    RefClass& ref;

public:
    // Constructor
    InnerClass(int v, const std::string& desc, int* arr, int size, RefClass& r)
        : value(v), description(desc), arraySize(size), ref(r) {
        array = new int[size];
        for (int i = 0; i < size; ++i) {
            array[i] = arr[i];
        }
        std::cout << "InnerClass constructor called with value: " << value
                  << " and description: " << description << std::endl;
    }

    // Custom Copy Constructor
    InnerClass(const InnerClass& other)
        : value(other.value), description(other.description), arraySize(other.arraySize), ref(other.ref) {
        array = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            array[i] = other.array[i];
        }
        std::cout << "InnerClass copy constructor called." << std::endl;
    }

    // Move Constructor
    InnerClass(InnerClass&& other) noexcept
        : value(other.value), description(std::move(other.description)),
          array(other.array), arraySize(other.arraySize), ref(other.ref) {
        other.array = nullptr;
        other.arraySize = 0;
        std::cout << "InnerClass move constructor called." << std::endl;
    }

    // Custom Assignment Operator
    InnerClass& operator=(const InnerClass& other) {
        if (this == &other) return *this;  // self-assignment check
        value = other.value;
        description = other.description;
        arraySize = other.arraySize;
        delete[] array;  // free existing resource
        array = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            array[i] = other.array[i];
        }
        std::cout << "InnerClass assignment operator called." << std::endl;
        return *this;
    }

    // Move Assignment Operator
    InnerClass& operator=(InnerClass&& other) noexcept {
        if (this == &other) return *this;  // self-assignment check
        delete[] array;  // free existing resource
        value = other.value;
        description = std::move(other.description);
        array = other.array;
        arraySize = other.arraySize;
        other.array = nullptr;
        other.arraySize = 0;
        std::cout << "InnerClass move assignment operator called." << std::endl;
        return *this;
    }

    // Destructor
    ~InnerClass() {
        delete[] array;
        std::cout << "InnerClass destructor called." << std::endl;
    }

    int getValue() const { return value; }
    std::string getDescription() const { return description; }
    void displayArray() const {
        std::cout << "Array: ";
        for (int i = 0; i < arraySize; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    void displayRef() const { std::cout << "Reference to: " << ref.refName << std::endl; }
};

// OuterClass holds a reference to InnerClass
class OuterClass {
    InnerClass* innerObj;
    double factor;
    std::string name;
    int* array;
    int arraySize;
    RefClass& ref;

public:
    // Constructor
    OuterClass(int v, const std::string& desc, double f, const std::string& n, int* arr, int size, RefClass& r)
        : factor(f), name(n), arraySize(size), ref(r) {
        innerObj = new InnerClass(v, desc, arr, size, r);
        array = new int[size];
        for (int i = 0; i < size; ++i) {
            array[i] = arr[i];
        }
        std::cout << "OuterClass constructor called with factor: " << factor
                  << " and name: " << name << std::endl;
    }

    // Custom Copy Constructor for deep copy
    OuterClass(const OuterClass& other)
        : factor(other.factor), name(other.name), arraySize(other.arraySize), ref(other.ref) {
        innerObj = new InnerClass(*other.innerObj);
        array = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            array[i] = other.array[i];
        }
        std::cout << "OuterClass copy constructor called." << std::endl;
    }

    // Custom Assignment Operator for deep copy
    OuterClass& operator=(const OuterClass& other) {
        if (this == &other) return *this;  // self-assignment check
        factor = other.factor;
        name = other.name;
        arraySize = other.arraySize;
        delete innerObj;  // free existing resource
        innerObj = new InnerClass(*other.innerObj); /// si clasele dinauntru primesc "=" dar cum au  date free => copy constr
        delete[] array;  // free existing resource
        array = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            array[i] = other.array[i];
        }
        std::cout << "OuterClass assignment operator called." << std::endl;
        return *this;
    }

 // Move Constructor
    OuterClass(OuterClass&& other) noexcept
        : innerObj(other.innerObj), factor(other.factor), name(std::move(other.name)),
          array(other.array), arraySize(other.arraySize), ref(other.ref) {
        other.innerObj = nullptr;
        other.array = nullptr;
        other.arraySize = 0;
        std::cout << "OuterClass move constructor called." << std::endl;
    }

    // Move Assignment Operator
    OuterClass& operator=(OuterClass&& other) noexcept {
        if (this == &other) return *this;  // self-assignment check
        delete innerObj;
        delete[] array;
        factor = other.factor;
        name = std::move(other.name);
        innerObj = other.innerObj;
        array = other.array;
        arraySize = other.arraySize;
        other.innerObj = nullptr;
        other.array = nullptr;
        other.arraySize = 0;
        std::cout << "OuterClass move assignment operator called." << std::endl;
        return *this;
    }

    // Destructor to free allocated memory
    ~OuterClass() {
        delete innerObj;
        delete[] array;
        std::cout << "OuterClass destructor called." << std::endl;
    }

    void display() {
        std::cout << "OuterClass contains an InnerClass object with value: "
                  << innerObj->getValue() << " and description: "
                  << innerObj->getDescription() << ". Factor: " << factor
                  << ", Name: " << name << std::endl;
        std::cout << "OuterClass array: ";
        for (int i = 0; i < arraySize; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
        innerObj->displayArray();
        innerObj->displayRef();
        std::cout << "OuterClass reference to: " << ref.refName << std::endl;
    }
};

int main() {
    RefClass ref("Shared Reference");
    int arr[] = {1, 2, 3, 4, 5};
    OuterClass* outer1 = new OuterClass(42, "Initial Object", 3.14, "Outer One", arr, 5, ref);
    OuterClass* outer2 = new OuterClass(*outer1); // Calls the custom copy constructor

    outer1->display();
    outer2->display();

    *outer1 = *outer2; // Calls the custom assignment operator

    outer1->display();
    outer2->display();

    OuterClass outer3 = std::move(*outer1); // Calls the move constructor
    outer3.display();

    *outer2 = std::move(outer3); // Calls the move assignment operator
    outer2->display();

    delete outer1; // Freeing the dynamically allocated memory
    delete outer2; // Freeing the dynamically allocated memory

    return 0;
}