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

# find the package
find_package(fep3_participant_core REQUIRED)

##################################################################
# DEMO EASY Sender with a CORE ElementBase
##################################################################
add_executable(demo_easy_core_sender
               core_sender_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_core_sender PROPERTIES FOLDER "examples/easy_core_example")
# link against the fep3_participant
target_link_libraries(demo_easy_core_sender PRIVATE
                      fep3_participant_core)
fep3_participant_install(demo_easy_core_sender examples/bin)
fep3_participant_deploy(demo_easy_core_sender)

##################################################################
# DEMO EASY Receiver with a CORE ElementBase
##################################################################
add_executable(demo_easy_core_receiver
               core_receiver_main.cpp
               example_ddl_types.h)

set_target_properties(demo_easy_core_receiver PROPERTIES FOLDER "examples/easy_core_example")
# link against the fep3_participant_core API
target_link_libraries(demo_easy_core_receiver PRIVATE
                      fep3_participant_core)
fep3_participant_install(demo_easy_core_receiver examples/bin)
fep3_participant_deploy(demo_easy_core_receiver)

