#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include "cli/include/commandexecutor.hpp"

namespace
{
    unsigned int castToUint (const std::string& str);
}

CommandExecutor::CommandExecutor():
        transactionManager(TransactionManager::create()),
        transactionGroup("cmexample2trgroup")
{
}

CommandExecutor::~CommandExecutor()
{
}

bool CommandExecutor::execute(const CommandVector& commands)
{
    bool ret = true;
    try
    {
        if (hasModificationCommand(commands))
            transactionID = transactionManager->startReadWrite(transactionGroup);
        else
            transactionID = transactionManager->startReadOnly(transactionGroup);

        for (const auto& command : commands)
        {
            if (command[0].compare("addb") == 0)
                addBuilding(command);
            else if (command[0].compare("delb") == 0)
                deleteBuilding(command);
            else if (command[0].compare("showb") == 0)
                showBuildings(command);
            else if (command[0].compare("addc") == 0)
                addCitizen(command);
            else if (command[0].compare("delc") == 0)
                deleteCitizen(command);
            else if (command[0].compare("showc") == 0)
                showCitizens(command);
            else if (command[0].compare("clear") == 0)
                clear(command);
            else
                throw UnsupportedCommand("Unsupported command: " + boost::algorithm::join(command, " "));
        }

        std::cout << "\nCommitting changes..." << std::endl;
        transactionManager->commit(transactionID);
        std::cout << "\nCommitted." << std::endl;
    }
    catch (const TransactionManager::ValidationFailure& e)
    {
        std::cout << e.what() << std::endl;
        for (const auto& failures : e.validationResult.failedDomains)
        {
            std::cout << "Domain: " << failures.first << std::endl;
            std::cout << "Reason: " << failures.second.errorString << std::endl;
        }
        ret = false;
    }
    catch (const std::exception& e)
    {
        std::cout << "Execution failed: " << e.what() << std::endl;
        transactionManager->cancel(transactionID);
        ret = false;
    }
    std::cout << std::endl;
    return ret;
}

bool CommandExecutor::hasModificationCommand(const CommandVector& commands)
{
    for (const auto& command : commands)
        if (command[0].compare("showb") != 0 && command[0].compare("showc") != 0)
            return true;
    return false;
}

void CommandExecutor::addBuilding(const CommandWithArguments& command)
{
    if (command.size() != 4)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    Town::Building building;
    try
    {
        building.name = command[1];
        building.opex = castToUint(command[2]);
        building.citizenLimit = castToUint(command[3]);
    }
    catch (const std::exception& e)
    {
        throw IllegalParameters("Illegal parameters in command: " + boost::algorithm::join(command, " "));
    }

    std::cout << "\nAdding building " << command[1] << "..." << std::endl;
    town.addBuilding(transactionID, building);
}

void CommandExecutor::deleteBuilding(const CommandWithArguments& command)
{
    if (command.size() != 2)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    std::cout << "\nDeleting building " << command[1] << "..." << std::endl;
    town.deleteBuilding(transactionID, command[1]);
}

void CommandExecutor::showBuildings(const CommandWithArguments& command)
{
    if (command.size() != 1)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    int totalOpex = 0;
    std::cout << "\nReading buildings..." << std::endl;
    for (const auto& building : town.getInfrastructure(transactionID).buildings)
    {
        std::cout << "\nBuilding: " << building.name << std::endl;
        std::cout << "Opex: " << building.opex << std::endl;
        totalOpex += building.opex;
        std::cout << "Free spaces: " << building.freeSpaces << std::endl;
        std::cout << "Citizens:";
        for (const auto& citizen : building.citizens)
            std::cout << " " << citizen;
        std::cout << std::endl;
    }
    std::cout << "\nTotal opex: " << totalOpex << std::endl;
}

void CommandExecutor::addCitizen(const CommandWithArguments& command)
{
    if (command.size() != 4)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    Town::Citizen citizen;
    try
    {
        citizen.name = command[1];
        citizen.tax = castToUint(command[2]);
        citizen.building = command[3];
    }
    catch (const std::exception& e)
    {
        throw IllegalParameters("Illegal parameters in command: " + boost::algorithm::join(command, " "));
    }

    std::cout << "\nAdding citizen " << command[1] << "..." << std::endl;
    town.addCitizen(transactionID, citizen);
}

void CommandExecutor::deleteCitizen(const CommandWithArguments& command)
{
    if (command.size() != 2)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    std::cout << "\nDeleting citizen " << command[1] << "..." << std::endl;
    town.deleteCitizen(transactionID, command[1]);
}

void CommandExecutor::showCitizens(const CommandWithArguments& command)
{
    if (command.size() != 1)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    int totalTax = 0;
    std::cout << "\nReading citizens..." << std::endl;
    for (const auto& citizen : town.getCitizens(transactionID))
    {
        std::cout << "\nCitizen: " << citizen.name << std::endl;
        std::cout << "Tax: " << citizen.tax << std::endl;
        totalTax += citizen.tax;
        std::cout << "Building: " << citizen.building << std::endl;
    }
    std::cout << "\nTotal tax income: " << totalTax << std::endl;
}

void CommandExecutor::clear(const CommandWithArguments& command)
{
    if (command.size() != 1)
        throw UnexpectedArgumnentCount("Unexpected number of arguments: " + boost::algorithm::join(command, " "));

    std::cout << "\nClearing all data..." << std::endl;
    town.clearAll(transactionID);
}

namespace
{

unsigned int castToUint (const std::string& str)
{
    // Checked negative value separately, because e.g. "-1" cast directly to unsigned int will not cause bad_lexical_cast,
    // but will be successful and return 4294967295.
    auto ret = boost::lexical_cast<int>(str);
    if (ret < 0)
        throw std::invalid_argument("Negative value not allowed: " + str);
    return (unsigned int)ret;
}

}
