#ifndef CMDEMO2_COMMANDPARSER_HPP_
#define CMDEMO2_COMMANDPARSER_HPP_

#include "cli/include/commandvector.hpp"

using namespace cmdemo2;

namespace cmdemo2
{
    class CommandParser
    {
    public:
        CommandParser();
        CommandParser(const CommandParser&) = delete;
        CommandParser& operator = (const CommandParser&) = delete;
        ~CommandParser();

        typedef std::vector<std::string> CommandWithArguments;
        typedef std::vector<CommandWithArguments> CommandVector;

        CommandVector parse(int argc, char* argv[]);

    private:
        std::string commandStartMark;
        std::string commandEndMark;

        bool checkCommandSyntax(int argc, char* argv[]);
        int getSectionStartCount(int argc, char* argv[]);
        int getSectionEndCount(int argc, char* argv[]);
        std::string cleanCommandStartEndMarks(const std::string& arg);
    };
}

#endif
