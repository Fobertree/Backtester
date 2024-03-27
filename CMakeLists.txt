cmake_minimum_required(VERSION 3.6.3)
project(CPP_BACKTESTER)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)

# target include directory
target_include_directories(main PRIVATE include include/tqdm)

# src
file(GLOB SOURCES src/*.cpp)
add_executable(main ${SOURCES})

# Link libcurl to main
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)