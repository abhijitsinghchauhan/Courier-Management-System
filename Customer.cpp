// Customer.cpp
#include "Customer.h"
#include "CourierException.h"
#include "Utils.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

Customer::Customer() : Person() {}
Customer::Customer(int customerId, const std::string& name, const std::string& phone)
    : Person(customerId, name, phone) {}
Customer::~Customer() {}

int Customer::getCustomerId() const { return getId(); }
std::string Customer::getRole() const { return "Customer"; }

void Customer::display() const {
    std::cout << std::left << std::setw(10) << getRole();
    printRow();
    std::cout << '\n';
}

Customer Customer::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 3) throw CourierException("Malformed customer record: " + line);
    return Customer(std::atoi(f[0].c_str()), f[1], f[2]);
}
