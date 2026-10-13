// Courier.cpp
#include "Courier.h"
#include "CourierException.h"
#include "Utils.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

Courier::Courier() : Person() {}
Courier::Courier(int courierId, const std::string& name, const std::string& phone)
    : Person(courierId, name, phone) {}
Courier::~Courier() {}

int Courier::getCourierId() const { return getId(); }
std::string Courier::getRole() const { return "Courier"; }

void Courier::display() const {
    std::cout << std::left << std::setw(10) << getRole();
    printRow();
    std::cout << '\n';
}

Courier Courier::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 3) throw CourierException("Malformed courier record: " + line);
    return Courier(std::atoi(f[0].c_str()), f[1], f[2]);
}
