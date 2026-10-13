// Customer.h - a person who books parcels (derived from Person)
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"

class Customer : public Person {
public:
    Customer();
    Customer(int customerId, const std::string& name, const std::string& phone);
    ~Customer();

    int         getCustomerId() const;
    std::string getRole() const override;
    void        display() const override;

    static Customer deserialize(const std::string& line);   // used by FileManager::load
};

#endif
