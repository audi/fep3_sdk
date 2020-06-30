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

#include <iostream>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

/**
 * This implementation demonstrates the basics of how to set up and use custom loggers.
 * If a custom logger is not needed fep3::logging::EasyLogging is recommended
 * which is part of fep3::core::ElementBase and provides convenient macros.
 */
class DemoLoggingElement : public fep3::core::ElementBase
{
public:
    DemoLoggingElement() : fep3::core::ElementBase("LoggingElementType", "1.0.0")
    {
        // Usually logging is part of other jobs and not a job by itself
        _my_job = std::make_shared<core::Job>(
            "logging_job",
            1s,
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); }
        );
    }

    fep3::Result load() override
    {
        auto logging_service = getComponents()->getComponent<fep3::ILoggingService>();
        if (logging_service)
        {
            _logger = logging_service->createLogger("demo");
        }
        else
        {
            std::cout << "Error: no logging service found!" << std::endl;
            return fep3::ERR_BAD_DEVICE;
        }

        std::cout << "Successfully created logger" << std::endl;
        return{};
    }

    fep3::Result initialize() override
    {
        return core::addToComponents({ _my_job }, *getComponents());
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        return _logger->logInfo("This is a log");
    }

private:
    std::shared_ptr<core::Job> _my_job;
    std::shared_ptr<ILoggingService::ILogger> _logger;
};

int main(int argn, const char* argv[])
{
    using namespace fep3::core;
    //creating a participant with the element that will send the logs
    auto demo_participant = createParticipant<ElementFactory<DemoLoggingElement>>(
        "demo_logging_participant",
        "1.0.0",
        "demo_logging");
    //wait until exit is send
    return demo_participant.exec();
}