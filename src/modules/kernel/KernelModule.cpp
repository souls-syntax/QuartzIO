#include <QuartzIO/modules/KernelModule.h>
#include <QuartzIO/datatypes.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


std::string parse_os_release() {
    std::ifstream file("/etc/os-release");
    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("PRETTY_NAME=", 0)==0)
        {
            std::string value = line.substr(line.find("=" + 1));
            //remove quotes as they spoil
            if (value.front() == '"' && value.back() == '"')
            {
                value = value.substr(1, value.length() -2);
            }
            return value;
        }
    }
    return "N/A";
    
}

std::string format_uptime() {
    std::ifstream file("/proc/uptime");
    double uptime_seconds;
    if(file >> uptime_seconds) {
        long seconds = static_cast<long>(uptime_seconds);
        long days = seconds / 86400;
        seconds %= 86400;
        long hours = seconds/3600;
        seconds %= 3600;
        long minutes = seconds / 60;

        std::stringstream ss;
        if (days > 0) ss << days << " days, ";
        if (hours > 0) ss << hours << " hours, ";
        ss << minutes << " minutes";
        return ss.str();
    }
    return "N/A";
}


const char* KernelModule::getFlag() const {
    return "--kernel";
}

void KernelModule::run(const Options& opts) {
    KernelInfo info;

    info.os_release = parse_os_release();

    std::ifstream Kernel_file("/proc/version");
    if (Kernel_file.is_open())
    {
        std::getline(Kernel_file, info.kernel_version);
    } else {
        info.kernel_version = "N/A";
    }
    info.uptime = format_uptime();
    
    std::ifstream file("/proc/uptime");
    double raw_seconds;
    file >> raw_seconds;
    info.uptime_seconds = raw_seconds;

    //---The worst work printing logic in 3 ways aaah is there any smarter way---//
    
    if (opts.format == OutputFormat::Raw) {
        std::cout << "os.release=" << info.os_release << "\n";
        std::cout << "kernel.version=" << info.kernel_version << "\n";
        std::cout << "system.uptime=" << info.uptime_seconds << "\n";
        return;
    }
    
    // Woohoo just gave chatgpt to write it and then copy pasted it hmm is this cheating surely not.
    
    if (opts.format == OutputFormat::Json) {
        std::cout << "{\n"
        << "  \"os_release\": \"" << info.os_release << "\",\n"
        << "  \"kernel_version\": \"" << info.kernel_version << "\",\n"
        << "  \"uptime\": \"" << info.uptime_seconds << "\"\n"
        << "}\n";
        return;
    }

    std::cout << "--- === System & Kernel Information === ---\n";
    std::cout << "Operating System: " << info.os_release << "\n";
    std::cout << "Kernel Version:   " << info.kernel_version << "\n";
    std::cout << "System Uptime:    " << info.uptime << "\n";
    std::cout << std::string(45, '-') << "\n";
}