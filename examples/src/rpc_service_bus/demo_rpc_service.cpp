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

//within this header we now find a demo_rpc_stubs::DemoServiceStub implementation 
#include <demo_rpc_service_stub.h>
#include <string>
#include <iostream>

//definiton of the interface identifier 
class IDemoRPCService
{
public:
    FEP_RPC_IID("demo_service.iid", "demo_service_defaultname");
};

//definiton and implementation of the service
class DemoRPCService :
    public fep3::rpc::RPCService<demo_rpc_stubs::DemoServiceStub,
    IDemoRPCService>
{
    //the method declarations can be copied from the generated "demo_rpc_service_stub.h"
    bool getABoolValue() override
    {
        std::cout << "somebody requesting the getABoolValue " << std::endl;
        return false;
    }
    int getAIntegerValue() override
    {
        std::cout << "somebody requesting the getAIntegerValue " << std::endl;
        return 42;
    }
    double getAFloatValue() override
    {
        std::cout << "somebody requesting the getAFloatValue " << std::endl;
        return 1.23;
    }
    Json::Value getAOptionalTimestamp(int integer_parameter) override
    {
        std::cout << "somebody requesting the getAOptionalTimestamp " << std::endl;
        Json::Value result;
        result["valid"] = true;
        result["value_of_timestamp"] = std::to_string(integer_parameter);
        return result;
    }
};


class DemoServiceElement : public fep3::core::ElementBase
{
public:
    DemoServiceElement() : fep3::core::ElementBase("ServiceElementType", "1.0.0")
    {
    }
    fep3::Result load() override
    {
        //retrieve the service bus component 
        auto service_bus = getComponents()->getComponent<fep3::IServiceBus>();
        if (service_bus)
        {
            //retrieve the default server of this participant
            auto server = service_bus->getServer();
            if (server)
            {
                //create and register the service usually under its default name
                auto result = server->registerService(IDemoRPCService::getRPCDefaultName(),
                    std::make_shared<DemoRPCService>());
                if (fep3::isFailed(result))
                {
                    std::cout << "Error: could not register ther demo service because: " << result.getDescription();
                    return result;
                }
            }
            else
            {
                std::cout << "Error: no default server created within service bus found!";
                return fep3::ERR_DEVICE_NOT_READY;
            }
        }
        else
        {
            std::cout << "Error: no service bus found!" << std::endl;
            return fep3::ERR_BAD_DEVICE;
        }

        std::cout << "Successfully loaded demo service, client will do something while initializing!";
        return {};
    }

    fep3::Result destroy()
    {
        //do not forget to unregister
        auto service_bus = getComponents()->getComponent<fep3::IServiceBus>();
        if (service_bus)
        {
            auto server = service_bus->getServer();
            if (server)
            {
                server->unregisterService(IDemoRPCService::getRPCDefaultName());
            }
        }
        return {};
    }
};


int main(int, const char**)
{
    using namespace fep3::core;
    //creating a participant with the element that will provide a service
    auto demo_participant = createParticipant<ElementFactory<DemoServiceElement>>(
        "demo_rpc_service_participant",
        "1.0.0",
        "demo_rpc");
    //wait until exit is send
    return demo_participant.exec();
}
