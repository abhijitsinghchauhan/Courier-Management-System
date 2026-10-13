# Courier Management System (C++ Console OOP Project)

**Enrollment No.:** IU2541230073  |  **Name:** Abhijit Chauhan  |  **Semester:** 3  |  **Course Code:** CPP48  |  **Date of Submission:** 13/10/2026

## 1. About
A console-based Courier Management System written in C++17. It lets an operator register customers and
couriers, book parcels (charge calculated automatically by parcel type), assign couriers, track the
delivery status of every parcel, search / update / delete records, view reports and keep all data
safe in text files between runs.

## 2. Features
* Menu driven: Add, Display, Search, Update, Delete, Main Transaction, Report, Save / Load, Exit
* Three parcel types with their own charge rules (runtime polymorphism): Standard, Express, Fragile
* Delivery life-cycle: Booked -> Assigned -> Picked Up -> In Transit -> Out For Delivery -> Delivered (or Cancelled)
* Input validation (names, 10-digit phone, weight 0.1-50 kg, DD/MM/YYYY dates, duplicate phone numbers)
* Exception handling with a custom `CourierException`
* Automatic load at start-up and automatic save on exit (`Data/` folder)

## 3. Folder structure
| Path | Description |
|---|---|
| `main.cpp` | Creates the `CourierSystem` object, shows menus, calls system functions |
| `Person.*`, `Customer.*`, `Courier.*` | Base class and its two derived classes |
| `Parcel.*`, `ParcelTypes.h` | Abstract `Parcel` + `StandardParcel`, `ExpressParcel`, `FragileParcel` |
| `Delivery.*` | Delivery record and status rules |
| `CourierSystem.*` | Controller: dynamic arrays and every operation |
| `FileManager.*` | `save()` / `load()` of all records |
| `Utils.*`, `CourierException.h` | Validation, input helpers, custom exception |
| `Data/` | `customers.txt`, `couriers.txt`, `parcels.txt`, `deliveries.txt`, `meta.txt` (sample data included) |
| `UML/` | Use case, class and two sequence diagrams (PNG) |
| `Screenshots/` | Program screenshots |
| `Tests/test_results.md` | 15 executed test cases |
| `CourierSystem` | Pre-built Linux executable |

## 4. How to compile and run
Requires a C++17 compiler (g++ 8 or newer / MinGW-w64 / Visual Studio 2019+).

```bash
# Linux / macOS / MinGW (Windows)
g++ -std=c++17 -Wall -Wextra -o CourierSystem main.cpp Utils.cpp Person.cpp Customer.cpp Courier.cpp Parcel.cpp Delivery.cpp FileManager.cpp CourierSystem.cpp
# or simply
make
```
Run: `./CourierSystem` (Windows: `CourierSystem.exe`).
Optional arguments: `CourierSystem <data-folder>` to use another folder, `--echo` to echo typed input.
In Visual Studio add all `.cpp` files to the project and set *C++ Language Standard = ISO C++17*.

To start with an empty database delete the `.txt` files inside `Data/`.

## 5. Quick demo
1. Menu 1 -> 1 : add a customer. Menu 1 -> 2 : add a courier.
2. Menu 6 -> 1 : book a parcel (note the charge). Menu 6 -> 2 : assign the courier.
3. Menu 6 -> 3 : update status step by step. Menu 6 -> 4 : track the parcel.
4. Menu 7 : report. Menu 9 : exit (data is saved automatically). Start again - the records are loaded back.

## 6. Charge rules
| Type | Formula | Delivery time |
|---|---|---|
| Standard | Rs. 50 + Rs. 20 x kg | 5 days |
| Express | (Rs. 100 + Rs. 35 x kg) x 1.15 | 2 days |
| Fragile | Rs. 80 + Rs. 25 x kg + Rs. 150 handling | 4 days |

## 7. OOP concepts and where they are used
* **Encapsulation** - all data members are private; access through getters / validated setters.
* **Constructors / Destructors** - default + parameterized constructors in every class; virtual destructors in `Person` and `Parcel`; `~CourierSystem()` frees all heap memory.
* **Inheritance** - `Customer`, `Courier` derive from `Person`; `StandardParcel`, `ExpressParcel`, `FragileParcel` derive from `Parcel`.
* **Runtime polymorphism** - `Parcel*` array calls `calculateCharge()` / `display()`; `Person*` array calls `display()`.
* **Dynamic memory** - `new Customer[cap]`, `new Courier[cap]`, `new Parcel*[cap]`, `new Delivery[cap]`, parcels created with `new`; arrays grow by doubling; released with `delete` / `delete[]`.
* **File handling** - `FileManager` uses `ifstream` / `ofstream` (pipe separated text files).
* **Exception handling** - `CourierException` thrown on invalid operations and caught in `main`.
