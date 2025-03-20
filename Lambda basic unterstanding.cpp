#include <iostream>
#include <vector>
#include <algorithm>

class Person {
public:
    std::string name;
    int age;

    Person(std::string name, int age) : name(name), age(age) {}

    void display() const {
        std::cout << name << ": " << age << std::endl;
    }

bool operator==(const Person &other) const {
 return name == other.name && age == other.age; 
}

};

//////////////////////////////////////////////////////////////////
/*
The capture list in a lambda captures variables from the surrounding scope where the lambda is defined.
 This means any variables that are in scope at the point where the lambda is created can be captured and used inside the lambda.
[=] all by value
[&] all by ref
[x,&y] specific
[&,y]
*/
//////////////////////////////////////////////////////////////////

int main() {

    // Create a vector of raw pointers to Person objects
    std::vector<Person*> people;
    people.push_back(new Person("Alice", 30));
    people.push_back(new Person("Bob", 25));
    people.push_back(new Person("Charlie", 35));
    people.push_back(new Person("David", 25));
    people.push_back(new Person("Eve", 30));

    // Sort the vector by age, and then by name if ages are equal
    std::sort(people.begin(), people.end(), [](const Person* a, const Person* b) {
        if (a->age == b->age) {
            return a->name < b->name;
        }
        return a->age < b->age;
    });

    // Print the sorted vector
    std::for_each(people.begin(), people.end(), [](const Person* person) {
        person->display();
    });

    // Use std::find_if to find a person named "Bob"
    auto it = std::find_if(people.begin(), people.end(), [](const Person* person) {
        return person->name == "Bob";
    });

    if (it != people.end()) {
        std::cout << "Found: ";
        (*it)->display();
    } else {
        std::cout << "Bob not found" << std::endl;
    }

Person *target=new Person("Alice", 30); // std::count used to count the number of target 

 int count = std::count(people.begin(), people.end(), target);
 std::cout << "Number of 'Alice' instances: " << count << std::endl;

auto it = std::find(people.begin(), people.end(), target);
 if (it != people.end()) {
 std::cout << "Found: " << it->name << " aged " << it->age << std::endl;
 } 
else {
 std::cout << "Person not found" << std::endl; 
}


// Clean up the dynamically allocated memory
    for (auto &person : people) {
        delete person;
    }

delete target;

    return 0;
}

//////////////////////////////////////////////////////////
/*
void process(D&& other) 
{ D d1 = other;
 // `other` is an lvalue here.
 D d2 = std::move(other);
 // `std::move` casts `other` back to an rvalue.
}
*/