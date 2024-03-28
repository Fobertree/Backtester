cmake_minimum_required(VERSION 3.6.3)
project(CPP_BACKTESTER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add O3 flag, which adds optimizations (may also increase file size)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# include directory
include_directories(include include/tqdm)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/tqdm")

file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES "cpp/include/*hpp" "cpp/include/tqdm/*hpp")
file(GLOB PYTHON_FILES "python/*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES)

# Headers
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include")
set(PYTHON_LIBRARY_DIR "python")

# Link libcurl to main
# find_package(CURL REQUIRED)
# target_link_libraries(main PRIVATE CURL::libcurl)

find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(CPP_BACKTESTER
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${PYTHON_FILES}
)

target_link_libraries(CPP_BACKTESTER PUBLIC)

install(TARGETS CPP_BACKTESTER
    COMPONENT python
    LIBRARY DESTINATION "${PYTHON_LIBRARY_DIR}"
)