// ParcelTypes.h - concrete parcel classes (inheritance from Parcel)
#ifndef PARCEL_TYPES_H
#define PARCEL_TYPES_H

#include "Parcel.h"

// Rs. 50 base + Rs. 20 per kg, delivered in about 5 days
class StandardParcel : public Parcel {
public:
    StandardParcel();
    StandardParcel(int id, int senderId, const std::string& receiver, double weight);
    ~StandardParcel();
    double      calculateCharge() const override;
    std::string getType() const override;
    int         estimatedDays() const override;
};

// (Rs. 100 base + Rs. 35 per kg) + 15% express surcharge, delivered in about 2 days
class ExpressParcel : public Parcel {
public:
    ExpressParcel();
    ExpressParcel(int id, int senderId, const std::string& receiver, double weight);
    ~ExpressParcel();
    double      calculateCharge() const override;
    std::string getType() const override;
    int         estimatedDays() const override;
};

// Rs. 80 base + Rs. 25 per kg + Rs. 150 special handling fee, delivered in about 4 days
class FragileParcel : public Parcel {
public:
    FragileParcel();
    FragileParcel(int id, int senderId, const std::string& receiver, double weight);
    ~FragileParcel();
    double      calculateCharge() const override;
    std::string getType() const override;
    int         estimatedDays() const override;
};

#endif
