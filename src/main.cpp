// QuartzIO let's go
#include "command.h"
#include "modules/storage/StorageModule.h"
#include "modules/IModule.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

void print_usage() {
    std::cout << "QuartzIO Diagnostic Tool" << "\n";
    std::cout << "Usage: quartzio <flag>" << "\n";
    std::cout << "Available flags:" << "\n";
    std::cout << " --storage Show drive and S.M.A.R.T info" << "\n";
}


// Main function to just connect pipelines
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::vector<std::unique_ptr<IModule>> modules;
    modules.push_back(std::make_unique<StorageModule>());

    std::string user_flag = argv[1];
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
    // if (argc < 2)
    // {
    //     /* code */
    //     std::cerr << "Usage: quartzio [--storage || -s] \n";
    //     return 1;
    // }
    // std::string command = argv[1];

    // if (command == "--storage" || command == "--storage")
    // {
    //     /* code */
    //     printDiskInfo();
    // }
    // else {
    //     std::cerr << "Error: Unknown command `" << command << "`" << "\n";
    //     return 1;
    // }

    // return 0;
}