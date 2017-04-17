#include <csignal>
#include <chrono>
#include <thread>
#include <syslog.h>
#include <sstream>
#include <cm/api/transactionmanager.hpp>
#include "cmexamples/town.hpp"

using namespace std::chrono;
using configurationmanagement::api::TransactionManager;
using cmexample2::Town;

namespace
{
    volatile sig_atomic_t sighupReceived = 0;

    void writeConfigurationToLog(const Town::Summary& summary, const Town::Infrastructure& infrastructure)
    {
        std::stringstream ss;
        ss << "Summary:\n  population: " << summary.population << "\n  finance: " << summary.finance;
        if (!infrastructure.buildings.empty())
            ss << "\nBuildings:";
        for (const auto& building : infrastructure.buildings)
        {
            ss << "\n  " << building.name;
            ss << "\n    free places:" << building.freeSpaces;
            if (!building.citizens.empty())
                ss << "\n    citizens:";
            for (const auto& citizen : building.citizens)
                ss << "\n      " << citizen;
        }
        syslog(LOG_INFO,"%s", ss.str().c_str());
    }

    void readConfiguration()
    {
        auto transactionManager(TransactionManager::create());
        auto transactionID = transactionManager->startReadOnly("cmexample2trgroup");
        Town town;
        writeConfigurationToLog(town.getSummary(transactionID), town.getInfrastructure(transactionID));
        transactionManager->commit(transactionID);
    }

    void sigHandler(int signo)
    {
        if (signo == SIGHUP)
            sighupReceived = 1;
    }
}

int main()
{
    openlog("cmdemo2daemon", LOG_PID, LOG_DAEMON);

    if (signal(SIGHUP, sigHandler) == SIG_ERR)
        syslog(LOG_ERR,"Signal handler register failed!");

    while(true)
    {
        if (sighupReceived)
        {
            sighupReceived = 0;
            readConfiguration();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
