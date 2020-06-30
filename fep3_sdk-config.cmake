#######################################################################
#
# FEP SDK Config File
# # Copyright @ 2019 Audi AG. All rights reserved.
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
#
#######################################################################

# execute only once
if (FEP_SDK_FOUND)
    return()
endif (FEP_SDK_FOUND)

if (WIN32)
    cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
endif()

if(UNIX AND NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "The CMAKE_BUILD_TYPE has to be specified first!")
endif(UNIX AND NOT CMAKE_BUILD_TYPE)

find_package(fep3_participant REQUIRED)
find_package(fep3_participant_core REQUIRED)
find_package(fep3_participant_cpp REQUIRED)
find_package(fep3_system REQUIRED)
find_package(fep3_controller REQUIRED)
find_package(fep_metamodel REQUIRED)
find_package(fep_base_utilities REQUIRED)

set(FEP_SDK_FOUND true)