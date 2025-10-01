// QuartzIO let's go
#include <QuartzIO/core/command.h>
#include <QuartzIO/modules/StorageModule.h>
#include <QuartzIO/modules/FileSystemModule.h>
#include <QuartzIO/IModule.h>

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
    

    

    std::vector<std::unique_ptr<IModule>> modules;
    modules.push_back(std::make_unique<StorageModule>());
    modules.push_back(std::make_unique<FileSystemModule>());

    bool module_found = false;

    for (const auto& module : modules)
    {
        /* code */
        if (user_flag == module->getFlag())
        {
            /* code */
            module->run();
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