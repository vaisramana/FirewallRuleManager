#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <cm/api/transactionmanager.hpp>
#include "cmexamples/fooconfiguration.hpp"

using configurationmanagement::api::TransactionManager;
using cmexample1::Foo;
using cmexample1::FooConfiguration;

namespace
{
    void pause()
    {
        std::cout << std::endl << "Press ENTER to continue..." << std::endl;
        const int status(system("read i"));
        if ((!WIFEXITED(status)) || (WEXITSTATUS(status) != EXIT_SUCCESS))
        {
            std::cerr << "system() failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::string startReadOnlyTransaction(TransactionManager& transactionManager)
    {
        std::string transactionID;
        std::cout << std::endl << "Starting read-only transaction..." << std::endl;
        transactionID = transactionManager.startReadOnly("cmexample1trgroup");
        return transactionID;
    }

    std::string startReadWriteTransaction(TransactionManager& transactionManager)
    {
        std::string transactionID;
        std::cout << std::endl << "Starting read-write transaction..." << std::endl;
        transactionID = transactionManager.startReadWrite("cmexample1trgroup");
        return transactionID;
    }

    void commitTransaction(TransactionManager& transactionManager, const std::string& transactionID)
    {
        transactionManager.commit(transactionID);
        std::cout << "Transaction committed." << std::endl;
    }

    void writeFoo(Foo foo, FooConfiguration& fooConfiguration, const std::string& transactionID)
    {
        std::cout << "Writing fsipACLRuleIndex: " << foo.fsipACLRuleIndex << std::endl;
        fooConfiguration.writeFoo(transactionID, foo);
    }

    void readFoo(std::string id, FooConfiguration& fooConfiguration, const std::string& transactionID)
    {
        auto foo = fooConfiguration.readFoo(transactionID, id);
        
        std::cout << "Reading fsipACLRuleIndex: " << id << std::endl;
        /*
        std::cout << "    ID: " << foo->id << std::endl;
        std::cout << "    Description: " << foo->description << std::endl;
        std::cout << "    Value array: " << std::endl;
        for (const auto& val : foo->values)
            std::cout << "        Value: " << val << std::endl;
            */
        std::cout << "    fsipACLRuleIndex: " << foo->fsipACLRuleIndex << std::endl;
        std::cout << "    fsipACLRuleSrc: " << foo->fsipACLRuleSrc << std::endl;
        std::cout << "    fsipACLRuleAction: " << foo->fsipACLRuleAction << std::endl;
    }
#if 0
    void readAllDescriptions(FooConfiguration& fooConfiguration, const std::string& transactionID)
    {
        std::cout << "Reading all descriptions:" << std::endl;
        auto descriptionMap = fooConfiguration.readDescriptions(transactionID);
        for (const auto& i : *descriptionMap)
            std::cout << "ID: " << i.first << ", " << "Description: " << i.second << std::endl;
    }
#endif
    void deleteFoo(std::string id, FooConfiguration& fooConfiguration, const std::string& transactionID)
    {
        std::cout << "Deleting fsipACLRuleIndex: " << id << std::endl;
        fooConfiguration.deleteFoo(transactionID, id);
    }

    void writeFooConfigurationItems(TransactionManager& transactionManager, FooConfiguration& fooConfiguration)
    {
        auto transactionID = startReadWriteTransaction(transactionManager);

        Foo foo1;
        /*
        foo1.id = 1;
        foo1.description = "Description for first Foo.";
        foo1.values.insert("First value string for first Foo.");
        foo1.values.insert("Second value string for first Foo.");
        */
        foo1.fsipACLRuleIndex = "100";
        foo1.fsipACLRuleSrc = "1.1.1.1";
        foo1.fsipACLRuleAction = "accept";
        writeFoo(foo1, fooConfiguration, transactionID);

        Foo foo2;
        /*
        foo2.id = 2;
        foo2.description = "Description for second Foo.";
        foo2.values.insert("First value string for second Foo.");
        foo2.values.insert("Second value string for second Foo.");
        */
        foo2.fsipACLRuleIndex = "101";
        foo2.fsipACLRuleSrc = "2.2.2.2";
        foo2.fsipACLRuleAction = "drop";
        writeFoo(foo2, fooConfiguration, transactionID);

        commitTransaction(transactionManager, transactionID);
    }

    void readFooConfigurationItems(TransactionManager& transactionManager, FooConfiguration& fooConfiguration)
    {
        auto transactionID = startReadOnlyTransaction(transactionManager);
        readFoo("100", fooConfiguration, transactionID);
        readFoo("101", fooConfiguration, transactionID);
        commitTransaction(transactionManager, transactionID);
    }
#if 0

    void readAllDescriptions(TransactionManager& transactionManager, FooConfiguration& fooConfiguration)
    {
        auto transactionID = startReadOnlyTransaction(transactionManager);
        readAllDescriptions(fooConfiguration, transactionID);
        commitTransaction(transactionManager, transactionID);
    }
#endif
    void causeValidationFailure(TransactionManager& transactionManager, FooConfiguration& fooConfiguration)
    {
        auto transactionID = startReadWriteTransaction(transactionManager);

        Foo foo;
        foo.id = 3;
        foo.description = "Description for first Foo.";
        foo.values.insert("Some value.");
        writeFoo(foo, fooConfiguration, transactionID);

        try
        {
            commitTransaction(transactionManager, transactionID);
        }
        catch (const TransactionManager::ValidationFailure& e)
        {
            std::cout << "Commit failed: " << e.what() << std::endl;
        }
    }

    void deleteFooConfigurationItems(TransactionManager& transactionManager, FooConfiguration& fooConfiguration)
    {
        auto transactionID = startReadWriteTransaction(transactionManager);
        deleteFoo("101", fooConfiguration, transactionID);
        //deleteFoo("100", fooConfiguration, transactionID);
        commitTransaction(transactionManager, transactionID);
    }
}

int main()
{
    auto transactionManager(TransactionManager::create());
    FooConfiguration fooConfiguration;

    try
    {
        std::cout << std::endl << "STEP 1: Write Foo configuration items:" << std::endl;
        writeFooConfigurationItems(*transactionManager, fooConfiguration);

        pause();

        std::cout << std::endl << "STEP 2: Read Foo configuration items:" << std::endl;
        readFooConfigurationItems(*transactionManager, fooConfiguration);
#if 0
        pause();

        std::cout << std::endl << "STEP 3: Read all descriptions:" << std::endl;
        readAllDescriptions(*transactionManager, fooConfiguration);
#endif
        pause();

        std::cout << std::endl << "STEP 4: Cause validation failure:" << std::endl;
        causeValidationFailure(*transactionManager, fooConfiguration);

        pause();
        std::cout << std::endl << "STEP 5: Delete Foo configuration items:" << std::endl;
        deleteFooConfigurationItems(*transactionManager, fooConfiguration);

        pause();
        std::cout << std::endl << "STEP 6: Read Foo configuration items:" << std::endl;
        readFooConfigurationItems(*transactionManager, fooConfiguration);

        std::cout << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception during demo execution: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
