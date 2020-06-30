##################################################################
# @file 
# @copyright &copy; 2020 Audi AG
#            All right reserved.
# 
# This Source Code Form is subject to the terms of the 
# Mozilla Public License, v. 2.0. 
# If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
##################################################################
if (NOT examples_integrated)
    project(fep3-sdk-examples)

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD 14)

    # if Conan is used, we have to include the generated file and doing some basic setup
    if(CONAN_COMPILER)
        if ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
        else()
            message(FATAL_ERROR "Conan build info can't be found.")
        endif()
    
        if(CORTEX_WORKSPACE)
            conan_basic_setup(TARGETS)
        else()
            conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
        endif()
    endif()

    # Enable project folder structure for Visual Studio IDE
    set_property(GLOBAL PROPERTY USE_FOLDERS true)

endif()

add_subdirectory(src/rpc_service_bus)
add_subdirectory(src/easy_cpp_example_datajob)
add_subdirectory(src/easy_core_example_element)
add_subdirectory(src/logging_example)
