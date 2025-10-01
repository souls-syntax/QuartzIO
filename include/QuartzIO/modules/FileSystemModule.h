#ifndef FILESYSTEM_MODULE_H
#define FILESYSTEM_MODULE_H

#include <QuartzIO/IModule.h>

class FileSystemModule : public IModule {
    public:
        const char* getFlag() const override;

        void run() override;
};

#endif