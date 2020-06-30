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

# find the packages
find_package(fep3_participant_core REQUIRED)
find_package(fep3_system REQUIRED)
find_package(a_util_system REQUIRED)

##################################################################
# DEMO Logging via RPC
##################################################################

### Participant ###
add_executable(demo_logging
               logging_participant.cpp)

set_target_properties(demo_logging PROPERTIES FOLDER "examples/logging_example")
# link against the fep3_participant library
target_link_libraries(demo_logging PRIVATE
                      fep3_participant_core)
fep3_participant_install(demo_logging examples/bin)
fep3_participant_deploy(demo_logging)


### System ###
add_executable(demo_logging_system
               logging_system.cpp)
               
set_target_properties(demo_logging_system PROPERTIES FOLDER "examples/logging_example")
#link against the fep3_system library
target_link_libraries(demo_logging_system PRIVATE
                      fep3_system
                      a_util_system)
fep3_system_install(demo_logging_system examples/bin)
fep3_system_deploy(demo_logging_system)                      