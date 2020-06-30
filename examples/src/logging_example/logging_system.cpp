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

#include <fep_system/fep_system.h>

#include <a_util/system.h>
#include <iostream>

struct DemoEventMonitor : public fep3::IEventMonitor
{
    DemoEventMonitor() = default;

    // This method will be called everytime a log is send to the system via RPC
    void onLog(std::chrono::milliseconds log_time,
        fep3::logging::Severity severity_level,
        const std::string& participant_name,
        const std::string& logger_name,
        const std::string& message) override
    {
        std::cout << message << std::endl;
    }
};

int main(int argn, const char* argv[])
{
    auto my_system = fep3::discoverSystem("demo_logging");
    DemoEventMonitor my_monitor;
    my_system.registerMonitoring(my_monitor);

    my_system.load();
    my_system.initialize();
    my_system.start();

    a_util::system::sleepMilliseconds(5 * 1000);

    auto logging_service_proxy = my_system.getParticipant("demo_logging_participant").getRPCComponentProxyByIID<fep3::rpc::IRPCLoggingService>();
    // The logger called "demo" is set to only log to the console (of the participant) thus disabling the logging via RPC to the system
    logging_service_proxy->setLoggerFilter("demo", { fep3::logging::Severity::info, {"console"} });
    std::cout << "RPC logging disabled" << std::endl;

    a_util::system::sleepMilliseconds(5 * 1000);

    // Console is still activated but the severity level has been raised which filters out the info log
    logging_service_proxy->setLoggerFilter("demo", { fep3::logging::Severity::warning, {"console"} });
    std::cout << "Severity level raised to warning" << std::endl;

    a_util::system::sleepMilliseconds(5 * 1000);

    my_system.stop();
    my_system.deinitialize();
    my_system.unload();
    my_system.shutdown();
}