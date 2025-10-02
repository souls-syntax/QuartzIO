#pragma once
#include <string>
#include <variant>
#include <QuartzIO/datatypes.h>

class IModule {
public:
    virtual ~IModule() = default;
    virtual const char* getFlag() const = 0;
    virtual void run(const Options& opts) = 0;
};