#include <QuartzIO/modules/FileSystemModule.h>
#include <QuartzIO/datatypes.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

std::string format_bytes(uintmax_t bytes)
{
    if (bytes == 0)
        return "0.0 B";
    const char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double d_bytes = bytes;
    while (d_bytes >= 1024 && i < 4)
    {
        d_bytes /= 1024;
        i++;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << d_bytes << " " << suffix[i];
    return ss.str();
}

const char *FileSystemModule::getFlag() const
{
    return "--fs";
}

void FileSystemModule::run(const Options &opts)
{

    std::vector<MountInfo> mounts;

    std::ifstream mounts_file("/proc/mounts");
    std::string line;

    while (std::getline(mounts_file, line))
    {
        std::istringstream iss(line);
        std::string device, mount_point, type;
        iss >> device >> mount_point >> type;

        // Don't need devices which are virtual or unreal.
        if (device.rfind("/dev/", 0) != 0)
        {
            continue;
        }

        std::error_code ec;
        auto space_info = std::filesystem::space(mount_point, ec);

        // If don't have the power to check it like encrypted of permission then skip.
        if (ec)
        {
            continue;
        }

        MountInfo current_mount;
        current_mount.mount_point = mount_point;
        current_mount.device = device;
        current_mount.total_bytes = space_info.capacity;
        current_mount.available_bytes = space_info.available;
        current_mount.used_bytes = current_mount.total_bytes - current_mount.available_bytes;
        current_mount.use_percentage = (current_mount.total_bytes > 0) ? (100 * current_mount.used_bytes / current_mount.total_bytes) : 0;

        mounts.push_back(current_mount);

        // std::cout << std::left << std::setw(25) << "Filesystem"
        //           << std::setw(15) << "Size"
        //           << std::setw(15) << "Used"
        //           << std::setw(15) << "Available"
        //           << std::setw(10) << "Use%" << std::endl;
        // std::cout << std::string(80, '-') << std::endl;
        // std::cout << std::left << std::setw(25) << mount_point
        //           << std::setw(15) << format_bytes(total)
        //           << std::setw(15) << format_bytes(used)
        //           << std::setw(15) << format_bytes(available)
        //           << std::setw(3) << use_present << "%" << "\n";
    }

    if (opts.format == OutputFormat::Json) {
        std::cout << "{\n  \"filesystems\": [\n";
        bool first = true;
        for (const auto& mount : mounts) {
            if (!first) std::cout << ",\n";
            std::cout << "    {\n";
            std::cout << "      \"mount_point\": \"" << mount.mount_point << "\",\n";
            std::cout << "      \"device\": \"" << mount.device << "\",\n";
            std::cout << "      \"size_bytes\": " << mount.total_bytes << ",\n";
            std::cout << "      \"used_bytes\": " << mount.used_bytes << ",\n";
            std::cout << "      \"available_bytes\": " << mount.available_bytes << ",\n";
            std::cout << "      \"use_percentage\": " << mount.use_percentage << "\n";
            std::cout << "    }";
            first = false;
        }
        std::cout << "\n  ]\n}\n";
        return;
    }

    if (opts.format == OutputFormat::Raw) {
        for (const auto& mount : mounts) {
            // Use a sanitized key for the mount point to be safe.
            std::string key_prefix = mount.mount_point;
            if (key_prefix == "/") key_prefix = "root";
            else key_prefix.erase(0, 1); // remove leading '/'
            std::replace(key_prefix.begin(), key_prefix.end(), '/', '_');

            std::cout << key_prefix << ".device=" << mount.device << "\n";
            std::cout << key_prefix << ".size=" << format_bytes(mount.total_bytes) << "\n";
            std::cout << key_prefix << ".used=" << format_bytes(mount.used_bytes) << "\n";
            std::cout << key_prefix << ".available=" << format_bytes(mount.available_bytes) << "\n";
            std::cout << key_prefix << ".use_percentage=" << mount.use_percentage << "%\n";
        }
        return;
    }

    // Default, pretty-printed table output
    std::cout << std::left << std::setw(25) << "Filesystem"
              << std::setw(15) << "Size"
              << std::setw(15) << "Used"
              << std::setw(15) << "Available"
              << std::setw(10) << "Use%" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (const auto& mount : mounts) {
        std::cout << std::left << std::setw(25) << mount.mount_point
                  << std::setw(15) << format_bytes(mount.total_bytes)
                  << std::setw(15) << format_bytes(mount.used_bytes)
                  << std::setw(15) << format_bytes(mount.available_bytes)
                  << std::setw(3) << mount.use_percentage << "%" << "\n";
    }
}