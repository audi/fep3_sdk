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
#include <string>

#include "example_commandline_helper.hpp"
#include "example_ddl_types.h"

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class EasySenderJob : public IJob 
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit EasySenderJob(
        const ExecuteCall& ex_in,
        const ExecuteCall& ex,
        const ExecuteCall& ex_out) :
        _execute_func_data_in(ex_in),
        _execute_func(ex),
        _execute_func_data_out(ex_out)
    {
    }

    fep3::Result executeDataIn(Timestamp time_of_execution)
    {
        return _execute_func_data_in(time_of_execution);
    }
    fep3::Result execute(Timestamp time_of_execution)
    {
        return _execute_func(time_of_execution);
    }
    fep3::Result executeDataOut(Timestamp time_of_execution)
    {
        return _execute_func_data_out(time_of_execution);
    }
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_in;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_out;
};

class EasyCoreSenderElement : public core::ElementConfigurable
{
public:
    //Impleemntation of the CTOR!
    //ElementConfigurable has no default CTOR 
    // you must define a type of your element -> to identifiy you implementation in a system
    // you must define a implementation version -> to identifiy you implementation version in a system
    // KEEP in MIND THIS IS NOT THE ELEMENT INSTANCE NAME! 
    EasyCoreSenderElement() 
        : core::ElementConfigurable("Demo Element Base Sender Type",
                                    FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        //register the properties
        registerPropertyVariable(_prop_to_send_as_integer, "integer_value");
        registerPropertyVariable(_prop_to_send_as_string, "string_value");
        registerPropertyVariable(_prop_to_send_as_double, "double_value");
        registerPropertyVariable(_prop_as_string_array, "string_array_value");

        //create DataAccess with the writer class
        //this will create a data writer for strings
        _data_writer_string = std::make_shared<core::DataWriter>("string_data",
            StreamTypeString());

        //this will create a data writer for ddl based structures
        _data_writer_ddl = std::make_shared<core::DataWriter>("ddl_data",
            StreamTypeDDL(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = std::make_shared<core::DataWriter>("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_writer_ddl_array = std::make_shared<core::DataWriter>("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for fixed size uint32_t values
        _data_writer_plain_c_type = std::make_shared<core::DataWriter>("plain_c_type_int32_t",
            StreamTypePlain<int32_t>());

        //this Job will connect the process methods to the scheduler
        //you may also use another option, consider cpp::DataJob i.e. 
        _my_job = std::make_shared<EasySenderJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataOut(sim_time); });
    }

    fep3::Result initialize() override
    {
        //register the job
        auto job_adding_res = core::addToComponents("sender_job", _my_job, { 1s }, *getComponents());
        if (isFailed(job_adding_res))
        {
            return job_adding_res;
        }
        //register the data
        auto data_adding_res = core::addToComponents(*_data_writer_string.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_writer_ddl.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_writer_ddl_array.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_writer_plain_c_type.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        return {};
    }

    void deinitialize() override
    {
        //very important in the core API ... you need to synchronously register and unregister your jobs and data
        core::removeFromComponents("sender_job", *getComponents());
        core::removeFromComponents(*_data_writer_string.get(), *getComponents());
        core::removeFromComponents(*_data_writer_ddl.get(), *getComponents());
        core::removeFromComponents(*_data_writer_ddl_array.get(), *getComponents());
        core::removeFromComponents(*_data_writer_plain_c_type.get(), *getComponents());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
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

    fep3::Result processDataOut(fep3::Timestamp sim_time_of_execution)
    {
        //this is to flush and write it to the bus 
        _data_writer_string->flushNow(sim_time_of_execution);
        _data_writer_ddl->flushNow(sim_time_of_execution);
        _data_writer_ddl_array->flushNow(sim_time_of_execution);
        _data_writer_plain_c_type->flushNow(sim_time_of_execution);
        return {};
    }

    //use the PropertyVariable as easy readable configuration element
    //but do NOT forget to register the variables in the CTOR with a name
    core::PropertyVariable<int32_t> _prop_to_send_as_integer{ 1 };
    core::PropertyVariable<std::string> _prop_to_send_as_string{ "Hello FEP3 World!" };
    core::PropertyVariable<double> _prop_to_send_as_double{ 0.1 };
    //it is possible to use vector properties 
    core::PropertyVariable<std::vector<std::string>> _prop_as_string_array{ {"value1", "value2", "value3"} };

    //in core API you need to deal everrything by yourself 
    //have a look at the fep3::cpp::DataJob in cpp API 
    std::shared_ptr<EasySenderJob> _my_job;
    std::shared_ptr<core::DataWriter> _data_writer_string;
    std::shared_ptr<core::DataWriter> _data_writer_ddl;
    std::shared_ptr<core::DataWriter> _data_writer_ddl_array;
    std::shared_ptr<core::DataWriter> _data_writer_plain_c_type;
};


int main(int argn, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<EasyCoreSenderElement>>(
            fep3_examples::getParticipantName(argn, argv, "demo_core_sender"),
            "My Demo Participant Version 1.0", 
            fep3_examples::getSystemName(argn, argv, "demo_system"));
        return part.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 3;
    }
}
