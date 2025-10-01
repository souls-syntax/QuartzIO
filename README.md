![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
# QuartzIO

QuartzIO is a lightweight C++ utility for enumerating and inspecting storage devices on Linux.  

Currently, it uses smartctl to fetch disk names, models, sizes, etc. then parses the output into structured data for display.

It also uses c++ builtin filesystem module for giving information about current state of each physical mount by inspecting `/proc/mounts`

Future plans include: 
* Addition of more tools for making and general purpose inspection tool
* Wrapping the whole thing in either bash for easier access and system-wide installation.
* Providing a Qt-based GUI view for easier visualization.
* Optional JSON/TUI output modes for integration with scripts or dashboards.

## Features (Current)

* Executes `lsblk -d -n -o NAME,MODEL,SIZE` via a safe wrapper.
* Executes `smartctl -a /dev/disk.name` via a safe wrapper.
* Uses `ifstream mounts_file()` to get the data regarding file system and produce human readable output.
* Parses command output into structured DiskInfo objects.
* Displays detected devices with model and size information.

Example output

```cpp
--- Parsing lsblk output ---
Device: /dev/nvme0n1
 Model: SK hynix BC711 HFHS12GD******
  Size: 476.9G
======================================
Device: /dev/zram0
 Model: N/A
  Size: 3.7G
======================================
```

## Build & Run

### Requirements
* Linux system.
* CMake >= 3.15.
* C++ 17 compiler (eg. g++, clang++).
* lsblk command & smartctl (from util-linux package).
  
### Build
```bash
git clone https://github.com/yourusername/QuartzIO.git
cd QuartzIO
cmake -B build && cd build
cmake --build .
```

### Run
```bash
./QuartzIO [ --storage, --fs]
```

## Project Structure

```css
QuartzIO/
├── CMakeLists.txt
│
├── include/
│   └── QuartzIO/
│       ├── datatypes.h           # Public: Shared data structure
│       ├── IModule.h             # Public: Core interface for all modules
│       │
│       ├── core/
│       │   └── command.h         # Public: Core utility interface
│       │
│       └── modules/
│           └── StorageModule.h   # Public: Interface for Storage specific module
│           └── FileSystemModule.h # Public: Interface for fs module.
└── src/
    ├── main.cpp                # Implements the application logic
    │
    ├── core/
    │   └── command.cpp         # Implements the command utility
    │
    └── modules/
        └── storage/
        |    └── StorageModule.cpp # Implements the storage module
        └── filesystem/
            └── FileSystemModule.cpp # Implement FileSystem information.  
```

![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)



