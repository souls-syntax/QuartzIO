// QuartzIO let's go
#include "command.h"
#include "lsblk_parser.h"
#include <iostream>
#include <vector>

// Main function to just connect pipelines
int main() {
    try
    {
        std::string cmd_output = exec("lsblk -d -n -o NAME,MODEL,SIZE");

        if (cmd_output.empty()) 
        {
            std::cerr << "No storage device found or lsblk command failed." << "\n";
            return 1;
        }

        std::cout << "--- Parsing lsblk output ---" << "\n";

        std::vector<DiskInfo> detected_disks = parse_lsblk(cmd_output);

        for (const auto& disk: detected_disks)
        {
            /* code */
            std::cout << "Device: /dev/" << disk.name << "\n";
            std::cout << " Model: " << (disk.model.empty() ? "N/A" : disk.model) << "\n";
            std::cout << "  Size: " << disk.size << "\n";
            std::cout << "======================================" << "\n";
        }
        
        
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}