#ifndef CMDEMO2_COMMANDEXECUTOR_HPP_
#define CMDEMO2_COMMANDEXECUTOR_HPP_

#include <exception>
#include <cm/api/transactionmanager.hpp>
#include "cli/include/commandvector.hpp"
#include "cmexamples/town.hpp"

using namespace cmdemo2;
using cmexample2::Town;
using configurationmanagement::api::TransactionManager;

namespace cmdemo2
{
    class CommandExecutor
    {
    public:
        class UnsupportedCommand;
        class UnexpectedArgumnentCount;
        class IllegalParameters;

        CommandExecutor();
        CommandExecutor(const CommandExecutor&) = delete;
        CommandExecutor& operator = (const CommandExecutor&) = delete;
        ~CommandExecutor();

        bool execute(const CommandVector& commands);

    private:
        Town town;
        std::shared_ptr<TransactionManager> transactionManager;
        std::string transactionGroup;
        std::string transactionID;

        bool hasModificationCommand(const CommandVector& commands);
        void addBuilding(const CommandWithArguments& command);
        void deleteBuilding(const CommandWithArguments& command);
        void showBuildings(const CommandWithArguments& command);
        void addCitizen(const CommandWithArguments& command);
        void deleteCitizen(const CommandWithArguments& command);
        void showCitizens(const CommandWithArguments& command);
        void clear(const CommandWithArguments& command);
    };

    class CommandExecutor::UnsupportedCommand: public std::invalid_argument
    {
    public:
        UnsupportedCommand() = delete;
        explicit UnsupportedCommand(const std::string& msg): std::invalid_argument(msg) { }
        virtual ~UnsupportedCommand() noexcept { }
    };

    class CommandExecutor::UnexpectedArgumnentCount: public std::invalid_argument
    {
    public:
        UnexpectedArgumnentCount() = delete;
        explicit UnexpectedArgumnentCount(const std::string& msg): std::invalid_argument(msg) { }
        virtual ~UnexpectedArgumnentCount() noexcept { }
    };

    class CommandExecutor::IllegalParameters: public std::invalid_argument
    {
    public:
        IllegalParameters() = delete;
        explicit IllegalParameters(const std::string& msg): std::invalid_argument(msg) { }
        virtual ~IllegalParameters() noexcept { }
    };
}

#endif
