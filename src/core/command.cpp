#include <QuartzIO/core/command.h>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>

// Executing the command and reading the output and returning it.
std::string exec(const char* cmd) {
    //opening pipe to execute and read the command output.
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    // if fails return nullptr.
    if (!pipe) {
        throw std::runtime_error("popen() failed");
    }

    //reading output one chunk at a time.
    std::array<char, 128> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
    {
        /* code */
        result += buffer.data(); 
    }
    return result;
}