// FileManager.cpp
#include "FileManager.h"
#include "CourierException.h"
#include "CourierSystem.h"
#include "Utils.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

FileManager::FileManager(const std::string& directory) : dataDir(directory) {}
FileManager::~FileManager() {}

const std::string& FileManager::getDirectory() const { return dataDir; }

std::string FileManager::path(const std::string& fileName) const {
    return dataDir + "/" + fileName;
}

void FileManager::save(const CourierSystem& s) const {
    std::filesystem::create_directories(dataDir);

    std::ofstream cf(path("customers.txt"));
    std::ofstream kf(path("couriers.txt"));
    std::ofstream pf(path("parcels.txt"));
    std::ofstream df(path("deliveries.txt"));
    std::ofstream mf(path("meta.txt"));
    if (!cf || !kf || !pf || !df || !mf)
        throw CourierException("Could not open data files for writing in folder '" + dataDir + "'.");

    for (int i = 0; i < s.customerCount; ++i) cf << s.customers[i].serialize() << '\n';
    for (int i = 0; i < s.courierCount; ++i)  kf << s.couriers[i].serialize() << '\n';
    for (int i = 0; i < s.parcelCount; ++i)   pf << s.parcels[i]->serialize() << '\n';
    for (int i = 0; i < s.deliveryCount; ++i) df << s.deliveries[i].serialize() << '\n';
    mf << s.nextCustomerId << '\n' << s.nextCourierId << '\n'
       << s.nextParcelId << '\n' << s.nextDeliveryId << '\n';

    if (!cf || !kf || !pf || !df || !mf)
        throw CourierException("Error while writing data files.");
}

int FileManager::load(CourierSystem& s) const {
    s.clearAll();
    int loaded = 0, skipped = 0;
    std::string line;

    {   std::ifstream in(path("customers.txt"));
        while (std::getline(in, line)) {
            if (Utils::trim(line).empty()) continue;
            try { s.appendCustomer(Customer::deserialize(line)); ++loaded; }
            catch (const CourierException& e) { std::cout << "  [Load warning] " << e.what() << '\n'; ++skipped; }
        } }
    {   std::ifstream in(path("couriers.txt"));
        while (std::getline(in, line)) {
            if (Utils::trim(line).empty()) continue;
            try { s.appendCourier(Courier::deserialize(line)); ++loaded; }
            catch (const CourierException& e) { std::cout << "  [Load warning] " << e.what() << '\n'; ++skipped; }
        } }
    {   std::ifstream in(path("parcels.txt"));
        while (std::getline(in, line)) {
            if (Utils::trim(line).empty()) continue;
            try { s.appendParcel(Parcel::deserialize(line)); ++loaded; }
            catch (const CourierException& e) { std::cout << "  [Load warning] " << e.what() << '\n'; ++skipped; }
        } }
    {   std::ifstream in(path("deliveries.txt"));
        while (std::getline(in, line)) {
            if (Utils::trim(line).empty()) continue;
            try { s.appendDelivery(Delivery::deserialize(line)); ++loaded; }
            catch (const CourierException& e) { std::cout << "  [Load warning] " << e.what() << '\n'; ++skipped; }
        } }

    // restore ID counters (never lower than the highest id found in the files)
    {   std::ifstream in(path("meta.txt"));
        int a, b, c, d;
        if (in >> a >> b >> c >> d) {
            if (a > s.nextCustomerId) s.nextCustomerId = a;
            if (b > s.nextCourierId)  s.nextCourierId = b;
            if (c > s.nextParcelId)   s.nextParcelId = c;
            if (d > s.nextDeliveryId) s.nextDeliveryId = d;
        } }
    if (skipped > 0) std::cout << "  " << skipped << " damaged record(s) were skipped.\n";
    return loaded;
}
