// Delivery.cpp
#include "Delivery.h"
#include "CourierException.h"
#include "Utils.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

Delivery::Delivery()
    : deliveryId(0), parcelId(0), courierId(0), status(DeliveryStatus::BOOKED), date(""), lastUpdate("") {}

Delivery::Delivery(int did, int pid, const std::string& bookingDate)
    : deliveryId(did), parcelId(pid), courierId(0), status(DeliveryStatus::BOOKED),
      date(bookingDate), lastUpdate(bookingDate) {
    Utils::validateDate(bookingDate);
}

Delivery::Delivery(int did, int pid, int cid, DeliveryStatus st,
                   const std::string& d, const std::string& lu)
    : deliveryId(did), parcelId(pid), courierId(cid), status(st), date(d), lastUpdate(lu) {
    Utils::validateDate(d);
    Utils::validateDate(lu);
}

Delivery::~Delivery() {}

int Delivery::getDeliveryId() const { return deliveryId; }
int Delivery::getParcelId() const { return parcelId; }
int Delivery::getCourierId() const { return courierId; }
DeliveryStatus Delivery::getStatus() const { return status; }
const std::string& Delivery::getDate() const { return date; }
const std::string& Delivery::getLastUpdate() const { return lastUpdate; }

bool Delivery::isActive() const {
    return status != DeliveryStatus::DELIVERED && status != DeliveryStatus::CANCELLED;
}

void Delivery::assign(int newCourierId, const std::string& onDate) {
    if (status != DeliveryStatus::BOOKED && status != DeliveryStatus::ASSIGNED)
        throw CourierException("A courier can only be assigned while the parcel is Booked or Assigned (current status: " +
                               statusToString(status) + ").");
    Utils::validateDate(onDate);
    courierId = newCourierId;
    status = DeliveryStatus::ASSIGNED;
    lastUpdate = onDate;
}

void Delivery::updateStatus(DeliveryStatus newStatus, const std::string& onDate) {
    if (!isActive())
        throw CourierException("This delivery is already " + statusToString(status) + " and cannot be changed.");
    Utils::validateDate(onDate);

    if (newStatus == DeliveryStatus::CANCELLED) {
        if (status == DeliveryStatus::IN_TRANSIT || status == DeliveryStatus::OUT_FOR_DELIVERY)
            throw CourierException("Parcel is already " + statusToString(status) + "; it can no longer be cancelled.");
    } else {
        if (newStatus == DeliveryStatus::BOOKED || newStatus == DeliveryStatus::ASSIGNED)
            throw CourierException("Use 'Assign Courier' to (re)assign a courier; status cannot go back to " +
                                   statusToString(newStatus) + ".");
        if (static_cast<int>(newStatus) != static_cast<int>(status) + 1) {
            std::string next = statusToString(static_cast<DeliveryStatus>(static_cast<int>(status) + 1));
            if (status == DeliveryStatus::BOOKED) next = "Assigned (use Assign Courier)";
            throw CourierException("Invalid transition: " + statusToString(status) + " -> " +
                                   statusToString(newStatus) + ". Next allowed status: " + next + ".");
        }
    }
    status = newStatus;
    lastUpdate = onDate;
}

std::string Delivery::statusToString(DeliveryStatus s) {
    switch (s) {
        case DeliveryStatus::BOOKED:           return "Booked";
        case DeliveryStatus::ASSIGNED:         return "Assigned";
        case DeliveryStatus::PICKED_UP:        return "Picked Up";
        case DeliveryStatus::IN_TRANSIT:       return "In Transit";
        case DeliveryStatus::OUT_FOR_DELIVERY: return "Out For Delivery";
        case DeliveryStatus::DELIVERED:        return "Delivered";
        case DeliveryStatus::CANCELLED:        return "Cancelled";
    }
    return "Unknown";
}

void Delivery::display() const {
    std::cout << std::left << std::setw(7) << deliveryId << std::setw(8) << parcelId
              << std::setw(9);
    if (courierId == 0) std::cout << "-"; else std::cout << courierId;
    std::cout << std::setw(18) << statusToString(status) << std::setw(13) << date
              << std::setw(12) << lastUpdate << '\n';
}

std::string Delivery::serialize() const {
    return std::to_string(deliveryId) + "|" + std::to_string(parcelId) + "|" +
           std::to_string(courierId) + "|" + std::to_string(static_cast<int>(status)) + "|" +
           date + "|" + lastUpdate;
}

Delivery Delivery::deserialize(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 6) throw CourierException("Malformed delivery record: " + line);
    int st = std::atoi(f[3].c_str());
    if (st < 0 || st > 6) throw CourierException("Invalid delivery status code in: " + line);
    return Delivery(std::atoi(f[0].c_str()), std::atoi(f[1].c_str()), std::atoi(f[2].c_str()),
                    static_cast<DeliveryStatus>(st), f[4], f[5]);
}

void Delivery::printHeader() {
    std::cout << std::left << std::setw(7) << "DelID" << std::setw(8) << "Parcel"
              << std::setw(9) << "Courier" << std::setw(18) << "Status" << std::setw(13) << "Booked On"
              << std::setw(12) << "Updated On" << '\n' << std::string(67, '-') << '\n';
}
