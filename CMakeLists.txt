cmake_minimum_required(VERSION 3.6.3)
project(CPP_BACKTESTER)

add_executable(main main.cpp)

# target include directory
target_include_directories(CPP_BACKTESTER PRIVATE include include/tqdm)

# Link libcurl to main
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)