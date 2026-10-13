// Parcel.cpp - base class + concrete parcel types
#include "Parcel.h"
#include "ParcelTypes.h"
#include "CourierException.h"
#include "Utils.h"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

// ============================================================ Parcel (base)
Parcel::Parcel() : parcelId(0), senderId(0), receiverName(""), weight(0.0) {}

Parcel::Parcel(int pid, int sid, const std::string& receiver, double w)
    : parcelId(pid), senderId(sid), receiverName(""), weight(0.0) {
    setReceiverName(receiver);
    setWeight(w);
}

Parcel::~Parcel() {}

int Parcel::getParcelId() const { return parcelId; }
int Parcel::getSenderId() const { return senderId; }
const std::string& Parcel::getReceiverName() const { return receiverName; }
double Parcel::getWeight() const { return weight; }

void Parcel::setReceiverName(const std::string& name) {
    std::string t = Utils::trim(name);
    Utils::validateName(t, "Receiver name");
    receiverName = t;
}

void Parcel::setWeight(double w) {
    Utils::validateWeight(w);
    weight = std::round(w * 100.0) / 100.0;
}

void Parcel::display() const {
    std::cout << std::left << std::setw(7) << parcelId << std::setw(8) << senderId
              << std::setw(27) << receiverName << std::setw(10) << getType()
              << std::right << std::setw(8) << std::fixed << std::setprecision(2) << weight << "  "
              << std::setw(11) << calculateCharge() << "  " << std::setw(4) << estimatedDays()
              << std::left << '\n';
}

std::string Parcel::serialize() const {
    std::ostringstream out;
    out << getType() << '|' << parcelId << '|' << senderId << '|' << receiverName << '|'
        << std::fixed << std::setprecision(2) << weight;
    return out.str();
}

Parcel* Parcel::create(const std::string& type, int id, int senderId,
                       const std::string& receiver, double weight) {
    if (type == "Standard") return new StandardParcel(id, senderId, receiver, weight);
    if (type == "Express")  return new ExpressParcel(id, senderId, receiver, weight);
    if (type == "Fragile")  return new FragileParcel(id, senderId, receiver, weight);
    throw CourierException("Unknown parcel type: " + type);
}

Parcel* Parcel::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 5) throw CourierException("Malformed parcel record: " + line);
    return create(f[0], std::atoi(f[1].c_str()), std::atoi(f[2].c_str()), f[3],
                  std::atof(f[4].c_str()));
}

void Parcel::printHeader() {
    std::cout << std::left << std::setw(7) << "ID" << std::setw(8) << "Sender"
              << std::setw(27) << "Receiver" << std::setw(10) << "Type"
              << std::right << std::setw(8) << "Wt(kg)" << "  " << std::setw(11) << "Charge(Rs)"
              << "  " << std::setw(4) << "Days" << std::left << '\n'
              << std::string(79, '-') << '\n';
}

// ============================================================ StandardParcel
StandardParcel::StandardParcel() : Parcel() {}
StandardParcel::StandardParcel(int id, int s, const std::string& r, double w) : Parcel(id, s, r, w) {}
StandardParcel::~StandardParcel() {}
double StandardParcel::calculateCharge() const { return 50.0 + 20.0 * getWeight(); }
std::string StandardParcel::getType() const { return "Standard"; }
int StandardParcel::estimatedDays() const { return 5; }

// ============================================================ ExpressParcel
ExpressParcel::ExpressParcel() : Parcel() {}
ExpressParcel::ExpressParcel(int id, int s, const std::string& r, double w) : Parcel(id, s, r, w) {}
ExpressParcel::~ExpressParcel() {}
double ExpressParcel::calculateCharge() const { return (100.0 + 35.0 * getWeight()) * 1.15; }
std::string ExpressParcel::getType() const { return "Express"; }
int ExpressParcel::estimatedDays() const { return 2; }

// ============================================================ FragileParcel
FragileParcel::FragileParcel() : Parcel() {}
FragileParcel::FragileParcel(int id, int s, const std::string& r, double w) : Parcel(id, s, r, w) {}
FragileParcel::~FragileParcel() {}
double FragileParcel::calculateCharge() const { return 80.0 + 25.0 * getWeight() + 150.0; }
std::string FragileParcel::getType() const { return "Fragile"; }
int FragileParcel::estimatedDays() const { return 4; }
