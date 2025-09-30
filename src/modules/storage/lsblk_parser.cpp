#include "lsblk_parser.h"
#include <sstream>
#include <vector>


// Parsing the output of lsblk command.
// The lsblk command we will be executing is "lsblk -d -n -o NAME,MODEL,SIZE"
//result will be like
// zram0                                     3.7G
//nvme0n1 SK hynix BC711 HFHS12GD******13M   476.9G

std::vector<DiskInfo> parse_lsblk(const std::string& lsblk_output) {
    std::vector<DiskInfo> disks;
    std::stringstream ss(lsblk_output);
    std::string line;

    //read output lini by line.
    while (std::getline(ss, line))
    {
        /* code */

        //parsing line word by word
        std::stringstream line_ss(line);
        DiskInfo current_disk;

        //first word is device name
        line_ss >> current_disk.name;

        // the last word is the size
        //the middel part is the model which will result in "" if there is no such thing like vram.
        std::string temp_model;
        std::string current_word;
        std::vector<std::string> words;

        //read all the reamining words in a vector
        while (line_ss >> current_word)
        {
            /* code */
            words.push_back(current_word);
        }
        //skip if no word
        if (words.empty())
        {
            /* code */
            continue;
        }
        // the last element is the size
        current_disk.size = words.back();
        words.pop_back();


        // whatever is left model name likely , join it with spaces.
        std::stringstream model_ss;
        for (size_t i = 0; i < words.size(); ++i)
        {
            /* code */
            model_ss << words[i] << (i==words.size() -1 ? "" : " ");
        }
        current_disk.model = model_ss.str();

        disks.push_back(current_disk);
    }
    return disks;
}