// Utils.h - validation helpers and safe console input functions
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {

extern bool echoInput;          // when true every line read is echoed (used for test transcripts)

struct InputEnded {};           // thrown when stdin is closed (EOF)

// ---- string helpers ----
std::string trim(const std::string& s);
std::string toLower(const std::string& s);
bool containsIgnoreCase(const std::string& text, const std::string& key);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string money(double amount);
std::string today();            // current date as DD/MM/YYYY

// ---- validators (throw CourierException when the value is invalid) ----
void validateName(const std::string& value, const std::string& label);
void validatePhone(const std::string& phone);
void validateWeight(double weight);
void validateDate(const std::string& date);

// ---- console input (re-ask until valid) ----
std::string readLine(const std::string& prompt);
int         readInt(const std::string& prompt, int minValue, int maxValue);
double      readDouble(const std::string& prompt, double minValue, double maxValue);
std::string readName(const std::string& prompt, const std::string& label);
std::string readPhone(const std::string& prompt);
std::string readDate(const std::string& prompt);   // blank input = today's date
bool        readYesNo(const std::string& prompt);

} // namespace Utils

#endif
