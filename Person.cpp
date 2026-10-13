// Person.cpp
#include "Person.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

Person::Person() : id(0), name(""), phone("") {}

Person::Person(int id_, const std::string& name_, const std::string& phone_) : id(id_) {
    setName(name_);       // validation happens inside the setters
    setPhone(phone_);
}

Person::~Person() {}

int Person::getId() const { return id; }
const std::string& Person::getName() const { return name; }
const std::string& Person::getPhone() const { return phone; }

void Person::setName(const std::string& newName) {
    std::string t = Utils::trim(newName);
    Utils::validateName(t, "Name");
    name = t;
}

void Person::setPhone(const std::string& newPhone) {
    std::string t = Utils::trim(newPhone);
    Utils::validatePhone(t);
    phone = t;
}

std::string Person::serialize() const {
    return std::to_string(id) + "|" + name + "|" + phone;
}

void Person::printRow() const {
    std::cout << std::left << std::setw(7) << id << std::setw(27) << name << std::setw(12) << phone;
}

void Person::printHeader() {
    std::cout << std::left << std::setw(10) << "Role" << std::setw(7) << "ID"
              << std::setw(27) << "Name" << std::setw(12) << "Phone" << '\n'
              << std::string(56, '-') << '\n';
}
