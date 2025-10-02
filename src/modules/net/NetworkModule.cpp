#include <QuartzIO/datatypes.h>
#include <QuartzIO/modules/NetworkModule.h>
#include <QuartzIO/core/command.h> // seing after many days did you felt lonely

#include <nlohmann/json.hpp>

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

//
std::string parse_simple_output(const std::string &output, const std::string &key)
{
    std::stringstream ss(output);
    std::string line;
    while (std::getline(ss, line))
    {
        if (line.rfind(key, 0) == 0)
        {
            size_t colon_pos = line.find(':'); // colon_pos sure likes his comebacks
            if (colon_pos != std::string::npos)
            {
                std::string value = line.substr(colon_pos + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                return value;
            }
        }
    }
    return "N?A";
}

const char *NetworkModule::getFlag() const
{
    return "--net";
}

void NetworkModule::run(const Options &opts)
{

    // Doing the foolish and making some part dependent on 3rd party install but it looks cool so whatever.
    if (opts.run_speed_test)
    {
        std::cout << "Running network speed test (this may take a moment).....\n";
        try
        {
            std::string speedtest_output = exec("speedtest-cli --simple");

            std::string ping = parse_simple_output(speedtest_output, "Ping");
            std::string download = parse_simple_output(speedtest_output, "Download");
            std::string upload = parse_simple_output(speedtest_output, "Upload");

            std::cout << "--- === Network Speed Test Results === ---\n";
            std::cout << std::left << std::setw(12) << "Ping:" << ping << "\n";
            std::cout << std::left << std::setw(12) << "Download:" << download << "\n";
            std::cout << std::left << std::setw(12) << "Upload:" << upload << "\n";
            std::cout << "------------------------------------------\n";

        } catch (const std::exception& e) {
            std::cerr << "Error: Failed to run speed test. Is 'speedtest-cli' installed?\n";
            std::cerr << "  (You can install it with 'sudo pacman -S speedtest-cli')\n";
        }
        return; 
    }

    std::vector<NetworkInterface> interfaces;
    try {
        std::string json_output = exec("ip -j addr");
        auto parsed_json = json::parse(json_output);

        for (const auto& iface_json : parsed_json) {
            NetworkInterface current_iface;
            current_iface.name = iface_json.value("ifname", "N/A");
            current_iface.mac_address = iface_json.value("address", "N?A");
            current_iface.state = iface_json.value("operstate", "N?A");

            if (iface_json.contains("addr_info")) {
                for (const auto& addr_json : iface_json["addr_info"]) {
                    AddressInfo current_addr;
                    current_addr.family = addr_json.value("family", "N/A");
                    current_addr.address = addr_json.value("local", "N/A");
                    current_addr.prefix_length = addr_json.value("prefixlen", 0);
                    current_iface.addresses.push_back(current_addr);
                }
            }
            interfaces.push_back(current_iface);
        }
    }
    catch (...) {
        std::cerr << "Error: Could not execute or parse 'ip -j addr'. Is 'ip' installed\n";
        return;
    }

    std::cout << "--- === Network Interface === ---\n";
    for (const auto& iface : interfaces) {
        std::cout << "Interface: " << iface.name
                  << "[" << iface.state << "]"
                  << " - " << iface.mac_address << "\n";
        if (iface.addresses.empty()) {
            std::cout << "  (No Ip addresses)\n";
        } else {
            for (const auto& addr : iface.addresses) {
                std::cout << "  " << std::left << std::setw(8) << addr.family
                          << addr.address << "/" << addr.prefix_length << "\n";
            }
        }
        std::cout << "----------------------------------------------\n";
    }
    
}