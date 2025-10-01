#include <QuartzIO/modules/MemoryModule.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <cstdint>

using std::setw;

std::string format_kb(uintmax_t bytes) {
    if (bytes == 0) return "0.0 B";
    const char* suffix[] = {"KB", "MB", "GB", "TB"};
    int i = 0;
    double d_bytes = bytes;
    while (d_bytes >= 1024 && i<3) {
        d_bytes /= 1024;
        i++;
    }
    std::stringstream ss;
    ss<< std::fixed << std::setprecision(2) << d_bytes << " " << suffix[i];
    return ss.str();
}


const char* MemoryModule::getFlag() const {
    return "--mem"
;}

void MemoryModule::run() {
    std::ifstream mem_file("/proc/meminfo");
    if (!mem_file.is_open()) 
    {
        std::cerr << "Error: Could not open /proc/meminfo\n";
        return;
    }
    
    std::map<std::string, long> mem_data;
    std::string line;

    while (std::getline(mem_file, line))
    {
        std::stringstream ss(line);
        std::string key;
        long value;
        ss >> key >> value;
        key.pop_back(); //to remove colon
        mem_data[key] = value; // hmm skill is improving using map instead of just struct woohoo
    }

    uintmax_t total_mem = mem_data["MemTotal"];
    uintmax_t free_mem = mem_data["MemFree"];
    uintmax_t available_mem = mem_data["MemAvailable"];
    uintmax_t used_mem = total_mem - available_mem;

    uintmax_t total_swap = mem_data["SwapTotal"];
    uintmax_t free_swap = mem_data["SwapFree"];
    uintmax_t used_swap = total_swap - free_swap;

    //==printing the output//

    std::cout << std::setw(8) << "RAM:"
              << std::setw(15) << "Total"
              << std::setw(15) << "Used"
              << std::setw(15) << "Available\n";
    
    std::cout << std::setw(8) << "" 
              << std::setw(15) << "7.50 GB" 
              << std::setw(15) << "5.38 GB" 
              << std::setw(15) << "2.12 GB"  
              << "\n\n";

    
    std::cout << std::setw(8) << "Swap:"
              << std::setw(15) << "Total"
              << std::setw(15) << "Used"
              << std::setw(15) << "Free\n";

    std::cout << std::setw(8) << "" 
              << std::setw(15) << "3.75 GB" 
              << std::setw(15) << "2.04 GB" 
              << std::setw(15) << "1.71 GB" 
              << "\n";

    std::cout << std::string(60, '-') << "\n";
}

