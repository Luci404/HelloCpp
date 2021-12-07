#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Animal
{
public:
    uint8_t Age;

    virtual void Speak()
    {
        std::cout << "..." << std::endl;
    }
};

class Dog : public Animal
{
public:
    virtual void Speak() override
    {
        std::cout << "Wuff" << std::endl;
    }
};

class Cat : public Animal
{
public:
    virtual void Speak() override
    {
        std::cout << "Meaooo" << std::endl;
    }
};

struct Employee
{
    std::string Name;
    uint8_t Age;
    float Pay;
    Animal* Pet;

    // Constructor
    Employee(const std::string& name, uint8_t age, float pay, Animal* pet)
    {
        Name = name;
        Age = age;
        Pay = pay;
        Pet = pet;
    }
};

int main(int argc, char* argv[])
{
    std::vector<Employee> EmployeeList;

    Employee* employeePtr = nullptr;
    
    // The shared dog...
    Dog bob1sDog = Dog(); 

    // Add new employee
    EmployeeList.push_back(Employee("Bob1", 54, 18923.0f, &bob1sDog));
    EmployeeList.push_back(Employee("Bob2", 34, 648923.0f, &bob1sDog));
    EmployeeList.push_back(Employee("Bob3", 15, 13.0f, &bob1sDog));
    EmployeeList.push_back(Employee("Bob4", 24, 1433.0f, &bob1sDog));
    EmployeeList.push_back(Employee("Bob5", 64, 48923.0f, &bob1sDog));

    // Loop over all employees and print out their data, as well as what their pet says.
    for (Employee& employee : EmployeeList)
    {
        std::cout << employee.Name << std::endl;
        employee.Pet->Speak();
        std::cout << std::endl;
    }
}