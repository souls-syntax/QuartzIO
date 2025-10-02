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


enum class OutputFormat {
    Default, // The normal pretty-printed table
    Raw,     // The simple key=value format
    Json     // The JSON format
};

struct Options {
    OutputFormat format = OutputFormat::Default;
};