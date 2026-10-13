// main.cpp - creates the system object, shows the menus and calls CourierSystem functions.
// Usage:  CourierSystem [data-folder] [--echo]
#include "CourierException.h"
#include "CourierSystem.h"
#include "Utils.h"

#include <iostream>
#include <string>

namespace {

void showMainMenu() {
    std::cout << "\n============================================================\n"
              << "            COURIER MANAGEMENT SYSTEM  (IU2541230073)\n"
              << "============================================================\n"
              << " 1. Add Record\n"
              << " 2. Display Records\n"
              << " 3. Search Record\n"
              << " 4. Update Record\n"
              << " 5. Delete Record\n"
              << " 6. Main Transaction (Book / Assign / Status / Track)\n"
              << " 7. Report\n"
              << " 8. Save / Load\n"
              << " 9. Exit\n"
              << "------------------------------------------------------------\n";
}

void addMenu(CourierSystem& s) {
    std::cout << "\n-- Add Record --\n 1. Customer\n 2. Courier\n 3. Parcel (book)\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 3)) {
        case 1: s.addCustomer(); break;
        case 2: s.addCourier();  break;
        case 3: s.bookParcel();  break;
        default: break;
    }
}

void displayMenu(CourierSystem& s) {
    std::cout << "\n-- Display Records --\n 1. Customers\n 2. Couriers\n 3. Parcels\n"
              << " 4. Deliveries\n 5. All persons (Customers + Couriers)\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 5)) {
        case 1: s.displayCustomers();  break;
        case 2: s.displayCouriers();   break;
        case 3: s.displayParcels();    break;
        case 4: s.displayDeliveries(); break;
        case 5: s.displayAllPersons(); break;
        default: break;
    }
}

void searchMenu(CourierSystem& s) {
    std::cout << "\n-- Search Record --\n 1. Customer\n 2. Courier\n 3. Parcel\n"
              << " 4. Deliveries by status\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 4)) {
        case 1: s.searchCustomer();         break;
        case 2: s.searchCourier();          break;
        case 3: s.searchParcel();           break;
        case 4: s.searchDeliveryByStatus(); break;
        default: break;
    }
}

void updateMenu(CourierSystem& s) {
    std::cout << "\n-- Update Record --\n 1. Customer\n 2. Courier\n 3. Parcel\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 3)) {
        case 1: s.updateCustomer(); break;
        case 2: s.updateCourier();  break;
        case 3: s.updateParcel();   break;
        default: break;
    }
}

void deleteMenu(CourierSystem& s) {
    std::cout << "\n-- Delete Record --\n 1. Customer\n 2. Courier\n 3. Parcel (with delivery)\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 3)) {
        case 1: s.deleteCustomer(); break;
        case 2: s.deleteCourier();  break;
        case 3: s.deleteParcel();   break;
        default: break;
    }
}

void transactionMenu(CourierSystem& s) {
    std::cout << "\n-- Main Transaction --\n 1. Book parcel\n 2. Assign courier\n"
              << " 3. Update delivery status\n 4. Track parcel\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 4)) {
        case 1: s.bookParcel();           break;
        case 2: s.assignCourier();        break;
        case 3: s.updateDeliveryStatus(); break;
        case 4: s.trackParcel();          break;
        default: break;
    }
}

void saveLoadMenu(CourierSystem& s) {
    std::cout << "\n-- Save / Load --\n 1. Save data now\n 2. Load data from files\n 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 2)) {
        case 1: s.saveData(); break;
        case 2:
            if (Utils::readYesNo("Loading replaces records in memory. Continue? (y/n): ")) s.loadData();
            break;
        default: break;
    }
}

} // namespace

int main(int argc, char* argv[]) {
    std::string dataDir = "Data";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--echo") Utils::echoInput = true;
        else dataDir = a;
    }

    CourierSystem system(dataDir);                       // object lives on the stack; arrays inside are on the heap
    std::cout << "Loading saved records...\n";
    try { system.loadData(); }
    catch (const std::exception& e) { std::cout << "Could not load data: " << e.what() << '\n'; }

    bool running = true;
    while (running) {
        try {
            showMainMenu();
            switch (Utils::readInt("Enter your choice (1-9): ", 1, 9)) {
                case 1: addMenu(system);         break;
                case 2: displayMenu(system);     break;
                case 3: searchMenu(system);      break;
                case 4: updateMenu(system);      break;
                case 5: deleteMenu(system);      break;
                case 6: transactionMenu(system); break;
                case 7: system.generateReport(); break;
                case 8: saveLoadMenu(system);    break;
                case 9: running = false;         break;
            }
        } catch (const CourierException& e) {
            std::cout << "  ERROR: " << e.what() << '\n';
        } catch (const Utils::InputEnded&) {
            std::cout << "\n(Input closed - exiting)\n";
            running = false;
        } catch (const std::exception& e) {
            std::cout << "  Unexpected error: " << e.what() << '\n';
        }
    }

    try { system.saveData(); }                           // automatic save on exit
    catch (const std::exception& e) { std::cout << "Could not save data: " << e.what() << '\n'; }
    std::cout << "Thank you for using the Courier Management System. Goodbye!\n";
    return 0;                                            // ~CourierSystem() releases all heap memory
}
