#ifndef CMEXAMPLE1_FOO_HPP_
#define CMEXAMPLE1_FOO_HPP_

#include <string>
#include <set>

namespace cmexample1
{
    struct Foo
    {
    public:
        int id;
        std::string description;
        std::set<std::string> values;
        
        std::string fsipNetworkObjectName;
        std::string fsipNetworkObjectOwner;
        std::string fsipNetworkObjectAddPhase;
        std::string fsipNetworkObjectDelPhase;
        std::string fsipNetworkObjectPriority;
        std::string fsipNetworkObjectDynamic;
        std::string fsipACLRuleIndex;
        std::string fsipACLRuleChain;
        std::string fsipACLRuleAction;
        std::string fsipACLRuleTable;
        std::string fsipACLRuleFamily;
        std::string fsipACLRuleDSCP;
        std::string fsipACLRuleInInterface;
        std::string fsipACLRuleOutInterface;
        std::string fsipACLRuleProtoco;
        std::string fsipACLRuleSrcPortStart;
        std::string fsipACLRuleDstPortStart;
        std::string fsipACLRuleSrcPortEnd;
        std::string fsipACLRuleDstPortEnd;
        std::string fsipACLRuleSrc;
        std::string fsipACLRuleDst;
        std::string fsipACLRuleSrcLength;
        std::string fsipACLRuleDstLength;
        std::string fsipACLRuleActionDSCP;
        std::string fsipACLRuleVRF;
        std::string fsipACLRuleICMPType;
        std::string fsipACLRuleICMPCode;
        std::string fsipACLRuleLimit;
        std::string fsipACLRuleLimitBurst;
        
        std::string fsipNetworkObjectPostDelCmd; 
        std::string fsipNetworkObjectNodeName;
        std::string fsipNetworkObjectUserModifiable;
        std::string fsipNetworkObjectUserDeletable;
        std::string fsipNetworkObjectUser;
        std::string fsipNetworkObjectDependency;
        std::string fsipNetworkObjectPreAddCmd;
        std::string fsipNetworkObjectPostAddCmd;
        std::string fsipNetworkObjectPreDelCmd;
        std::string objectClass;
        std::string key;

    };
}
#endif
