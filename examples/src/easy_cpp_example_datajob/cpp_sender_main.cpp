/**

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

class EasyCPPSenderDataJob : public cpp::DataJob
{
public:
    //Impleemntation of the CTOR!
    //DataJob has no default CTOR 
    // you must define a name of the job -> to create the job name in registry
    EasyCPPSenderDataJob()
        : cpp::DataJob("sender_job", 1s)
    {
        //register the properties
        registerPropertyVariable(_prop_to_send_as_integer, "integer_value");
        registerPropertyVariable(_prop_to_send_as_string, "string_value");
        registerPropertyVariable(_prop_to_send_as_double, "double_value");
        registerPropertyVariable(_prop_as_string_array, "string_array_value");

        //create DataAccess with the writer class
        //this will create a data writer for strings
        _data_writer_string = addDataOut("string_data",
            StreamTypeString());

        //this will create a data writer for ddl based structures
        _data_writer_ddl = addDataOut("ddl_data",
            StreamTypeDDL(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = addDataOut("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = addDataOut("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for fixed size uint32_t values
        _data_writer_plain_c_type = addDataOut("plain_c_type_int32_t",
            StreamTypePlain<int32_t>());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution) override
    {
        //we want to read the current properties to oure propertyvariables
        //so we can use them for sendig the data
        updatePropertyVariables();

        //write the data to the string signal queue
        std::string string_value_to_write = _prop_to_send_as_string;
        *_data_writer_string << string_value_to_write;

        //write the data to the DDL signal
        fep3_examples::tEasyStruct my_easy_struct;
        my_easy_struct.double_value = _prop_to_send_as_double;
        my_easy_struct.pos = { static_cast<uint32_t>(_prop_to_send_as_integer) ,
                               static_cast<uint32_t>(_prop_to_send_as_integer) ,
                               static_cast<uint32_t>(_prop_to_send_as_integer) };
        *_data_writer_ddl << my_easy_struct;

        //prepare and write the data to the DDL Arraysignal
        std::vector<fep3_examples::tEasyStruct> easy_array;
        int fill_idx = 32;
        while (fill_idx > 0)
        {
            easy_array.emplace_back(my_easy_struct);
            --fill_idx;
        }
        //currently we need that helper class to write it 
        fep3::StdVectorSampleType<fep3_examples::tEasyStruct> easy_array_wrapper(easy_array);
        easy_array_wrapper.setTime(sim_time_of_execution);

        _data_writer_ddl_array->write(easy_array_wrapper);

        //write the plain value
        int32_t value_to_write = _prop_to_send_as_integer;
        *_data_writer_plain_c_type << value_to_write;

        FEP3_LOG_INFO("Data sent");

        return {};
    }


    //use the PropertyVariable as easy readable configuration element
    //but do NOT forget to register the variables in the CTOR with a name
    cpp::PropertyVariable<int32_t> _prop_to_send_as_integer{ 1 };
    cpp::PropertyVariable<std::string> _prop_to_send_as_string{ "Hello FEP3 World from CPP!" };
    cpp::PropertyVariable<double> _prop_to_send_as_double{ 0.1 };
    //it is possible to use vector properties 
    cpp::PropertyVariable<std::vector<std::string>> _prop_as_string_array{ {"value1", "value2", "value3"} };

    //in core API you need to deal everrything by yourself 
    //have a look at the fep3::cpp::DataJob in cpp API 
    cpp::DataWriter* _data_writer_string;
    cpp::DataWriter* _data_writer_ddl;
    cpp::DataWriter* _data_writer_ddl_array;
    cpp::DataWriter* _data_writer_plain_c_type;
};

int main(int argn, const char* argv[])
{
    try
    {
        auto part = cpp::createParticipant<cpp::DataJobElement<EasyCPPSenderDataJob>>(
            fep3_examples::getParticipantName(argn, argv, "demo_cpp_sender"),
            fep3_examples::getSystemName(argn, argv, "demo_system"));
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}
