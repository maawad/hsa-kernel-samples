cmake_minimum_required(VERSION 3.10)
project(hsa_kernel_samples)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Set path to CMake modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Find HSA runtime
find_package(HSA REQUIRED)

# Add HSA include directory
include_directories(${HSA_INCLUDE_DIR})

# Direct all output to /bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Create interface library for HSA dependencies
add_library(hsa_samples INTERFACE)
target_include_directories(hsa_samples INTERFACE ${HSA_INCLUDE_DIR})
target_link_libraries(hsa_samples INTERFACE ${HSA_RUNTIME_LIBRARY})




add_subdirectory(vector_add)
