// CourierException.h - custom exception used for every business / validation error
#ifndef COURIER_EXCEPTION_H
#define COURIER_EXCEPTION_H

#include <stdexcept>
#include <string>

class CourierException : public std::runtime_error {
public:
    explicit CourierException(const std::string& message) : std::runtime_error(message) {}
};

#endif
