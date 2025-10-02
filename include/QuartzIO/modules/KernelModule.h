#pragma once

#include <QuartzIO/IModule.h>

class KernelModule : public IModule {
    public:
        const char* getFlag() const override;
        void run(const Options& opts) override;
};