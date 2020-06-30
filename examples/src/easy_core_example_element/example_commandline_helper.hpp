/**
 *
 * @file

   @copyright
   @verbatim
   Copyright @ 2019 Audi AG. All rights reserved.
   
       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
   
   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.
   
   You may add additional accurate notices of copyright ownership.
   @endverbatim
 *
 *
 * @remarks
 *
 * Do not edit manually the MDE generated part of this file!
 *
 * Added to the generated file
 * - header protection
 * - namespace
 */

#pragma once

#include <string>

namespace fep3_examples
{

std::string getParticipantName(int argn, const char* argv[], const std::string& default_value)
{
    if (argn < 2)
    {
        if (default_value.empty())
        {
            throw std::invalid_argument("invalid commandline call: use <exe> <participant_name> <system_name>");
        }
        else
        {
            return default_value;
        }
    }
    else
    {
        return argv[1];
    }
}

std::string getSystemName(int argn, const char* argv[], const std::string& default_value)
{
    if (argn < 3)
    {
        if (default_value.empty())
        {
            throw std::invalid_argument("invalid commandline call: use <exe> <participant_name> <system_name>");
        }
        else
        {
            return default_value;
        }
    }
    else
    {
        return argv[2];
    }
}

} // namespace fep3_examples
