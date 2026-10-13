// CourierSystem.h - the controller class: owns all dynamic arrays and implements every operation
#ifndef COURIER_SYSTEM_H
#define COURIER_SYSTEM_H

#include "Courier.h"
#include "Customer.h"
#include "Delivery.h"
#include "FileManager.h"
#include "Parcel.h"

class CourierSystem {
private:
    // ---- dynamically allocated storage (new[] ... delete[]) ----
    Customer*  customers;   int customerCount;  int customerCap;
    Courier*   couriers;    int courierCount;   int courierCap;
    Parcel**   parcels;     int parcelCount;    int parcelCap;     // array of base-class pointers -> polymorphism
    Delivery*  deliveries;  int deliveryCount;  int deliveryCap;

    int nextCustomerId, nextCourierId, nextParcelId, nextDeliveryId;

    FileManager fileManager;           // composition: the system owns its FileManager
    friend class FileManager;          // FileManager needs direct access to the arrays

    CourierSystem(const CourierSystem&) = delete;           // copying is disabled (raw pointers)
    CourierSystem& operator=(const CourierSystem&) = delete;

    // ---- lookup helpers: return index or -1 ----
    int findCustomer(int id) const;
    int findCourier(int id) const;
    int findParcel(int id) const;
    int findDeliveryByParcel(int parcelId) const;

    // ---- storage helpers ----
    void appendCustomer(const Customer& c);
    void appendCourier(const Courier& c);
    void appendParcel(Parcel* p);
    void appendDelivery(const Delivery& d);
    void clearAll();
    void showParcelRow(int parcelIndex) const;

public:
    explicit CourierSystem(const std::string& dataDirectory = "Data");
    ~CourierSystem();

    // 1. Add Record
    void addCustomer();
    void addCourier();
    void bookParcel();                 // also the first step of the main transaction

    // 2. Display Records
    void displayCustomers() const;
    void displayCouriers() const;
    void displayParcels() const;
    void displayDeliveries() const;
    void displayAllPersons() const;    // Customers + Couriers through Person* (polymorphism)

    // 3. Search Record
    void searchCustomer() const;
    void searchCourier() const;
    void searchParcel() const;
    void searchDeliveryByStatus() const;

    // 4. Update Record
    void updateCustomer();
    void updateCourier();
    void updateParcel();

    // 5. Delete Record
    void deleteCustomer();
    void deleteCourier();
    void deleteParcel();

    // 6. Main transaction: book -> assign -> update status -> track
    void assignCourier();
    void updateDeliveryStatus();
    void trackParcel() const;

    // 7. Report
    void generateReport() const;

    // 8. Save / Load
    void saveData() const;
    void loadData();
    int  totalRecords() const;
};

#endif
