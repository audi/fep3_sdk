## ℹ️ This repository is archived 

It is now maintained at https://github.com/cariad-tech


---


<!---
  Copyright @ 2020 Audi AG. All rights reserved.
  
      This Source Code Form is subject to the terms of the Mozilla
      Public License, v. 2.0. If a copy of the MPL was not distributed
      with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
  
  If it is not possible or desirable to put the notice in a particular file, then
  You may include the notice in a location (such as a LICENSE file in a
  relevant directory) where a recipient would be likely to look for such a notice.
  
  You may add additional accurate notices of copyright ownership.
  -->
# FEP 3 SDK

# Description

This package is a bundle package and contains (or pulls in) all of the FEP SDK:

### FEP 3 SDK Participant Library 

    fep_sdk_participant/3.0.0@aev25/beta

* Middleware for distributed simulation systems
* a_util platform abstraction library
** https://github.com/audi/a_util
* DDL library (data description language with codec API)
** https://github.com/audi/ddl
* RPC library with JSON-RPC code generator
** https://github.com/audi/pkg_rpc

### FEP 3 SDK System Library 

    fep_sdk_system/3.0.0@aev25/beta

* Connect and control a bundle of participants, put them together to a fep::System

### FEP SDK Controller Library 

    fep_sdk_controller/2.0.0@aev25/beta

* Connect and control a bundle of participants as a system with the help of the FEP SDK Metamodel File Descriptions

### FEP SDK Metamodel Library 

    fep_sdk_metamodel/1.0.1@aev25/stable

* Definition and a parser helper library for the  FEP SDK Metamodel File Descriptions

### FEP SDK Base Utilities

    fep_sdk_base_utilities/0.1.0@aev25/stable
    
* small command line tool to discover, control and configure FEP Systems
* provides an automation commandline interface for scripted automatic processing

### FEP 3 SDK Documentation

* see fep3_sdk/doc/fep3-sdk.html

## How to use ###

The FEP 3 SDK provides a CMake >= 3.5 configuration. Here's how to use it from your own CMake projects:

    find_package(fep3_sdk REQUIRED)

After this instruction, you can create CMake executable targets linking against FEP using the following command:

    #add a target for your participant code
    add_executable(my_participant source_file.cpp)

    #this will link the participant library cpp
    target_link_libraries(my_participant PRIVATE fep3_participant_cpp)
    #this will deploy and install the necessary libraries to the given folder
    fep3_participant_install(my_participant bin) 
    #this will deploy the necessary libraries while building to the build folder of the target (this is for debugging a good variant)
    fep3_participant_deploy(my_participant) 

Alternatively, you can append use the FEP System library 

    #add a target for your program code
    add_executable(my_system_control_tool source_file.cpp)
    #this will link the system library
    target_link_libraries(my_system_control_tool PRIVATE fep3_system)

    #this will deploy and install the necessary libraries to the given folder
    fep3_system_install(my_system_control_tool bin) 
    #this will deploy the necessary libraries while building to the build folder of the target (this is for debugging a good variant)
    fep3_system_deploy(my_system_control_tool)

For FEP 2 user

* there is NO variable to use anymore, link the target you need! 
* there is no fep_core target anymore, use fep3_participant instead!


### Build Environment ####

The libraries are built and tested only under following compilers and operating systems: 
* Windows 10 x64 with Visual Studio C++ 2015 Update 3.1 (Update 3 and KB3165756)
* Linux Ubuntu 16.04 LTS x64 with GCC 5.4 and libstdc++14 (C++14 ABI)

## How to build the examples ###

Simply point CMake to the examples directory (containing the CMakeLists.txt file) and generate a project.
Choose "Visual Studio 15 2017 Win64" with v140 toolset or "Unix Makefiles" generator, depending on your platform.

CMake might ask for the CMAKE_BUILD_TYPE variable to be defined. Possible values are Debug, Release or RelWithDebInfo

