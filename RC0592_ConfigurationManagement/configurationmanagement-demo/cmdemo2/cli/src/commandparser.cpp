#include <iostream>
#include <vector>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/erase.hpp>
#include "cli/include/commandparser.hpp"

CommandParser::CommandParser():
        commandStartMark("["),
        commandEndMark("]")
{
}

CommandParser::~CommandParser()
{
}

CommandParser::CommandVector CommandParser::parse(int argc, char* argv[])
{
    CommandVector commandVector;

    if (checkCommandSyntax(argc, argv))
    {
        CommandWithArguments command;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg(argv[i]);
            command.push_back(cleanCommandStartEndMarks(arg));
            if ((boost::ends_with(arg,commandEndMark)) || (i+1 == argc))
            {
                commandVector.push_back(command);
                command.clear();
            }
        }
    }

    return commandVector;
}

bool CommandParser::checkCommandSyntax(int argc, char* argv[])
{
    if (argc < 2)
        return false;

    if (getSectionStartCount(argc, argv) != getSectionEndCount(argc, argv))
        return false;

    return true;
}

int CommandParser::getSectionStartCount(int argc, char* argv[])
{
    int count = 0;
    for (int i = 1; i < argc; ++i)
        if (boost::starts_with(argv[i],commandStartMark))
            count++;
    return count;
}

int CommandParser::getSectionEndCount(int argc, char* argv[])
{
    int count = 0;
    for (int i = 1; i < argc; ++i)
        if (boost::ends_with(argv[i],commandEndMark))
            count++;
    return count;
}

std::string CommandParser::cleanCommandStartEndMarks(const std::string& arg)
{
    std::string cleanedArg(arg);

    if (boost::starts_with(cleanedArg,commandStartMark))
       boost::erase_head(cleanedArg,1);

    if (boost::ends_with(cleanedArg,commandEndMark))
        boost::erase_tail(cleanedArg,1);

    return cleanedArg;
}
