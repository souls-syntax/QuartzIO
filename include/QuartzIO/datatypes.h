#pragma once
#include <string>

// Holds disk information collected from lsblk and smartctl
struct DiskInfo {
    std::string name;
    std::string model;
    std::string size;
    std::string serialNumber;
    std::string firmwareVersion;
    std::string health;
    std::string temperature;
    std::string powerOnHours;
};
