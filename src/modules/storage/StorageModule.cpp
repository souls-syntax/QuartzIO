#include <QuartzIO/modules/StorageModule.h>
#include <QuartzIO/core/command.h>
#include <QuartzIO/datatypes.h>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

//-------- AI edit credit copilot -------//
#ifdef _WIN32
// Windows does not have getuid(), define a stub or handle accordingly
inline int getuid() { return 0; }
#else
#include <unistd.h>
#endif
//-------- AI edit ends ------//


// Parsing the output of lsblk command.
// The lsblk command we will be executing is "lsblk -d -n -o NAME,MODEL,SIZE"
//result will be like
// zram0                                     3.7G
//nvme0n1 SK hynix BC711 HFHS12GD******13M   476.9G

std::vector<DiskInfo> parse_lsblk(const std::string& lsblk_output) {
    std::vector<DiskInfo> disks;
    std::stringstream ss(lsblk_output);
    std::string line;

    //read output lini by line.
    while (std::getline(ss, line))
    {
        /* code */

        //parsing line word by word
        std::stringstream line_ss(line);
        DiskInfo current_disk;

        //first word is device name
        line_ss >> current_disk.name;

        // the last word is the size
        //the middel part is the model which will result in "" if there is no such thing like vram.
        std::string temp_model;
        std::string current_word;
        std::vector<std::string> words;

        //read all the reamining words in a vector
        while (line_ss >> current_word)
        {
            /* code */
            words.push_back(current_word);
        }
        //skip if no word
        if (words.empty())
        {
            /* code */
            continue;
        }
        // the last element is the size
        current_disk.size = words.back();
        words.pop_back();


        // whatever is left model name likely , join it with spaces.
        std::stringstream model_ss;
        for (size_t i = 0; i < words.size(); ++i)
        {
            /* code */
            model_ss << words[i] << (i==words.size() -1 ? "" : " ");
        }
        current_disk.model = model_ss.str();

        disks.push_back(current_disk);
    }
    return disks;
}

std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    if (std::string::npos == first)
    {
        /* code */
        return s;
    }
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, last -first+1);   
}


// ===SMART parser==== //
void parse_smartctl(const std::string& smartctl_output, DiskInfo& disk) {

    std::stringstream ss(smartctl_output);
    std::string line;

    while (std::getline(ss, line))
    {
        /* code */
        std::stringstream line_ss(line);
        std::string key, value;

        //seperating by colon (:)

        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos)
        {
            /* code */
            key = line.substr(0, colon_pos);
            value = line.substr(colon_pos + 1);

            key = trim(key);
            value = trim(value);

            if (key == "Model Number") disk.model = value;
            else if (key == "Serial Number")
            {
                /* code */
                disk.serialNumber = value;

            }
            else if (key == "Firmware Version")
            {
                /* code */
                disk.firmwareVersion = value;
            }
            else if (key == "SMART overall-health self-assessment test result")
            {
                /* code */
                disk.health = value;
            }
            else if (key == "Temperature" || key == "Temperature Celsius")
            {
                /* code */
                disk.temperature = value;
            }
            else if (key == "Power On Hours")
            {
                /* code */
                disk.powerOnHours = value;
            }
        }
    }
}

// ======Setting the flag for IModule abstraction transform=======//
const char* StorageModule::getFlag() const {
    return "--storage";
}

// ======Printing The Values======= //

void StorageModule::run() {
    std::string lsblk_output = exec("lsblk -d -n -o NAME,MODEL,SIZE");

    auto disks = parse_lsblk(lsblk_output);

    for(auto& disk : disks) {
        bool smart_ok = true;
        if (getuid() != 0) {
        std::cout << "SMART data: (Not available, please run with sudo)\n";
        smart_ok = false;
        
    } else {
        try {
            std::string smartctl_cmd = "smartctl -a /dev/" + disk.name + " 2>&1";
            std::string smartctl_output = exec(smartctl_cmd.c_str());
            if (smartctl_output.find("Permission denied") != std::string::npos ||
                smartctl_output.find("Command not found") != std::string::npos)
            {
                /* code */
                smart_ok = false
            ;}
            else {
                parse_smartctl(smartctl_output, disk);
            }
        }
        catch (...) {
            smart_ok = false;
        }
    }

        std::cout << "===============================" << "\n";
        std::cout << "Drive:             " + disk.name << "\n";
        std::cout << "Model Number:      " + disk.model << "\n";
        std::cout << "Size:              " + disk.size << "\n";
        
        if (smart_ok)
        {
            /* code */
            std::cout << "Serial Number:              " + disk.serialNumber << "\n";
            std::cout << "Firmware:              " + disk.firmwareVersion << "\n";
            std::cout << "Health:              " + disk.health << "\n";
            std::cout << "Temperature:              " + disk.temperature << "\n";
            std::cout << "Power On Hours:              " + disk.powerOnHours << "\n";
        }
        else {
            std::cout << "SMART data:        (Not available / need sude) \n"; 
        }

        std::cout << "===============================" << "\n";
    }
}