// FileManager.h - saves / loads every record to text files inside the Data folder
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

class CourierSystem;      // forward declaration (CourierSystem declares FileManager as a friend)

class FileManager {
private:
    std::string dataDir;

    std::string path(const std::string& fileName) const;

public:
    explicit FileManager(const std::string& directory = "Data");
    ~FileManager();

    const std::string& getDirectory() const;

    void save(const CourierSystem& system) const;   // writes customers/couriers/parcels/deliveries/meta
    int  load(CourierSystem& system) const;         // returns number of records read (0 on first run)
};

#endif
