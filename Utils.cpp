// Utils.cpp
#include "Utils.h"
#include "CourierException.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Utils {

bool echoInput = false;

std::string trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return r;
}

bool containsIgnoreCase(const std::string& text, const std::string& key) {
    return toLower(text).find(toLower(key)) != std::string::npos;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> parts;
    std::string item;
    std::istringstream in(s);
    while (std::getline(in, item, delimiter)) parts.push_back(item);
    if (!s.empty() && s.back() == delimiter) parts.push_back("");
    return parts;
}

std::string money(double amount) {
    std::ostringstream out;
    out << "Rs. " << std::fixed << std::setprecision(2) << amount;
    return out.str();
}

std::string today() {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%d/%m/%Y", lt);
    return buf;
}

// ------------------------------------------------------------------ validators
void validateName(const std::string& value, const std::string& label) {
    if (value.size() < 2 || value.size() > 25)
        throw CourierException(label + " must be 2 to 25 characters long.");
    bool hasLetter = false;
    for (unsigned char c : value) {
        if (std::isalpha(c)) hasLetter = true;
        else if (c != ' ' && c != '.' && c != '-' && c != '\'')
            throw CourierException(label + " may contain only letters, spaces, '.', '-' and '''.");
    }
    if (!hasLetter) throw CourierException(label + " must contain at least one letter.");
}

void validatePhone(const std::string& phone) {
    if (phone.size() != 10)
        throw CourierException("Phone number must be exactly 10 digits.");
    for (unsigned char c : phone)
        if (!std::isdigit(c)) throw CourierException("Phone number must contain digits only.");
    if (phone[0] < '6')
        throw CourierException("Phone number must start with 6, 7, 8 or 9.");
}

void validateWeight(double weight) {
    if (!(weight >= 0.1 && weight <= 50.0))
        throw CourierException("Weight must be between 0.1 kg and 50 kg.");
}

void validateDate(const std::string& date) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/')
        throw CourierException("Date must be in DD/MM/YYYY format.");
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 2 || i == 5) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            throw CourierException("Date must be in DD/MM/YYYY format.");
    }
    int d = std::atoi(date.substr(0, 2).c_str());
    int m = std::atoi(date.substr(3, 2).c_str());
    int y = std::atoi(date.substr(6, 4).c_str());
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1)
        throw CourierException("Date is not a valid calendar date.");
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = days[m - 1];
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (m == 2 && leap) maxDay = 29;
    if (d > maxDay) throw CourierException("Date is not a valid calendar date.");
}

// ------------------------------------------------------------------ input
std::string readLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) throw InputEnded();
    if (echoInput) std::cout << line << '\n';
    return trim(line);
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::string s = readLine(prompt);
        char* end = nullptr;
        long v = std::strtol(s.c_str(), &end, 10);
        if (s.empty() || *end != '\0')
            std::cout << "  Invalid input. Please enter a whole number.\n";
        else if (v < minValue || v > maxValue)
            std::cout << "  Please enter a number between " << minValue << " and " << maxValue << ".\n";
        else
            return static_cast<int>(v);
    }
}

double readDouble(const std::string& prompt, double minValue, double maxValue) {
    while (true) {
        std::string s = readLine(prompt);
        char* end = nullptr;
        double v = std::strtod(s.c_str(), &end);
        if (s.empty() || *end != '\0')
            std::cout << "  Invalid input. Please enter a number.\n";
        else if (v < minValue || v > maxValue)
            std::cout << "  Value must be between " << minValue << " and " << maxValue << ".\n";
        else
            return v;
    }
}

std::string readName(const std::string& prompt, const std::string& label) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            validateName(s, label);
            return s;
        } catch (const CourierException& e) {
            std::cout << "  Error: " << e.what() << '\n';
        }
    }
}

std::string readPhone(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            validatePhone(s);
            return s;
        } catch (const CourierException& e) {
            std::cout << "  Error: " << e.what() << '\n';
        }
    }
}

std::string readDate(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return today();
        try {
            validateDate(s);
            return s;
        } catch (const CourierException& e) {
            std::cout << "  Error: " << e.what() << '\n';
        }
    }
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        std::string s = toLower(readLine(prompt));
        if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        std::cout << "  Please answer y or n.\n";
    }
}

} // namespace Utils
