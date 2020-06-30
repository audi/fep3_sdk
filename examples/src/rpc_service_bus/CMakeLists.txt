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

# find the package
find_package(fep3_participant_core REQUIRED PATHS ${fep3_participant_DIR})
find_package(pkg_rpc REQUIRED)

##################################################################
# DEMO RPC SERVICE
##################################################################

# parameter1: the json file to generate the service stub for
# parameter2: the class name + namespaces for the service stub 
# parameter3: the header file where to generate the service stub to 
jsonrpc_generate_server_stub(${CMAKE_CURRENT_SOURCE_DIR}/demo_rpc_service.json
                             demo_rpc_stubs::DemoServiceStub
                             ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_service_stub.h)

add_executable(demo_rpc_service
               demo_rpc_service.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_service_stub.h)
target_include_directories(demo_rpc_service PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(demo_rpc_service PROPERTIES FOLDER "examples/rpc_service_bus")
# link against the fep3_participant, pkg_rpc
target_link_libraries(demo_rpc_service PRIVATE
                      fep3_participant_core
                      pkg_rpc)

fep3_participant_install(demo_rpc_service examples/bin)
fep3_participant_deploy(demo_rpc_service)

##################################################################
# DEMO RPC CLIENT
##################################################################

# parameter1: the json file to generate the client stub for
# parameter2: the class name + namespaces for the client stub 
# parameter3: the header file where to generate the ckient stub to 
jsonrpc_generate_client_stub(${CMAKE_CURRENT_SOURCE_DIR}/demo_rpc_service.json
                               demo_rpc_stubs::DemoClientStub
                               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_client_stub.h)

add_executable(demo_rpc_client
               demo_rpc_client.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_client_stub.h)
target_include_directories(demo_rpc_client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(demo_rpc_client PROPERTIES FOLDER "examples/rpc_service_bus")
target_link_libraries(demo_rpc_client PRIVATE
                      fep3_participant_core
                      pkg_rpc)

fep3_participant_install(demo_rpc_client examples/bin)
fep3_participant_deploy(demo_rpc_client)




