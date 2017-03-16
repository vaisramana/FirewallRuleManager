
#ifndef ACL_RULE_MANAGER_HPP_
#define ACL_RULE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/optional.hpp>
#include <boost/asio.hpp>
#include "CommonFunc.hpp"





class ACLRuleManager
{
public:
    ACLRuleManager():indexPool(2){}
    bool addACLRule(InOut<ACLRule> rule, In<int> indexNumberToAdd);
    bool delACLRule(InOut<ACLRule> rule);
    bool getACLRulePositionByRule(InOut<ACLRule> rule, Out<int> pos);
    bool compareACLRule(In<ACLRule> rule1, In<ACLRule> rule2);
    bool getFreePoolIndex(In<ACLChainType> chainType, In<int> indexNumberToAdd, Out<IndexList> indexListToAdd);
    bool delPoolIndex(In<ACLChainType> chainType, In<IndexList> indexListToDel);
    bool getIndexMinMax(In<ACLChainType> chainType, Out<int> indexMin, Out<int> indexMax);
    bool getIndexList(In<ACLChainType> chainType, IndexList **indexListPtrPtr);
    bool findIndexPositionFromPool(In<ACLChainType> chainType, In<IndexList> indexListToFind, Out<IndexList> indexPosListFound);
    bool getRuleListByFlowtype(In<FlowType> flowType, Out<std::vector<ACLRule> > foundRuleList);

    bool addDefaultRules();
    bool getIpAndIfMap(Out<map<string, string> >ipMap, Out<map<string, string> >ifMap);
    bool convertIpAddress(In<string> inputAddress, Out<string> outputAddress);
    
         
    vector<ACLRule> ruleList;
    vector<IndexList > indexPool; /* 2 index list corresponding to input, output dscp and output others */
};

#endif
