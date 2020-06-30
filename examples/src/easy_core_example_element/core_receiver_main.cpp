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

#include "example_commandline_helper.hpp"
#include "example_ddl_types.h"

#include <fep3/core.h>
#include <fep3/core/element_configurable.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class EasyReceiverJob : public IJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit EasyReceiverJob(
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

class EasyCoreReceiverElement : public core::ElementConfigurable
{
public:
    //Impleemntation of the CTOR!
    //ElementConfigurable has no default CTOR 
    // you must define a type of your element -> to identifiy you implementation in a system
    // you must define a implementation version -> to identifiy you implementation version in a system
    // KEEP in MIND THIS IS NOT THE ELEMENT INSTANCE NAME! 
    EasyCoreReceiverElement()
        : core::arya::ElementConfigurable("Demo Element Base Receiver Type",
            FEP3_PARTICIPANT_LIBRARY_VERSION_STR)
    {
        //create DataAccess with the reader class
        //this will create a data writer for strings
        _data_reader_string = std::make_shared<core::DataReader>("string_data",
            StreamTypeString());

        //this will create a data writer for ddl based structures
        _data_reader_ddl = std::make_shared<core::DataReader>("ddl_data",
            StreamTypeDDL(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_reader_ddl_array = std::make_shared<core::DataReader>("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for dynamic arrays of DDL based structures
        _data_reader_ddl_array = std::make_shared<core::DataReader>("ddl_array",
            StreamTypeDDLArray(fep3_examples::examples_ddl_struct, fep3_examples::examples_ddl_description, 32));

        //this will create a data writer for fixed size uint32_t values
        _data_reader_plain_c_type = std::make_shared<core::DataReader>("plain_c_type_int32_t",
            StreamTypePlain<int32_t>());

        //this Job will connect the process methods to the scheduler
        //you may also use another option, consider cpp::DataJob i.e. 
        _my_job = std::make_shared<EasyReceiverJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataIn(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; });
    }

    fep3::Result initialize() override
    {
        //register the job
        auto job_adding_res = core::addToComponents("receiver_job", _my_job, { 1s }, *getComponents());
        if (isFailed(job_adding_res))
        {
            return job_adding_res;
        }
        //register the data
        auto data_adding_res = core::addToComponents(*_data_reader_string.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_reader_ddl.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_reader_ddl_array.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        data_adding_res = core::addToComponents(*_data_reader_plain_c_type.get(), *getComponents());
        if (isFailed(data_adding_res)) return data_adding_res;
        return {};
    }

    void deinitialize() override
    {
        //very important in the core API ... you need to synchronously register and unregister your jobs and data
        core::removeFromComponents("receiver_job", *getComponents());
        core::removeFromComponents(*_data_reader_string.get(), *getComponents());
        core::removeFromComponents(*_data_reader_ddl.get(), *getComponents());
        core::removeFromComponents(*_data_reader_ddl_array.get(), *getComponents());
        core::removeFromComponents(*_data_reader_plain_c_type.get(), *getComponents());
    }

    fep3::Result processDataIn(fep3::Timestamp sim_time_of_execution)
    {
        //this is to flush and write it to the bus 
        _data_reader_string->receiveNow(sim_time_of_execution);
        _data_reader_ddl->receiveNow(sim_time_of_execution);
        _data_reader_ddl_array->receiveNow(sim_time_of_execution);
        _data_reader_plain_c_type->receiveNow(sim_time_of_execution);
        return {};
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

    //in core API you need to deal everrything by yourself 
    //have a look at the fep3::cpp::DataJob in cpp API 
    std::shared_ptr<EasyReceiverJob> _my_job;
    std::shared_ptr<core::DataReader> _data_reader_string;
    std::shared_ptr<core::DataReader> _data_reader_ddl;
    std::shared_ptr<core::DataReader> _data_reader_ddl_array;
    std::shared_ptr<core::DataReader> _data_reader_plain_c_type;
};


int main(int argn, const char* argv[])
{
    try
    {
        auto part = core::createParticipant<core::ElementFactory<EasyCoreReceiverElement>>(
            fep3_examples::getParticipantName(argn, argv, "demo_core_receiver"),
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
