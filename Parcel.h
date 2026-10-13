// Parcel.h - abstract base class of all parcel types (runtime polymorphism)
#ifndef PARCEL_H
#define PARCEL_H

#include <string>

class Parcel {
private:
    int         parcelId;
    int         senderId;        // Customer ID of the sender
    std::string receiverName;
    double      weight;          // in kg

public:
    Parcel();
    Parcel(int parcelId, int senderId, const std::string& receiverName, double weight);
    virtual ~Parcel();

    int                getParcelId() const;
    int                getSenderId() const;
    const std::string& getReceiverName() const;
    double             getWeight() const;
    void setReceiverName(const std::string& name);   // validates
    void setWeight(double w);                        // validates, rounds to 2 decimals

    // ---- polymorphic interface: every parcel type overrides these ----
    virtual double      calculateCharge() const = 0;
    virtual std::string getType() const = 0;
    virtual int         estimatedDays() const = 0;
    virtual void        display() const;             // common table row (uses the virtuals above)

    std::string serialize() const;                   // type|id|sender|receiver|weight

    static Parcel* create(const std::string& type, int id, int senderId,
                          const std::string& receiver, double weight);   // factory
    static Parcel* deserialize(const std::string& line);
    static void    printHeader();
};

#endif
