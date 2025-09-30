#ifndef STORAGE_MODULE_H
#define STORAGE_MODULE_H
#include <QuartzIO/IModule.h>

#include <string>
#include <vector>
struct DiskInfo;
//std::vector<DiskInfo> parse_lsblk(const std::string& lsblk_output);
class StorageModule : public IModule {
public:
    const char* getFlag() const override;
    void run() override;
// private:
//     std::vector<DiskInfo> parse_lsblk(const std::string& lsblk_output);
//     void printDiskInfo();
};

#endif 
