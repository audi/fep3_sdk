#
# Copyright @ 2020 Audi AG. All rights reserved.
# 
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
# 
# You may add additional accurate notices of copyright ownership.
#
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(SET CMP0011 NEW)

# Disable extensions here and require the chosen CMAKE_CXX_STANDARD (coming from Conan)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use shared libraries from current directory on Linux (same behavior as on Windows)
SET(CMAKE_INSTALL_RPATH "$ORIGIN")

project(fep3-sdk VERSION 3.0.0)

set(FEP_SDK_PKG_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(FEP_SDK_PKG_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(FEP_SDK_PKG_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(FEP_SDK_PKG_VERSION "${FEP_SDK_PKG_VERSION_MAJOR}.${FEP_SDK_PKG_VERSION_MINOR}.${FEP_SDK_PKG_VERSION_PATCH}")

# Enable project folder structure for Visual Studio IDE
set_property(GLOBAL PROPERTY USE_FOLDERS true)

# check for conan
if(CONAN_COMPILER)
    message(STATUS "Including Conan build info")
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

    set(DOXYGEN_ROOT ${CONAN_DOXYGEN_ROOT})
endif()

include(scripts/cmake/enable_multicore_compilation.cmake)
include(scripts/cmake/use_integrated_debug_symbols.cmake)

# Create version config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/fep3_sdk-config-version.cmake
                                 COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fep3_sdk-config-version.cmake DESTINATION .)
install(FILES fep3_sdk-config.cmake DESTINATION .)

### Set basic and obligatory CMake parameters and find Project Templates package.
set(AEV_PRODUCT_FULL_NAME "FEP3 SDK - Functional Engineering Platform SDK")
set(AEV_PRODUCT_SHORT_NAME "fep3-sdk")

### Product specific
set(FEP3_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BETA_BUILD false CACHE BOOL "Mark as beta")

# some settings need to be set explicitly for QNX
if (UNIX)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
    set(CMAKE_SKIP_BUILD_RPATH  OFF)
endif(UNIX)

option(fep3_sdk_cmake_enable_documentation
       "If enabled, generate the source code documentation -\
 requires doxygen and sphinx-build (default: ON)" ON)

################################################################################
### Setting up packages
################################################################################
find_package(fep_base_utilities REQUIRED)
find_package(fep3_controller REQUIRED)
find_package(fep_metamodel REQUIRED)
find_package(fep3_system REQUIRED)
find_package(fep3_participant REQUIRED)

set(examples_integrated ON)
add_subdirectory(examples)
install(DIRECTORY examples
        DESTINATION .)

# add subdirectories core
if (fep3_sdk_cmake_enable_documentation)
    add_subdirectory(doc)
endif()

# Licenses
add_subdirectory(doc/license)
