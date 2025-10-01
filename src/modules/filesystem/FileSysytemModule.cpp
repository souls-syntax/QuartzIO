#include <QuartzIO/modules/FileSystemModule.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

std::string format_bytes(uintmax_t bytes) {
    if (bytes == 0) return "0.0 B";
    const char* suffix[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double d_bytes = bytes;
    while (d_bytes >= 1024 && i<4) {
        d_bytes /= 1024;
        i++;
    }
    std::stringstream ss;
    ss<< std::fixed << std::setprecision(2) << d_bytes << " " << suffix[i];
    return ss.str();
}

const char* FileSystemModule::getFlag() const {
    return "--fs";
}

void FileSystemModule::run() {
    std::cout << std::left << std::setw(25) << "Filesystem"
              << std::setw(15) << "Size"
              << std::setw(15) << "Used"
              << std::setw(15) << "Available"
              << std::setw(10) << "Use%" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    std::ifstream mounts_file("/proc/mounts");
    std::string line;

    while (std::getline(mounts_file, line)) {
        std::istringstream iss(line);
        std::string device, mount_point , type;
        iss >> device >> mount_point >> type;


        // Don't need devices which are virtual or unreal.
        if(device.rfind("/dev/" , 0) != 0) {
            continue;
        }

        std::error_code ec;
        auto space_info = std::filesystem::space(mount_point, ec);

        // If don't have the power to check it like encrypted of permission then skip.
        if (ec) {
            continue;
        }

        uintmax_t total = space_info.capacity;
        uintmax_t available = space_info.available;
        uintmax_t used = total - available;
        int use_present = (total > 0) ? (100 * used / total) : 0;

        std::cout << std::left << std::setw(25) << mount_point
                  << std::setw(15) << format_bytes(total)
                  << std::setw(15) << format_bytes(used)
                  << std::setw(15) << format_bytes(available)
                  << std::setw(3) << use_present << "%" << "\n";
    }
}