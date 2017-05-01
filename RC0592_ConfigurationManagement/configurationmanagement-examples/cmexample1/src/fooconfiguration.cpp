#include "fooconfiguration.hpp"
#include <boost/lexical_cast.hpp>
#include "protobuf/cmexample1domain/requestinterface/readrequest.pb.h"
#include "protobuf/cmexample1domain/requestinterface/readresponse.pb.h"
#include "protobuf/cmexample1domain/requestinterface/readwriterequest.pb.h"
#include "protobuf/cmexample1domain/requestinterface/readwriteresponse.pb.h"
#include <syslog.h>


using namespace cmexample1;
using namespace cmexample1domain::requestinterface;

namespace
{
    void checkReadWriteResponse(const ReadWriteResponse& response);
    void checkReadResponse(const ReadResponse& response);
}

FooConfiguration::FooConfiguration():
        domainName("cmexample1domain"),
        requestDispatcher(configurationmanagement::api::RequestDispatcher::create())
{
}

void FooConfiguration::writeFoo(const std::string& txid, const Foo& foo)
{
    ReadWriteRequest readWriteRequest;
    readWriteRequest.set_type(ReadWriteRequest::WRITE_FOO);
    readWriteRequest.mutable_writefoorequest()->mutable_foo()->set_id(foo.id);
    readWriteRequest.mutable_writefoorequest()->mutable_foo()->set_description(foo.description);
    readWriteRequest.mutable_writefoorequest()->mutable_foo()->set_fsipaclrulesrc(foo.fsipACLRuleSrc);
    readWriteRequest.mutable_writefoorequest()->mutable_foo()->set_fsipaclruleindex(foo.fsipACLRuleIndex);
    readWriteRequest.mutable_writefoorequest()->mutable_foo()->set_fsipaclruleaction(foo.fsipACLRuleAction);
    for (const auto& val : foo.values)
        readWriteRequest.mutable_writefoorequest()->mutable_foo()->add_value(val);
    syslog(LOG_INFO, "writeFoo fsipaclruleindex: %s %s", (readWriteRequest.mutable_writefoorequest()->mutable_foo()->fsipaclruleindex()).c_str(),foo.fsipACLRuleIndex.c_str());
        
    ReadWriteResponse readWriteResponse;
    requestDispatcher->readWriteRequest(txid, domainName, readWriteRequest).parse(readWriteResponse);
    checkReadWriteResponse(readWriteResponse);
}

std::shared_ptr<cmexample1::Foo> FooConfiguration::readFoo(const std::string& txid, std::string fsipaclruleindex)
{
    auto foo = std::make_shared<Foo>();

    ReadRequest readRequest;
    readRequest.set_type(ReadRequest::READ_FOO);
    //readRequest.mutable_readfoorequest()->set_fooid(fooId);
    readRequest.mutable_readfoorequest()->set_fsipaclruleindex(fsipaclruleindex);
    syslog(LOG_INFO, "readFoo fsipaclruleindex: %s %s", (readRequest.mutable_readfoorequest()->fsipaclruleindex()).c_str(),fsipaclruleindex.c_str());
         

    ReadResponse readResponse;
    requestDispatcher->readOnlyRequest(txid, domainName, readRequest).parse(readResponse);
    checkReadResponse(readResponse);

    if (readResponse.mutable_readfooresponse()->has_foo())
    {
        foo->id = readResponse.mutable_readfooresponse()->foo().id();
        if (readResponse.mutable_readfooresponse()->foo().has_description())
            foo->description = readResponse.mutable_readfooresponse()->foo().description();
        for (const auto& val : readResponse.mutable_readfooresponse()->foo().value())
            foo->values.insert(val);
        
        if (readResponse.mutable_readfooresponse()->foo().has_fsipaclrulesrc())
        {
            foo->fsipACLRuleSrc = readResponse.mutable_readfooresponse()->foo().fsipaclrulesrc();
        }
        if (readResponse.mutable_readfooresponse()->foo().has_fsipaclruleindex())
        {
            foo->fsipACLRuleIndex = readResponse.mutable_readfooresponse()->foo().fsipaclruleindex();
        }
        if (readResponse.mutable_readfooresponse()->foo().has_fsipaclruleaction())
        {
            foo->fsipACLRuleAction = readResponse.mutable_readfooresponse()->foo().fsipaclruleaction();
        }
    }

    return foo;
}

std::shared_ptr<FooConfiguration::FooIdDescriptionPairs> FooConfiguration::readDescriptions(const std::string& txid)
{
    auto descriptions = std::make_shared<FooIdDescriptionPairs>();

    ReadRequest readRequest;
    readRequest.set_type(ReadRequest::READ_ALL_DESCRIPTIONS);

    ReadResponse readResponse;
    requestDispatcher->readOnlyRequest(txid, domainName, readRequest).parse(readResponse);
    checkReadResponse(readResponse);

    for (const auto& i : readResponse.mutable_readalldescriptionsresponse()->fooiddescriptionpair())
        descriptions->insert(std::make_pair(i.fooid(), i.description()));
    return descriptions;
}


void FooConfiguration::deleteFoo(const std::string& txid, std::string fsipACLRuleIndex)
{
    ReadWriteRequest readWriteRequest;
    readWriteRequest.set_type(ReadWriteRequest::DELETE_FOO);
    readWriteRequest.mutable_deletefoorequest()->set_fsipaclruleindex(fsipACLRuleIndex);

    ReadWriteResponse readWriteResponse;
    requestDispatcher->readWriteRequest(txid, domainName, readWriteRequest).parse(readWriteResponse);
    checkReadWriteResponse(readWriteResponse);
}

namespace
{
    void checkReadWriteResponse(const ReadWriteResponse& response)
    {
        if (response.status() == ReadWriteResponse::FOO_ID_MISSING)
            throw FooConfiguration::FooIdMissingException("Obligatory parameter Foo ID missing.");
        if (response.status() != ReadWriteResponse::SUCCESS)
            throw FooConfiguration::InternalErrorException("Internal error: " + boost::lexical_cast<std::string>(response.status()));
    }

    void checkReadResponse(const ReadResponse& response)
    {
        if (response.status() == ReadResponse::FOO_NOT_FOUND)
            throw FooConfiguration::FooNotFoundException("Given Foo ID does not exist. ID: " + response.readfooresponse().foo().fsipaclruleindex());
        if (response.status() != ReadResponse::SUCCESS)
            throw FooConfiguration::InternalErrorException("Internal error: " + boost::lexical_cast<std::string>(response.status()));
    }
}
