cmake_minimum_required(VERSION 3.15)
project(QuartzIO VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/core/command.cpp
    src/modules/storage/StorageModule.cpp
    src/modules/filesystem/FileSystemModule.cpp
    src/modules/memory/MemoryModule.cpp
    src/modules/kernel/KernelModule.cpp
    src/modules/cpu/CpuModule.cpp
    src/modules/net/NetworkModule.cpp
)

add_executable(quartzio ${SOURCES})

target_include_directories(quartzio PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party"
)

install(TARGETS quartzio DESTINATION bin)