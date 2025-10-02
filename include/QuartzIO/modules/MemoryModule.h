#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

#include <QuartzIO/IModule.h>

#include <string>
#include <vector>

class MemoryModule : public IModule {
    public:
        const char* getFlag() const override;
        void run(const Options& opts) override;
};
#endif