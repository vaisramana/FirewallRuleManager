#include <iostream>
#include "cli/include/commandparser.hpp"
#include "cli/include/commandexecutor.hpp"

using namespace cmdemo2;

namespace
{
    void printUsage(const std::string executable)
    {
        std::cerr << "\nUsage:" << std::endl;
        std::cerr << "\n   " << executable << " cmd arg1 arg2 ..." << std::endl;
        std::cerr << "\n      or" << std::endl;
        std::cerr << "\n   " << executable << " [cmd arg1 arg2 ...] [cmd arg1 arg2 ...] ..." << std::endl;
        std::cerr << "\n   Later one will execute multiple commands in one transaction." << std::endl;
        std::cerr << "\n   Supported commands: " << std::endl;
        std::cerr << "\n      addb <building_name> <opex> <citizen_limit>" << std::endl;
        std::cerr << "\n         Add building." << std::endl;
        std::cerr << "\n         <building_name> = Unique name for building (string)." << std::endl;
        std::cerr << "         <opex> = Operating expenses (unsigned integer)." << std::endl;
        std::cerr << "         <citizen_limit> = Maximum number of citizens inside building (unsigned integer)." << std::endl;
        std::cerr << "\n      delb <building_name>" << std::endl;
        std::cerr << "\n         Delete building." << std::endl;
        std::cerr << "\n         <building_name> = Name of building to be deleted (string)." << std::endl;
        std::cerr << "\n      showb" << std::endl;
        std::cerr << "\n         Show buildings." << std::endl;
        std::cerr << "\n      addc <citizen_name> <tax> <building_name>" << std::endl;
        std::cerr << "\n         Add citizen." << std::endl;
        std::cerr << "\n         <citizen_name> = Unique name for citizen (string), limited character set" << std::endl;
        std::cerr << "                          (alphabetic characters, numbers, spaces, dots and hyphens)." << std::endl;
        std::cerr << "                          If name contains spaces it must be given inside quotes." << std::endl;
        std::cerr << "         <tax> = Citizen tax for town balance (unsigned integer)." << std::endl;
        std::cerr << "         <building_name> = Building where citizen in located to (string)." << std::endl;
        std::cerr << "\n      delc <citizen_name>" << std::endl;
        std::cerr << "\n         Delete citizen." << std::endl;
        std::cerr << "\n         <citizen_name> = Name of citizen to be deleted (string)." << std::endl;
        std::cerr << "\n      showc" << std::endl;
        std::cerr << "\n         Show citizens." << std::endl;
        std::cerr << "\n      clear" << std::endl;
        std::cerr << "\n         Clear all town data." << std::endl;
        std::cerr << "\n   e.g." << std::endl;
        std::cerr << "\n   " << executable << " [addb Library 10000 25] [addc John 5000 Library] [addc Jack 7000 Library]" << std::endl;
        std::cerr << "\n   " << executable << " showb\n" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    CommandParser commandParser;
    CommandExecutor commandExecutor;

    auto commands = commandParser.parse(argc, argv);
    if (commands.empty())
    {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!commandExecutor.execute(commands))
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
