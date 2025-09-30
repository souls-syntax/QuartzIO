#pragma once
#include <string>
#include <variant>
#include "../../include/QuartzIO/datatypes.h"

using ModuleData = std::variant<StorageInfo, MemoryInfo, CpuInfo>;

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string getName() const = 0;
    virtual ModuleData fetchData() = 0;
};