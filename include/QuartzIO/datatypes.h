#pragma once
#include <string>
#include <cstdint>
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


struct MountInfo {
    std::string mount_point;
    std::string device;
    uintmax_t total_bytes;
    uintmax_t used_bytes;
    uintmax_t available_bytes;
    int use_percentage;
};

struct KernelInfo {
    std::string os_release;
    std::string kernel_version;
    std::string uptime;
    uintmax_t      uptime_seconds;
};


struct CpuInfo {
    std::string model_name;
    std::string vendor_id;
    int logical_cores = 0;
    int physical_cores = 0;
    double utilization = 0.0;
};

enum class OutputFormat {
    Default, // The normal pretty-printed table
    Raw,     // The simple key=value format
    Json     // The JSON format
};

struct Options {
    OutputFormat format = OutputFormat::Default;
};