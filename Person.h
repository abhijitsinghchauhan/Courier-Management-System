// Person.h - abstract base class for Customer and Courier (inheritance)
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:                                   // encapsulation: data hidden from outside
    int         id;
    std::string name;
    std::string phone;

protected:
    void printRow() const;                 // prints "ID  Name  Phone" columns

public:
    Person();                                                        // default constructor
    Person(int id, const std::string& name, const std::string& phone); // parameterized constructor
    virtual ~Person();                                               // virtual destructor

    int                getId() const;
    const std::string& getName() const;
    const std::string& getPhone() const;
    void setName(const std::string& newName);     // validates, throws CourierException
    void setPhone(const std::string& newPhone);   // validates, throws CourierException

    virtual std::string getRole() const = 0;      // pure virtual -> runtime polymorphism
    virtual void        display() const = 0;      // pure virtual -> runtime polymorphism
    std::string         serialize() const;        // id|name|phone

    static void printHeader();
};

#endif
