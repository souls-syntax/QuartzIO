![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform: Linux](./asset/linux.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

# QuartzIO - A Modern C++ Diagnostic Tool for Linux

QuartzIO is a comprehensive, lightweight, and modular command-line utility for inspecting all of a Linux system's core vitals. Built from the ground up in modern C++17, it provides clean, easy-to-read information about your hardware and operating system, with optional script-friendly output formats like JSON.

From S.M.A.R.T. status to live CPU utilization and network speed tests, QuartzIO is a versatile tool for system administrators, developers, and curious power users.

## Features

   * Modular Design: Each piece of functionality is a self-contained module, making the codebase clean and extensible.

   * Storage (`--storage`): Lists all physical storage devices and retrieves detailed S.M.A.R.T. health data, including temperature, power-on hours, and overall health assessment.

   * Filesystem (`--fs`): Shows the total size, used space, and available space for all mounted physical filesystems.

   * Memory (`--mem`): Displays current RAM and Swap usage (total, used, available/free).

   * CPU (`--cpu`): Reports the CPU vendor, model, physical/logical core counts, and a live snapshot of the current CPU utilization percentage.

   * Kernel & OS (`--kernel`): Shows the distribution's pretty name, the full kernel version string, and the system's current uptime.

   * Network (`--net`): Enumerates all network interfaces with their state, MAC address, and all associated IPv4 and IPv6 addresses.

   * Speed Test (`--net --test`): Runs an active internet speed test to measure ping, download, and upload speeds using speedtest-cli.

   * Script-Friendly Output: All modules support `--json` and `--raw` flags for easy integration with scripts and other tools.

### Example Usage

#### Get a summary of your CPU, including live utilization:

```bash
$ quartzio --cpu
--- === CPU Information === ---
Vendor:       GenuineIntel
Model:        12th Gen Intel(R) Core(TM) i7-12700H
Cores:        14 (Physical) / 20 (Logical)
Utilization:  5.73 %
-------------------------------

```

#### View all network interfaces and their IP addresses:

```bash
$ quartzio --net
--- === Network Interfaces === ---
Interface: lo [UP] - 00:00:00:00:00:00
  inet    127.0.0.1/8
  inet6   ::1/128
------------------------------------
Interface: wlan0 [UP] - 1a:2b:3c:4d:5e:6f
  inet    192.168.1.123/24
  inet6   fe80::1234:5678:9abc:def0/64
------------------------------------
```

#### Run an active internet speed test:

```bash
$ quartzio --net --test
Running network speed test (this may take a moment).....
--- === Network Speed Test Results === ---
Ping:        12.345 ms
Download:    98.76 Mbit/s
Upload:      54.32 Mbit/s
------------------------------------------
```

#### Get memory information in JSON format:

```json
$ quartzio --mem --json
{
  "RAM": {
    "Total": "7.67 GB",
    "Used": "4.21 GB",
    "Available": "3.46 GB"
  },
  "Swap": {
    "Total": "8.00 GB",
    "Used": "0.00 B",
    "Free": "8.00 GB"
  }
}

```

## Requirements

* A Linux-based operating system.
* CMake >= 3.15.
* A C++17 compatible compiler (e.g., GCC, Clang).
* Core Utilities: `util-linux` (for lsblk), iproute2 (for ip).
* Optional Dependencies:
    * `smartmontools` (for S.M.A.R.T. data with --storage).
    * `speedtest-cli` (for the --net --test feature).
* The `nlohmann/json` library is included in the third_party directory.

## Installation

Clone the repository into a location of your choice (e.g., `~/.local/share/`):

```bash
git clone [https://github.com/souls-syntax/QuartzIO.git](https://github.com/souls-syntax/QuartzIO.git) ~/.local/share/QuartzIO
cd ~/.local/share/QuartzIO
```

### User-Local Install (Recommended)
This installs the quartzio binary into `~/.local/bin`, which does not require root privileges.

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build
cmake --install build

```
Make sure ~/.local/bin is in your shell's PATH. If it isn't, add the following line to your `~/.profile`, `~/.bashrc`, or `~/.zshrc`:

```bash
export PATH="$HOME/.local/bin:$PATH"
```

### System-Wide Install

To install for all users (usually to `/usr/local/bin`), run:

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

## Command reference

| Flag       | Sub-command / Format | Description                                                       |
|------------|----------------------|-------------------------------------------------------------------|
| `--storage`| `--json`, `--raw`    | Show S.M.A.R.T. health and info for all physical disks.           |
| `--fs`     | `--json`, `--raw`    | Show usage for all mounted physical filesystems.                  |
| `--mem`    | `--json`, `--raw`    | Show RAM and Swap usage.                                          |
| `--kernel` | `--json`, `--raw`    | Show OS, kernel version, and system uptime.                       |
| `--cpu`    | `--json`, `--raw`    | Show CPU model, core counts, and live utilization.                |
| `--net`    | `--json`, `--raw`    | Show all network interfaces and IP addresses.                     |
|            | `--test`             | When used with `--net`, performs an active speed test.            |
| `--version`|                      | Prints the current version of **QuartzIO**.                       |
| `--update` |                      | Attempts to `git pull` and reinstall the application.             |


## Future Plans

* GUI Wrapper: Develop a simple GUI front-end using a framework like Qt.

* TUI Mode: Create a terminal-based user interface (TUI) for a more interactive experience.

* GPU Module: Add a module for inspecting GPU information (a complex but valuable addition for V2.0).

## Project Structure

```text
QuartzIO/
├── CMakeLists.txt
├── README.md
├── third_party/
│   └── nlohmann/
│       └── json.hpp
├── include/
│   └── QuartzIO/
│       ├── IModule.h
│       ├── datatypes.h
│       ├── core/
│       │   └── command.h
│       └── modules/
│           ├── CpuModule.h
│           ├── FileSystemModule.h
│           ├── KernelModule.h
│           ├── MemoryModule.h
│           ├── NetworkModule.h
│           └── StorageModule.h
└── src/
    ├── main.cpp
    ├── core/
    │   └── command.cpp
    └── modules/
        ├── cpu/
        ├── filesystem/
        ├── kernel/
        ├── memory/
        ├── network/
        └── storage/
```



