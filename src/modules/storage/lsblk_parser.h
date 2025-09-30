#ifndef LSBLK_PARSER_H
#define LSBLK_PARSER_H

#include <string>
#include <vector>

struct DiskInfo {
    std::string name;
    std::string model;
    std::string size;
};

std::vector<DiskInfo> parse_lsblk(const std::string& lsblk_output);

#endif