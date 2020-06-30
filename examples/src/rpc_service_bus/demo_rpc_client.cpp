/**
 * @file 
 * @copyright Audi AG
 *            All right reserved.
 *
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 */

#include <fep3/core.h>

#include <fep3/components/service_bus/service_bus_intf.h>
#include <fep3/components/service_bus/rpc/fep_rpc.h>

//within this header we now find a demo_rpc_stubs::DemoClientStub implementation 
#include <demo_rpc_client_stub.h>
#include <string>
#include <iostream>

//definiton of the interface identifier 
class IDemoRPCService
{
public:
    FEP_RPC_IID("demo_service.iid", "demo_service_defaultname");
};

//definiton and implementation of the client
class DemoRPCClient :
    public fep3::rpc::RPCServiceClient<demo_rpc_stubs::DemoClientStub,
    IDemoRPCService>
{
    typedef fep3::rpc::RPCServiceClient<demo_rpc_stubs::DemoClientStub,
        IDemoRPCService> super;
public:
    DemoRPCClient(const std::string& service_name,
        const std::shared_ptr<fep3::rpc::IRPCRequester>& rpc_requester)
        : super(service_name,
            rpc_requester)
    {
    }
};

//the element that uses a client to retrieve information from a service
class DemoClientElement : public fep3::core::ElementBase
{
public:
    DemoClientElement() : fep3::core::ElementBase("ElementClientType", "1.0.0")
    {
    }

    fep3::Result initialize() override
    {
        //retrieve the service bus component 
        auto service_bus = getComponents()->getComponent<fep3::IServiceBus>();
        if (service_bus)
        {
            //retrieve a requester for the participant you want to
            //get the service from
            // YES! You need to know the name or the address of the other participant! 
            // by using the name only ... discovery must be switched on!
            auto requester = service_bus->getRequester("demo_rpc_service_participant");
            if (requester)
            {
                DemoRPCClient client(IDemoRPCService::getRPCDefaultName(),
                    requester);

                //the bool value
                auto bool_response = client.getABoolValue();
                std::cout << "calling getABoolValue - result: " << bool_response << std::endl;
                //the integer value
                auto int_response = client.getAIntegerValue();
                std::cout << "calling getAIntegerValue - result: " << int_response << std::endl;
                //the double value
                auto double_response = client.getAFloatValue();
                std::cout << "calling getAFloatValue - result: " << double_response << std::endl;
                //a complex return value and a paramer
                auto complex_response = client.getAOptionalTimestamp(123);
                auto is_valid = complex_response["valid"].asBool();
                auto timestamp_response = complex_response["64bitTimeStamp"].asInt64();
                std::cout << "calling getAOptionalTimestamp - result: " << is_valid << " -> " << timestamp_response << std::endl;
            }
            else
            {
                std::cout << "Error: no default requester can be retrieved from the service bus!";
                return fep3::ERR_DEVICE_NOT_READY;
            }
        }
        else
        {
            std::cout << "Error: no service bus found!";
            return fep3::ERR_BAD_DEVICE;
        }
        return {};
    }
};

int main(int, const char**)
{
    using namespace fep3::core;
    //creating a participant with an element that will access a service while initializing
    auto demo_participant = createParticipant<ElementFactory<DemoClientElement>>(
        "demo_rpc_client_participant",
        "1.0.0",
        "demo_rpc");

    return demo_participant.exec();
}

