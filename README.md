![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform: Linux](./asset/linux.svg)
# QuartzIO

QuartzIO is a lightweight C++ utility for enumerating and inspecting storage devices on Linux.  

Currently, it uses smartctl to fetch disk names, models, sizes, etc. then parses the output into structured data for display.

It also uses c++ builtin filesystem module for giving information about current state of each physical mount by inspecting `/proc/mounts`

Future plans include: 
* Addition of more tools for making and general purpose inspection tool
* Developing a user-friendly installer script (install.sh) to automate the cloning and building process.
* Providing a Qt-based GUI view for easier visualization.
* Optional JSON/TUI output modes for integration with scripts or dashboards.

## Features (Current)

* Device Discovery: Lists all physical storage devices with their model and size
* S.M.A.R.T. Health Status: Retrieves and displays detailed health information for each drive, including temperature, power-on hours, and overall health assessment.
* Filesystem Usage: Shows the total size, used space, and available space for all mounted filesystems.
* Parses command output into structured DiskInfo objects.
* Displays detected devices with model and size information.

Example output

```cpp
$ quartzio --storage
===============================
Drive:           nvme0n1
Model Number:    SK hynix BC711
Size:            476.9G
Serial Number:   XXXXXXXXXXXX
Firmware:        XXXXXXXX
Health:          PASSED
Temperature:     35 Celsius
Power On Hours:  1234
===============================

$ quartzio --fs
Filesystem               Size           Used           Available      Use%
--------------------------------------------------------------------------------
/                        29.80 GB       15.21 GB       14.59 GB       51%
/boot                    975.91 MB      250.00 MB      725.91 MB      25%
/home                    400.00 GB      150.00 GB      250.00 GB      37%
```

## Install & Run

### Requirements
* Linux system.
* CMake >= 3.15.
* C++ 17 compiler (eg. g++, clang++).
* lsblk command & smartctl (from util-linux package).
  
### Install

Clone into a location you control ( we'll use `~/.local/share/QuartzIO` for the user install example):
```bash
git clone https://github.com/yourusername/QuartzIO.git ~/.local/share/QuartzIO
cd ~/.local/share/QuartzIO
```
#### Build & user-install (no sudo)

This installs the quartzio binary into ~/.local/bin:

```cmake
cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build
```

Make sure ~/.local/bin is in your PATH (add to ~/.profile or ~/.bashrc or ~/.zshrc if needed):

```bash
export PATH="$HOME/.local/bin:$PATH"
```

### Run

You should now be able to run

```bash
quartzio --storage

quartzio --fs
```

### System-wide install (for all users)

```cmake
cmake -B build
cmake --build build
sudo cmake --install build
```
This installs the binary to /usr/local/bin/ by default.

## Update

Simple self-update (requires repo to be at ~/.local/share/QuartzIO and git, cmake installed):

```bash
quartzio --update
```

### Manual update (if you prefer manual steps):
```bash
cd ~/.local/share/QuartzIO
git pull
cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build
```

## Uninstall

User uninstall (remove binary only):

`rm -f ~/.local/bin/quartzio`

If installed system-wide:

`sudo rm -f /usr/local/bin/quartzio`

## Troubleshooting

#### `quartzio --update` fails:
  * Ensure the repo was cloned into ~/.local/share/QuartzIO.

  * Ensure git, cmake, and a compiler (g++/clang++) are installed.

  * Run the manual update steps to see the failing command and error output.

#### `quartzio: command not found`:

  * Confirm ~/.local/bin is in your $PATH.

  * Make sure you installed with -DCMAKE_INSTALL_PREFIX=$HOME/.local.

## Project Structure

```css
QuartzIO/
├── CMakeLists.txt
├── include/
│   └── QuartzIO/
│       ├── IModule.h
│       ├── datatypes.h
│       ├── core/
│       │   └── command.h
│       └── modules/
│           ├── FileSystemModule.h
│           └── StorageModule.h
└── src/
    ├── main.cpp
    ├── core/
    │   └── command.cpp
    └── modules/
        ├── filesystem/
        │   └── FileSystemModule.cpp
        └── storage/
            └── StorageModule.cpp 
```

![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)



