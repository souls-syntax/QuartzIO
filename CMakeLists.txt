cmake_minimum_required(VERSION 3.15)
project(QuartzIO VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List all your source files here
set(SOURCES
    src/core/command.cpp
    src/modules/storage/StorageModule.cpp
    src/main.cpp
)

add_executable(QuartzIO ${SOURCES})

# Tell the compiler to look for headers in the "include" directory
target_include_directories(QuartzIO PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)