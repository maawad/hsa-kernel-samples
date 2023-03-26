## This file is based on
## https://github.com/HSAFoundation/HSA-Runtime-Conformance/blob/master/cmake/FindHSA.cmake



if (HSA_RUNTIME_INCLUDE_DIR)
    ## The HSA information is already in the cache.
    set (HSA_RUNTIME_FIND_QUIETLY TRUE)
endif (HSA_RUNTIME_INCLUDE_DIR)

## Look for the hsa include file path.

## If the HSA_INCLUDE_DIR variable is set,
## use it for the HSA_RUNTIME_INCLUDE_DIR variable.
## Otherwise set the value to /opt/rocm/hsa/include.
## Note that this can be set when running cmake
## by specifying -D HSA_INCLUDE_DIR=<directory>.

if(NOT DEFINED HSA_INCLUDE_DIR)
    set (HSA_INCLUDE_DIR "/opt/rocm/hsa/include")
endif()

MESSAGE("HSA_INCLUDE_DIR=${HSA_INCLUDE_DIR}")

find_path (HSA_RUNTIME_INCLUDE_DIR NAMES hsa.h PATHS ${HSA_INCLUDE_DIR})

## If the HSA_LIBRARY_DIR environment variable is set,
## use it for the HSA_RUNTIME_LIBRARY_DIR variable.
## Otherwise set the value to /opt/rocm/hsa/lib.
## Note that this can be set when running cmake
## by specifying -D HSA_LIBRARY_DIR=<directory>.

if(NOT DEFINED HSA_LIBRARY_DIR)
    set (HSA_LIBRARY_DIR "/opt/rocm/hsa/lib/")
endif()

MESSAGE("HSA_LIBRARY_DIR=${HSA_LIBRARY_DIR}")

# Check if the library was found
if(HSA_RUNTIME_LIBRARY)
    # Get the directory of the library
    get_filename_component(HSA_RUNTIME_LIBRARY_DIR ${HSA_RUNTIME_LIBRARY} DIRECTORY)

    # Add the library directory to the linker search path
    link_directories(${HSA_RUNTIME_LIBRARY_DIR})

    # Add the library to the target
    # target_link_libraries(${PROJECT_NAME} PRIVATE ${HSA_RUNTIME_LIBRARY})
else()
    message(FATAL_ERROR "HSA runtime library not found")
endif()

MESSAGE("HSA_RUNTIME_LIBRARY=${HSA_RUNTIME_LIBRARY}")
