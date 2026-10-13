// Courier.h - a delivery person (derived from Person)
#ifndef COURIER_H
#define COURIER_H

#include "Person.h"

class Courier : public Person {
public:
    Courier();
    Courier(int courierId, const std::string& name, const std::string& phone);
    ~Courier();

    int         getCourierId() const;
    std::string getRole() const override;
    void        display() const override;

    static Courier deserialize(const std::string& line);
};

#endif
