// QuartzIO let's go
#include <QuartzIO/core/command.h>
#include <QuartzIO/modules/StorageModule.h>
#include <QuartzIO/modules/FileSystemModule.h>
#include <QuartzIO/IModule.h>
#include <QuartzIO/modules/MemoryModule.h>
#include <QuartzIO/datatypes.h>
#include <QuartzIO/modules/KernelModule.h>
#include <QuartzIO/modules/CpuModule.h>
#include <QuartzIO/modules/NetworkModule.h>

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#define QUARTZIO_VERSION "0.1.2"

void print_usage() {
    std::cout << "QuartzIO Diagnostic Tool" << "\n";
    std::cout << "Usage: quartzio <flag>" << "\n";
    std::cout << "Available flags:" << "\n";
    std::cout << " --storage        Show drive and S.M.A.R.T info" << "\n";
    std::cout << " --fs             Show the total space available and free space in each mount" << "\n";
    std::cout << " --mem            Show the information about memory";
    std::cout << " --version        Show Version \n";
    std::cout << " --update         Update QuartzIO (git pull + rebuild)\n";
}


// Main function to just connect pipelines
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    std::string user_flag = argv[1];

    if (user_flag == "--version")
    {
        std::cout << "QuartzIO version " << QUARTZIO_VERSION << "\n";
        return 0;
    }
    else if (user_flag == "--update")
    {
        std::cout << "Updating QuartzIO...\n";
        int ret = std::system(
                "cd ~/.local/share/QuartzIO && git pull && cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local && cmake --build build && cmake --install build"
        );

        if (ret == 0)
        {
            std::cout << "QuartzIO updated successfully.\n"
        ;}
        else {
            std::cerr << "Update failed. Check your setup.\n";
        }
        return ret;
    }

    // =====New Architecture ===== // Aren't I amazing making all things so modular you can't imagine the effort it takes in start.

    Options opts;

    if(argc>2) {
        std::string format_flag = argv[2];
        if (format_flag == "--raw")
        {
            opts.format = OutputFormat::Raw
        ;} else if (format_flag == "--json") {
            opts.format = OutputFormat::Json;
        } else if (format_flag == "--test")
        {
            opts.run_speed_test = true;
        }
    }

    
    // Main functionality engine starts.
    std::vector<std::unique_ptr<IModule>> modules;
    modules.push_back(std::make_unique<StorageModule>());
    modules.push_back(std::make_unique<FileSystemModule>());
    modules.push_back(std::make_unique<MemoryModule>());
    modules.push_back(std::make_unique<KernelModule>());
    modules.push_back(std::make_unique<CpuModule>());
    modules.push_back(std::make_unique<NetworkModule>());

    bool module_found = false;

    for (const auto& module : modules)
    {
        /* code */
        if (user_flag == module->getFlag())
        {
            /* code */
            module->run(opts);
            module_found = true;
            break;
        }   
    }
    if (!module_found)
    {
        /* code */
        std::cerr << "Error: Unknown flag `" << user_flag << "`" << "\n";
    }
    return 0;
}