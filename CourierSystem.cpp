// CourierSystem.cpp - business logic of the Courier Management System
#include "CourierSystem.h"
#include "CourierException.h"
#include "ParcelTypes.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>

namespace {

const int INITIAL_CAPACITY = 4;

// Doubles the capacity of a dynamic array: allocate bigger block, copy, release the old block.
template <typename T>
void grow(T*& arr, int count, int& cap) {
    int newCap = cap * 2;
    T* bigger = new T[newCap];
    for (int i = 0; i < count; ++i) bigger[i] = arr[i];
    delete[] arr;
    arr = bigger;
    cap = newCap;
}

void line(char c = '=', int n = 60) { std::cout << std::string(n, c) << '\n'; }

} // namespace

// ====================================================================== lifetime
CourierSystem::CourierSystem(const std::string& dataDirectory)
    : customers(new Customer[INITIAL_CAPACITY]), customerCount(0), customerCap(INITIAL_CAPACITY),
      couriers(new Courier[INITIAL_CAPACITY]),   courierCount(0),  courierCap(INITIAL_CAPACITY),
      parcels(new Parcel*[INITIAL_CAPACITY]),    parcelCount(0),   parcelCap(INITIAL_CAPACITY),
      deliveries(new Delivery[INITIAL_CAPACITY]), deliveryCount(0), deliveryCap(INITIAL_CAPACITY),
      nextCustomerId(1001), nextCourierId(2001), nextParcelId(3001), nextDeliveryId(4001),
      fileManager(dataDirectory) {}

CourierSystem::~CourierSystem() {
    std::cout << "[Memory] Releasing " << customerCount << " customer(s), " << courierCount
              << " courier(s), " << parcelCount << " parcel(s) and " << deliveryCount
              << " delivery record(s).\n";
    for (int i = 0; i < parcelCount; ++i) delete parcels[i];   // each parcel was created with new
    delete[] parcels;
    delete[] customers;
    delete[] couriers;
    delete[] deliveries;
}

void CourierSystem::clearAll() {
    for (int i = 0; i < parcelCount; ++i) delete parcels[i];
    customerCount = courierCount = parcelCount = deliveryCount = 0;
    nextCustomerId = 1001; nextCourierId = 2001; nextParcelId = 3001; nextDeliveryId = 4001;
}

int CourierSystem::totalRecords() const {
    return customerCount + courierCount + parcelCount + deliveryCount;
}

// ====================================================================== helpers
int CourierSystem::findCustomer(int id) const {
    for (int i = 0; i < customerCount; ++i) if (customers[i].getId() == id) return i;
    return -1;
}
int CourierSystem::findCourier(int id) const {
    for (int i = 0; i < courierCount; ++i) if (couriers[i].getId() == id) return i;
    return -1;
}
int CourierSystem::findParcel(int id) const {
    for (int i = 0; i < parcelCount; ++i) if (parcels[i]->getParcelId() == id) return i;
    return -1;
}
int CourierSystem::findDeliveryByParcel(int parcelId) const {
    for (int i = 0; i < deliveryCount; ++i) if (deliveries[i].getParcelId() == parcelId) return i;
    return -1;
}

void CourierSystem::appendCustomer(const Customer& c) {
    if (customerCount == customerCap) grow(customers, customerCount, customerCap);
    customers[customerCount++] = c;
    if (c.getId() >= nextCustomerId) nextCustomerId = c.getId() + 1;
}
void CourierSystem::appendCourier(const Courier& c) {
    if (courierCount == courierCap) grow(couriers, courierCount, courierCap);
    couriers[courierCount++] = c;
    if (c.getId() >= nextCourierId) nextCourierId = c.getId() + 1;
}
void CourierSystem::appendParcel(Parcel* p) {
    if (parcelCount == parcelCap) grow(parcels, parcelCount, parcelCap);
    parcels[parcelCount++] = p;
    if (p->getParcelId() >= nextParcelId) nextParcelId = p->getParcelId() + 1;
}
void CourierSystem::appendDelivery(const Delivery& d) {
    if (deliveryCount == deliveryCap) grow(deliveries, deliveryCount, deliveryCap);
    deliveries[deliveryCount++] = d;
    if (d.getDeliveryId() >= nextDeliveryId) nextDeliveryId = d.getDeliveryId() + 1;
}

void CourierSystem::showParcelRow(int idx) const { parcels[idx]->display(); }   // virtual call

// ====================================================================== 1. ADD
void CourierSystem::addCustomer() {
    std::cout << "\n--- Add Customer ---\n";
    std::string name  = Utils::readName("Customer name : ", "Name");
    std::string phone = Utils::readPhone("Phone (10 digits): ");
    for (int i = 0; i < customerCount; ++i)
        if (customers[i].getPhone() == phone)
            throw CourierException("A customer with phone " + phone + " already exists (ID " +
                                   std::to_string(customers[i].getId()) + ").");
    appendCustomer(Customer(nextCustomerId, name, phone));
    std::cout << "Customer added successfully. Customer ID = " << customers[customerCount - 1].getId() << '\n';
}

void CourierSystem::addCourier() {
    std::cout << "\n--- Add Courier ---\n";
    std::string name  = Utils::readName("Courier name  : ", "Name");
    std::string phone = Utils::readPhone("Phone (10 digits): ");
    for (int i = 0; i < courierCount; ++i)
        if (couriers[i].getPhone() == phone)
            throw CourierException("A courier with phone " + phone + " already exists (ID " +
                                   std::to_string(couriers[i].getId()) + ").");
    appendCourier(Courier(nextCourierId, name, phone));
    std::cout << "Courier added successfully. Courier ID = " << couriers[courierCount - 1].getId() << '\n';
}

void CourierSystem::bookParcel() {
    std::cout << "\n--- Book Parcel ---\n";
    if (customerCount == 0) throw CourierException("No customers registered. Add a customer first.");
    int senderId = Utils::readInt("Sender (Customer) ID : ", 1, 999999);
    int ci = findCustomer(senderId);
    if (ci < 0) throw CourierException("Customer ID " + std::to_string(senderId) + " not found.");
    std::cout << "Sender: " << customers[ci].getName() << '\n';

    std::string receiver = Utils::readName("Receiver name         : ", "Receiver name");
    double weight = Utils::readDouble("Weight in kg (0.1-50) : ", -1e9, 1e9);
    Utils::validateWeight(weight);
    std::cout << "Parcel type: 1. Standard  2. Express  3. Fragile\n";
    int t = Utils::readInt("Choose type (1-3)     : ", 1, 3);
    std::string date = Utils::readDate("Booking date DD/MM/YYYY (blank = today): ");

    const char* types[] = {"Standard", "Express", "Fragile"};
    Parcel* p = Parcel::create(types[t - 1], nextParcelId, senderId, receiver, weight);
    try {
        Delivery d(nextDeliveryId, p->getParcelId(), date);
        appendParcel(p);          // from here the system owns the pointer
        appendDelivery(d);
    } catch (...) {
        delete p;
        throw;
    }
    std::cout << "\nParcel booked successfully!\n";
    line('-', 44);
    std::cout << " Parcel ID    : " << p->getParcelId() << '\n'
              << " Delivery ID  : " << deliveries[deliveryCount - 1].getDeliveryId() << '\n'
              << " Type         : " << p->getType() << '\n'
              << " Charge       : " << Utils::money(p->calculateCharge()) << '\n'
              << " Est. delivery: " << p->estimatedDays() << " day(s)\n"
              << " Status       : Booked (courier not yet assigned)\n";
    line('-', 44);
}

// ====================================================================== 2. DISPLAY
void CourierSystem::displayCustomers() const {
    std::cout << "\n--- Customers (" << customerCount << ") ---\n";
    if (customerCount == 0) { std::cout << "No customer records.\n"; return; }
    Person::printHeader();
    for (int i = 0; i < customerCount; ++i) customers[i].display();
}

void CourierSystem::displayCouriers() const {
    std::cout << "\n--- Couriers (" << courierCount << ") ---\n";
    if (courierCount == 0) { std::cout << "No courier records.\n"; return; }
    Person::printHeader();
    for (int i = 0; i < courierCount; ++i) couriers[i].display();
}

void CourierSystem::displayParcels() const {
    std::cout << "\n--- Parcels (" << parcelCount << ") ---\n";
    if (parcelCount == 0) { std::cout << "No parcel records.\n"; return; }
    Parcel::printHeader();
    for (int i = 0; i < parcelCount; ++i) showParcelRow(i);
}

void CourierSystem::displayDeliveries() const {
    std::cout << "\n--- Deliveries (" << deliveryCount << ") ---\n";
    if (deliveryCount == 0) { std::cout << "No delivery records.\n"; return; }
    Delivery::printHeader();
    for (int i = 0; i < deliveryCount; ++i) deliveries[i].display();
}

void CourierSystem::displayAllPersons() const {
    int n = customerCount + courierCount;
    std::cout << "\n--- All Registered Persons (" << n << ") ---\n";
    if (n == 0) { std::cout << "No records.\n"; return; }
    Person** all = new Person*[n];               // temporary, non-owning array of base pointers
    int k = 0;
    for (int i = 0; i < customerCount; ++i) all[k++] = &customers[i];
    for (int i = 0; i < courierCount; ++i)  all[k++] = &couriers[i];
    Person::printHeader();
    for (int i = 0; i < n; ++i) all[i]->display();   // runtime polymorphism: Customer::display / Courier::display
    delete[] all;                                    // only the pointer array is released, not the objects
}

// ====================================================================== 3. SEARCH
void CourierSystem::searchCustomer() const {
    std::cout << "\n--- Search Customer ---\n1. By ID   2. By name (partial)\n";
    int mode = Utils::readInt("Choice: ", 1, 2);
    int found = 0;
    if (mode == 1) {
        int id = Utils::readInt("Customer ID: ", 1, 999999);
        int i = findCustomer(id);
        if (i >= 0) { Person::printHeader(); customers[i].display(); found = 1; }
    } else {
        std::string key = Utils::readLine("Name contains: ");
        if (key.empty()) throw CourierException("Search text cannot be empty.");
        for (int i = 0; i < customerCount; ++i)
            if (Utils::containsIgnoreCase(customers[i].getName(), key)) {
                if (found == 0) Person::printHeader();
                customers[i].display(); ++found;
            }
    }
    if (found == 0) std::cout << "No matching customer found.\n";
    else std::cout << found << " record(s) found.\n";
}

void CourierSystem::searchCourier() const {
    std::cout << "\n--- Search Courier ---\n1. By ID   2. By name (partial)\n";
    int mode = Utils::readInt("Choice: ", 1, 2);
    int found = 0;
    if (mode == 1) {
        int id = Utils::readInt("Courier ID: ", 1, 999999);
        int i = findCourier(id);
        if (i >= 0) { Person::printHeader(); couriers[i].display(); found = 1; }
    } else {
        std::string key = Utils::readLine("Name contains: ");
        if (key.empty()) throw CourierException("Search text cannot be empty.");
        for (int i = 0; i < courierCount; ++i)
            if (Utils::containsIgnoreCase(couriers[i].getName(), key)) {
                if (found == 0) Person::printHeader();
                couriers[i].display(); ++found;
            }
    }
    if (found == 0) std::cout << "No matching courier found.\n";
    else std::cout << found << " record(s) found.\n";
}

void CourierSystem::searchParcel() const {
    std::cout << "\n--- Search Parcel ---\n1. By Parcel ID   2. By receiver name (partial)\n";
    int mode = Utils::readInt("Choice: ", 1, 2);
    int found = 0;
    if (mode == 1) {
        int id = Utils::readInt("Parcel ID: ", 1, 999999);
        int i = findParcel(id);
        if (i >= 0) { Parcel::printHeader(); showParcelRow(i); found = 1; }
    } else {
        std::string key = Utils::readLine("Receiver name contains: ");
        if (key.empty()) throw CourierException("Search text cannot be empty.");
        for (int i = 0; i < parcelCount; ++i)
            if (Utils::containsIgnoreCase(parcels[i]->getReceiverName(), key)) {
                if (found == 0) Parcel::printHeader();
                showParcelRow(i); ++found;
            }
    }
    if (found == 0) std::cout << "No matching parcel found.\n";
    else std::cout << found << " record(s) found.\n";
}

void CourierSystem::searchDeliveryByStatus() const {
    std::cout << "\n--- Search Deliveries by Status ---\n";
    for (int s = 0; s <= 6; ++s)
        std::cout << " " << (s + 1) << ". " << Delivery::statusToString(static_cast<DeliveryStatus>(s)) << '\n';
    int choice = Utils::readInt("Choose status (1-7): ", 1, 7);
    DeliveryStatus wanted = static_cast<DeliveryStatus>(choice - 1);
    int found = 0;
    for (int i = 0; i < deliveryCount; ++i)
        if (deliveries[i].getStatus() == wanted) {
            if (found == 0) Delivery::printHeader();
            deliveries[i].display(); ++found;
        }
    if (found == 0) std::cout << "No deliveries with status '" << Delivery::statusToString(wanted) << "'.\n";
    else std::cout << found << " record(s) found.\n";
}

// ====================================================================== 4. UPDATE
void CourierSystem::updateCustomer() {
    std::cout << "\n--- Update Customer ---\n";
    int id = Utils::readInt("Customer ID: ", 1, 999999);
    int i = findCustomer(id);
    if (i < 0) throw CourierException("Customer ID " + std::to_string(id) + " not found.");
    Person::printHeader(); customers[i].display();
    std::cout << "1. Change name   2. Change phone\n";
    int c = Utils::readInt("Choice: ", 1, 2);
    if (c == 1) customers[i].setName(Utils::readName("New name : ", "Name"));
    else {
        std::string ph = Utils::readPhone("New phone (10 digits): ");
        for (int k = 0; k < customerCount; ++k)
            if (k != i && customers[k].getPhone() == ph)
                throw CourierException("Phone " + ph + " already belongs to customer " +
                                       std::to_string(customers[k].getId()) + ".");
        customers[i].setPhone(ph);
    }
    std::cout << "Customer updated successfully.\n";
    Person::printHeader(); customers[i].display();
}

void CourierSystem::updateCourier() {
    std::cout << "\n--- Update Courier ---\n";
    int id = Utils::readInt("Courier ID: ", 1, 999999);
    int i = findCourier(id);
    if (i < 0) throw CourierException("Courier ID " + std::to_string(id) + " not found.");
    Person::printHeader(); couriers[i].display();
    std::cout << "1. Change name   2. Change phone\n";
    int c = Utils::readInt("Choice: ", 1, 2);
    if (c == 1) couriers[i].setName(Utils::readName("New name : ", "Name"));
    else {
        std::string ph = Utils::readPhone("New phone (10 digits): ");
        for (int k = 0; k < courierCount; ++k)
            if (k != i && couriers[k].getPhone() == ph)
                throw CourierException("Phone " + ph + " already belongs to courier " +
                                       std::to_string(couriers[k].getId()) + ".");
        couriers[i].setPhone(ph);
    }
    std::cout << "Courier updated successfully.\n";
    Person::printHeader(); couriers[i].display();
}

void CourierSystem::updateParcel() {
    std::cout << "\n--- Update Parcel ---\n";
    int id = Utils::readInt("Parcel ID: ", 1, 999999);
    int i = findParcel(id);
    if (i < 0) throw CourierException("Parcel ID " + std::to_string(id) + " not found.");
    int d = findDeliveryByParcel(id);
    if (d >= 0 && deliveries[d].getStatus() != DeliveryStatus::BOOKED &&
        deliveries[d].getStatus() != DeliveryStatus::ASSIGNED)
        throw CourierException("Parcel can be edited only while Booked or Assigned (current status: " +
                               Delivery::statusToString(deliveries[d].getStatus()) + ").");
    Parcel::printHeader(); showParcelRow(i);
    std::cout << "1. Change receiver name   2. Change weight (charge is recalculated)\n";
    int c = Utils::readInt("Choice: ", 1, 2);
    if (c == 1) parcels[i]->setReceiverName(Utils::readName("New receiver name: ", "Receiver name"));
    else {
        double w = Utils::readDouble("New weight in kg: ", -1e9, 1e9);
        parcels[i]->setWeight(w);
    }
    std::cout << "Parcel updated successfully.\n";
    Parcel::printHeader(); showParcelRow(i);
}

// ====================================================================== 5. DELETE
void CourierSystem::deleteCustomer() {
    std::cout << "\n--- Delete Customer ---\n";
    int id = Utils::readInt("Customer ID: ", 1, 999999);
    int i = findCustomer(id);
    if (i < 0) throw CourierException("Customer ID " + std::to_string(id) + " not found.");
    for (int k = 0; k < parcelCount; ++k)
        if (parcels[k]->getSenderId() == id)
            throw CourierException("Customer has parcel records (e.g. Parcel " +
                                   std::to_string(parcels[k]->getParcelId()) + "). Delete those parcels first.");
    Person::printHeader(); customers[i].display();
    if (!Utils::readYesNo("Delete this customer? (y/n): ")) { std::cout << "Cancelled.\n"; return; }
    for (int k = i; k < customerCount - 1; ++k) customers[k] = customers[k + 1];
    --customerCount;
    std::cout << "Customer deleted.\n";
}

void CourierSystem::deleteCourier() {
    std::cout << "\n--- Delete Courier ---\n";
    int id = Utils::readInt("Courier ID: ", 1, 999999);
    int i = findCourier(id);
    if (i < 0) throw CourierException("Courier ID " + std::to_string(id) + " not found.");
    for (int k = 0; k < deliveryCount; ++k)
        if (deliveries[k].getCourierId() == id && deliveries[k].isActive())
            throw CourierException("Courier has an active delivery (Parcel " +
                                   std::to_string(deliveries[k].getParcelId()) + "). Re-assign or finish it first.");
    Person::printHeader(); couriers[i].display();
    if (!Utils::readYesNo("Delete this courier? (y/n): ")) { std::cout << "Cancelled.\n"; return; }
    for (int k = i; k < courierCount - 1; ++k) couriers[k] = couriers[k + 1];
    --courierCount;
    std::cout << "Courier deleted.\n";
}

void CourierSystem::deleteParcel() {
    std::cout << "\n--- Delete Parcel (and its delivery record) ---\n";
    int id = Utils::readInt("Parcel ID: ", 1, 999999);
    int i = findParcel(id);
    if (i < 0) throw CourierException("Parcel ID " + std::to_string(id) + " not found.");
    int d = findDeliveryByParcel(id);
    if (d >= 0) {
        DeliveryStatus st = deliveries[d].getStatus();
        if (st == DeliveryStatus::PICKED_UP || st == DeliveryStatus::IN_TRANSIT ||
            st == DeliveryStatus::OUT_FOR_DELIVERY)
            throw CourierException("Parcel is on its way (" + Delivery::statusToString(st) +
                                   "); it cannot be deleted now.");
    }
    Parcel::printHeader(); showParcelRow(i);
    if (!Utils::readYesNo("Delete this parcel? (y/n): ")) { std::cout << "Cancelled.\n"; return; }
    delete parcels[i];                                   // release the heap object
    for (int k = i; k < parcelCount - 1; ++k) parcels[k] = parcels[k + 1];
    --parcelCount;
    if (d >= 0) {
        for (int k = d; k < deliveryCount - 1; ++k) deliveries[k] = deliveries[k + 1];
        --deliveryCount;
    }
    std::cout << "Parcel and its delivery record deleted.\n";
}

// ====================================================================== 6. TRANSACTION
void CourierSystem::assignCourier() {
    std::cout << "\n--- Assign Courier to Parcel ---\n";
    int pid = Utils::readInt("Parcel ID : ", 1, 999999);
    int d = findDeliveryByParcel(pid);
    if (d < 0) throw CourierException("No delivery record found for Parcel ID " + std::to_string(pid) + ".");
    if (courierCount == 0) throw CourierException("No couriers registered. Add a courier first.");
    int cid = Utils::readInt("Courier ID: ", 1, 999999);
    int c = findCourier(cid);
    if (c < 0) throw CourierException("Courier ID " + std::to_string(cid) + " not found.");
    std::string date = Utils::readDate("Date DD/MM/YYYY (blank = today): ");
    deliveries[d].assign(cid, date);
    std::cout << "Parcel " << pid << " assigned to courier " << couriers[c].getName()
              << " (ID " << cid << "). Status: " << Delivery::statusToString(deliveries[d].getStatus()) << '\n';
}

void CourierSystem::updateDeliveryStatus() {
    std::cout << "\n--- Update Delivery Status ---\n";
    int pid = Utils::readInt("Parcel ID: ", 1, 999999);
    int d = findDeliveryByParcel(pid);
    if (d < 0) throw CourierException("No delivery record found for Parcel ID " + std::to_string(pid) + ".");
    std::cout << "Current status: " << Delivery::statusToString(deliveries[d].getStatus()) << '\n'
              << "New status: 1. Picked Up  2. In Transit  3. Out For Delivery  4. Delivered  5. Cancelled\n";
    int c = Utils::readInt("Choice (1-5): ", 1, 5);
    static const DeliveryStatus map[] = {DeliveryStatus::PICKED_UP, DeliveryStatus::IN_TRANSIT,
                                         DeliveryStatus::OUT_FOR_DELIVERY, DeliveryStatus::DELIVERED,
                                         DeliveryStatus::CANCELLED};
    std::string date = Utils::readDate("Date DD/MM/YYYY (blank = today): ");
    deliveries[d].updateStatus(map[c - 1], date);
    std::cout << "Status updated. Parcel " << pid << " is now: "
              << Delivery::statusToString(deliveries[d].getStatus()) << '\n';
}

void CourierSystem::trackParcel() const {
    std::cout << "\n--- Track Parcel ---\n";
    int pid = Utils::readInt("Parcel ID: ", 1, 999999);
    int p = findParcel(pid);
    if (p < 0) throw CourierException("Parcel ID " + std::to_string(pid) + " not found.");
    int d = findDeliveryByParcel(pid);
    int s = findCustomer(parcels[p]->getSenderId());
    line('-', 52);
    std::cout << " Parcel ID     : " << pid << " (" << parcels[p]->getType() << ", "
              << std::fixed << std::setprecision(2) << parcels[p]->getWeight() << " kg)\n"
              << " Sender        : " << (s >= 0 ? customers[s].getName() : std::string("(customer removed)"))
              << " [ID " << parcels[p]->getSenderId() << "]\n"
              << " Receiver      : " << parcels[p]->getReceiverName() << '\n'
              << " Charge        : " << Utils::money(parcels[p]->calculateCharge()) << '\n';
    if (d >= 0) {
        int c = findCourier(deliveries[d].getCourierId());
        std::cout << " Delivery ID   : " << deliveries[d].getDeliveryId() << '\n'
                  << " Status        : " << Delivery::statusToString(deliveries[d].getStatus()) << '\n'
                  << " Courier       : ";
        if (deliveries[d].getCourierId() == 0) std::cout << "Not assigned yet\n";
        else if (c < 0) std::cout << "(courier removed)\n";
        else std::cout << couriers[c].getName() << " (" << couriers[c].getPhone() << ")\n";
        std::cout << " Booked on     : " << deliveries[d].getDate() << '\n'
                  << " Last update   : " << deliveries[d].getLastUpdate() << '\n';
    }
    line('-', 52);
}

// ====================================================================== 7. REPORT
void CourierSystem::generateReport() const {
    std::cout << "\n";
    line('=', 62);
    std::cout << "                 COURIER MANAGEMENT SYSTEM REPORT\n";
    line('=', 62);
    std::cout << "Customers: " << customerCount << "   Couriers: " << courierCount
              << "   Parcels: " << parcelCount << "   Deliveries: " << deliveryCount << "\n\n";

    // ---- parcels by type (polymorphic calls) ----
    const char* types[] = {"Standard", "Express", "Fragile"};
    int    cnt[3] = {0, 0, 0};
    double rev[3] = {0, 0, 0};
    double totalRevenue = 0;
    for (int i = 0; i < parcelCount; ++i) {
        int d = findDeliveryByParcel(parcels[i]->getParcelId());
        if (d >= 0 && deliveries[d].getStatus() == DeliveryStatus::CANCELLED) continue;
        for (int t = 0; t < 3; ++t)
            if (parcels[i]->getType() == types[t]) {
                ++cnt[t];
                rev[t] += parcels[i]->calculateCharge();   // runtime polymorphism
            }
        totalRevenue += parcels[i]->calculateCharge();
    }
    std::cout << "Parcels by type (cancelled excluded)\n";
    std::cout << std::left << std::setw(12) << "  Type" << std::setw(8) << "Count" << "Revenue\n";
    for (int t = 0; t < 3; ++t)
        std::cout << "  " << std::left << std::setw(10) << types[t] << std::setw(8) << cnt[t]
                  << Utils::money(rev[t]) << '\n';
    std::cout << "  Total revenue: " << Utils::money(totalRevenue) << "\n\n";

    // ---- deliveries by status ----
    std::cout << "Deliveries by status\n";
    for (int s = 0; s <= 6; ++s) {
        int n = 0;
        for (int i = 0; i < deliveryCount; ++i) if (static_cast<int>(deliveries[i].getStatus()) == s) ++n;
        std::cout << "  " << std::left << std::setw(18) << Delivery::statusToString(static_cast<DeliveryStatus>(s)) << n << '\n';
    }

    // ---- courier workload ----
    std::cout << "\nCourier workload\n";
    if (courierCount == 0) std::cout << "  (no couriers)\n";
    else std::cout << "  " << std::left << std::setw(7) << "ID" << std::setw(27) << "Name"
                   << std::setw(8) << "Active" << "Delivered\n";
    for (int c = 0; c < courierCount; ++c) {
        int active = 0, done = 0;
        for (int i = 0; i < deliveryCount; ++i) {
            if (deliveries[i].getCourierId() != couriers[c].getId()) continue;
            if (deliveries[i].getStatus() == DeliveryStatus::DELIVERED) ++done;
            else if (deliveries[i].isActive()) ++active;
        }
        std::cout << "  " << std::left << std::setw(7) << couriers[c].getId() << std::setw(27)
                  << couriers[c].getName() << std::setw(8) << active << done << '\n';
    }

    // ---- customer billing ----
    std::cout << "\nCustomer billing (cancelled excluded)\n";
    if (customerCount == 0) std::cout << "  (no customers)\n";
    else std::cout << "  " << std::left << std::setw(7) << "ID" << std::setw(27) << "Name"
                   << std::setw(9) << "Parcels" << "Amount\n";
    for (int c = 0; c < customerCount; ++c) {
        int n = 0; double amt = 0;
        for (int i = 0; i < parcelCount; ++i) {
            if (parcels[i]->getSenderId() != customers[c].getId()) continue;
            int d = findDeliveryByParcel(parcels[i]->getParcelId());
            if (d >= 0 && deliveries[d].getStatus() == DeliveryStatus::CANCELLED) continue;
            ++n; amt += parcels[i]->calculateCharge();
        }
        std::cout << "  " << std::left << std::setw(7) << customers[c].getId() << std::setw(27)
                  << customers[c].getName() << std::setw(9) << n << Utils::money(amt) << '\n';
    }
    line('=', 62);
}

// ====================================================================== 8. SAVE / LOAD
void CourierSystem::saveData() const {
    fileManager.save(*this);
    std::cout << "Data saved to folder '" << fileManager.getDirectory() << "' ("
              << totalRecords() << " record(s)).\n";
}

void CourierSystem::loadData() {
    int n = fileManager.load(*this);
    if (n == 0) std::cout << "No saved records found in '" << fileManager.getDirectory() << "' (starting empty).\n";
    else std::cout << "Loaded " << n << " record(s) from '" << fileManager.getDirectory() << "': "
                   << customerCount << " customer(s), " << courierCount << " courier(s), "
                   << parcelCount << " parcel(s), " << deliveryCount << " delivery record(s).\n";
}
