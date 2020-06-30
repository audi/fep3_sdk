/**
   @copyright
   @verbatim
   Copyright @ 2020 Audi AG. All rights reserved.
   
       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
   
   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.
   
   You may add additional accurate notices of copyright ownership.
   @endverbatim
 */
#include <iostream>
#include <memory>
#include <string>

#include <fep3/cpp.h>

#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

#include "example_commandline_helper.hpp"
#include "example_ddl_types.h"


class EasyCPPReceiverDataJob : public cpp::DataJob
{
public:
    //Implementation of the CTOR!
    //Data has no default CTOR 
    // you must define job name 
    EasyCPPReceiverDataJob()
        : cpp::DataJob("receive_job", 1s )
    {
        //create DataAccess with the reader class
        //this will create a data reader for strings
        _data_reader_string = addDataIn("string_data",
            StreamTypeString());

        //this will create a data reader for ddl based structures
        _data_reader_ddl = addDataIn("ddl_data",
            StreamTypeDDL(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description));

        //this will create a data reader for dynamic arrays of DDL based structures
        _data_reader_ddl_array = addDataIn("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data reader for dynamic arrays of DDL based structures
        _data_reader_ddl_array = addDataIn("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data reader for fixed size uint32_t values
        _data_reader_plain_c_type = addDataIn("plain_c_type_int32_t",
            StreamTypePlain<int32_t>());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        //print the last value in queue for the plain value 
        //the content of the _data_reader_plain_c_type reader queue changes only in processDataIn!
        // processDataIn will receive the content of the data in queues in DataRegistry until they are empty
        int32_t received_plain_value = 0;
        *_data_reader_plain_c_type >> received_plain_value;
        FEP3_LOG_INFO("received plain value:" + std::to_string(received_plain_value));

        //receive the string value
        std::string received_string_value;
        *_data_reader_string >> received_string_value;
        FEP3_LOG_INFO("received string value:" + received_string_value);

        //received ddl structure value
        fep3_examples::tEasyStruct easy_struct = {};
        *_data_reader_ddl >> easy_struct;
        FEP3_LOG_INFO("received easy struct value: " + easy_to_string(easy_struct));

        //received ddl array structure value
        std::vector<fep3_examples::tEasyStruct> easy_struct_array;
        //if we do not have a fixed size of the array, but a dynamic size
        //we need to get the content with the help of fep3::StdVectorSampleType
        fep3::StdVectorSampleType<fep3_examples::tEasyStruct> easy_struct_array_to_receive(easy_struct_array);
        auto read_sample = _data_reader_ddl_array->read();
        if (read_sample)
        {
            read_sample->read(easy_struct_array_to_receive);
        }
        if (easy_struct_array.size() > 0)
        {
            FEP3_LOG_INFO(std::string() + "received easy struct array in size of "
                + std::to_string(easy_struct_array.size())
                + "first value is :"
                + easy_to_string(easy_struct_array[0]));
        }
        else
        {
            FEP3_LOG_INFO("not yet received ddl array");
        }

        return {};
    }

    std::string easy_to_string(const fep3_examples::tEasyStruct& easy_struct) const
    {
        std::string res = "{ pos = { " + std::to_string(easy_struct.pos.x_pos) + ", ";
        res += std::to_string(easy_struct.pos.y_pos) + ", ";
        res += std::to_string(easy_struct.pos.z_pos) + " }";
        res += ", double_value = " + std::to_string(easy_struct.double_value) + " }";
        return res;
    }

    //Definitions of the readers you want to use
    core::DataReader* _data_reader_string;
    core::DataReader* _data_reader_ddl;
    core::DataReader* _data_reader_ddl_array;
    core::DataReader* _data_reader_plain_c_type;
};

int main(int argn, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<cpp::DataJobElement<EasyCPPReceiverDataJob>>(
            fep3_examples::getParticipantName(argn, argv, "demo_cpp_receiver"),
            fep3_examples::getSystemName(argn, argv, "demo_system"));
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}
