#include <QuartzIO/modules/CpuModule.h>
#include <QuartzIO/datatypes.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <thread> // very much saved me tho don't really know much about it just copy pasting it's implementation.
#include <chrono>
#include <iomanip>
#include <vector>

struct CpuTimes {
    long long total_time = 0;
    long long idle_time = 0;
};

CpuTimes get_cpu_times() {
    std::ifstream proc_stat("/proc/stat");
    std::string line;

    if (proc_stat && std::getline(proc_stat, line)) {
        std::stringstream ss(line);
        std::string cpu_label;
        ss >> cpu_label; // initial cpu label

        if (cpu_label == "cpu") {
            std::vector<long long> times;
            long long time_val;
            while (ss >> time_val) {
                times.push_back(time_val);
            }

            // For quick reference this is from linux docs
            // The meanings of the columns are as follows, from left to right:

            // user: normal processes executing in user mode
            // nice: niced processes executing in user mode
            // system: processes executing in kernel mode
            // idle: twiddling thumbs
            // iowait: waiting for I/O to complete
            // irq: servicing interrupts
            // softirq: servicing softirqs
            // Sample output ( cat proc/stat is super confusing to get useful out of it atleast for me)

          /*> cat /proc/stat
            cpu  2255 34 2290 22625563 6290 127 456
            cpu0 1132 34 1441 11311718 3675 127 438
            cpu1 1123 0 849 11313845 2614 0 18
            intr 114930548 113199788 3 0 5 263 0 4 [... lots more numbers ...]
            ctxt 1990473
            btime 1062191376
            processes 2915
            procs_running 1
            procs_blocked 0
            */
            // We consider 'idle' and iowait part as non-busy 
            long long idle = times.size() > 3 ? times[3] : 0;
            long long iowait = times.size() > 4 ? times[4] : 0;
            long long idle_time = idle + iowait;

            long long total_time = 0;
            for (long long t : times) {
                total_time += t;
            }
            return {total_time, idle_time};

        }
    }
    return {};

}

const char* CpuModule::getFlag() const {
    return "--cpu";
}

std::string trimc(const std::string &s)
{
    size_t first = s.find_first_not_of(" \t\n\r");
    if (std::string::npos == first)
    {
        /* code */
        return s;
    }
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, last - first + 1);
}

void CpuModule::run(const Options& opts) {
    CpuInfo info;

    std::ifstream cpu_file("/proc/cpuinfo");
    if(!cpu_file.is_open()) {
        std::cerr << "Error: Could not open /proc/cpuinfo\n";
    }

    std::set<int> core_ids;
    bool model_found = false;
    bool vendor_found = false;
    std::string line;

    while (std::getline(cpu_file, line)) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            continue; // skipping lines without colon too much hassle or could be empty
        }
        
        std::string key = line.substr(0, colon_pos);
        std::string value = line.substr(colon_pos+1);

        key = trimc(key);
        value = trimc(value);

        if (key == "processor")
        {
            info.logical_cores++;
        }
        else if (key == "model name" && !model_found)
        {
            info.model_name = value;
            model_found = true;
        }
        else if (key == "vendor_id" && !vendor_found)
        {
            info.vendor_id = value;
            vendor_found = true;
        }
        else if (key == "core_id")
        {
            // very clever i know , aren't i a genius.( laughing narcissisticly)

            try {
                core_ids.insert(std::stoi(value));
            } catch (...) {
                // don't really care.
            }
        }
    }

    cpu_file.close(); // gentlemen behaviour

    info.physical_cores = core_ids.size(); // just no. of unique id. forward thinking not like i got wrong output before without knowing the reason nuhuh definitely got correct in first time.
    if (info.physical_cores == 0)
    {
        info.physical_cores = info.logical_cores; // the duct tape of this pipeline.
    }


    // calculating cpu utilization as that is cool thing to do.
    CpuTimes start_times = get_cpu_times();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    CpuTimes end_times = get_cpu_times();

    long long total_delta = end_times.total_time - start_times.total_time; // started using long long instead of just uintmax_t aren't i generous.
    long long idle_delta = end_times.idle_time - start_times.idle_time;

    if (total_delta > 0)
    {
        double busy_ratio = 1.0 - (static_cast<double>(idle_delta) / total_delta);
        info.utilization = busy_ratio * 100.0;
    }

    // Using AI for printing for God's shake i just can't do this much text formatting.

    if (opts.format == OutputFormat::Json) {
        std::cout << "{\n"
                  << "  \"vendor_id\": \"" << info.vendor_id << "\",\n"
                  << "  \"model_name\": \"" << info.model_name << "\",\n"
                  << "  \"physical_cores\": " << info.physical_cores << ",\n"
                  << "  \"logical_cores\": " << info.logical_cores << ",\n"
                  << "  \"utilization_percent\": " << std::fixed << std::setprecision(2) << info.utilization << "\n"
                  << "}\n";
        return;
    }

    if (opts.format == OutputFormat::Raw) {
        std::cout << "cpu.vendor=" << info.vendor_id << "\n";
        std::cout << "cpu.model=" << info.model_name << "\n";
        std::cout << "cpu.cores.physical=" << info.physical_cores << "\n";
        std::cout << "cpu.cores.logical=" << info.logical_cores << "\n";
        std::cout << "cpu.utilization_percent=" << std::fixed << std::setprecision(2) << info.utilization << "\n";
        return;
    }

    // Default pretty-printed output
    std::cout << "--- === CPU Information === ---\n";
    std::cout << "Vendor:       " << info.vendor_id << "\n";
    std::cout << "Model:        " << info.model_name << "\n";
    std::cout << "Cores:        " << info.physical_cores << " (Physical) / " << info.logical_cores << " (Logical)\n";
    std::cout << "Utilization:  " << std::fixed << std::setprecision(2) << info.utilization << " %\n";
    std::cout << std::string(31, '-') << "\n";

    // Praise the omnissiah forgive me using Abominable Intelligence.
}