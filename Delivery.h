// Delivery.h - links one parcel to one courier and tracks its status
#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>

enum class DeliveryStatus {
    BOOKED = 0, ASSIGNED, PICKED_UP, IN_TRANSIT, OUT_FOR_DELIVERY, DELIVERED, CANCELLED
};

class Delivery {
private:
    int            deliveryId;
    int            parcelId;
    int            courierId;      // 0 = no courier assigned yet
    DeliveryStatus status;
    std::string    date;           // booking date  (DD/MM/YYYY)
    std::string    lastUpdate;     // date of last status change

public:
    Delivery();
    Delivery(int deliveryId, int parcelId, const std::string& bookingDate);
    Delivery(int deliveryId, int parcelId, int courierId, DeliveryStatus status,
             const std::string& date, const std::string& lastUpdate);   // used when loading
    ~Delivery();

    int                getDeliveryId() const;
    int                getParcelId() const;
    int                getCourierId() const;
    DeliveryStatus     getStatus() const;
    const std::string& getDate() const;
    const std::string& getLastUpdate() const;

    void assign(int newCourierId, const std::string& onDate);               // Booked/Assigned -> Assigned
    void updateStatus(DeliveryStatus newStatus, const std::string& onDate); // enforces legal transitions
    bool isActive() const;                                                  // not Delivered / Cancelled

    void        display() const;
    std::string serialize() const;
    static Delivery deserialize(const std::string& line);
    static std::string statusToString(DeliveryStatus s);
    static void printHeader();
};

#endif
