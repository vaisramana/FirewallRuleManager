
#ifndef ACL_RULE_MANAGER_HPP_
#define ACL_RULE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define ACL_CHAIN_INDEX_RANGE (10000)
#define ACL_CHAIN_INDEX_MIN (3000)
#define ACL_CHAIN_INDEX_MARGIN (3000)
#define ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN (ACL_CHAIN_INDEX_MIN)
#define ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX (ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)
#define ACL_OUTPUT_CHAIN_OTHER_INDEX_MIN (ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_OUTPUT_CHAIN_OTHER_INDEX_MAX (ACL_OUTPUT_CHAIN_OTHER_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)
#define ACL_INPUT_CHAIN_INDEX_MIN (ACL_OUTPUT_CHAIN_OTHER_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_INPUT_CHAIN_INDEX_MAX (ACL_INPUT_CHAIN_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)
#define ACL_CHAIN_INDEX_MAX (ACL_INPUT_CHAIN_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)


typedef enum{
   Input,
   OuputDscp,
   //OutputOthers,
   /*new type should be added above this line*/
   endOfAclChainType
}ACLChainType;

//#define IndexList (vector<int>)



struct ACLRule
{
    ACLChainType chainType;
    int rule;
    int dscp;
    vector<int> ruleIndexList; 
};



bool isless(const int & a, const int & b);




class ACLRuleManager
{
public:
    ACLRuleManager():indexPool(2){}
    bool addACLRule(ACLRule & rule, const int & indexNumberToAdd);
    bool delACLRule(ACLRule & rule);
    bool getACLRulePositionByRule(ACLRule & rule, int & pos);
    bool compareACLRule(const ACLRule & rule1, const ACLRule & rule2);
    bool getFreePoolIndex(const ACLChainType & chainType, const int & indexNumberToAdd, vector<int> & indexListToAdd);
    bool delPoolIndex(const ACLChainType & chainType, const vector<int> & indexListToDel);
    bool getIndexMinMax(const ACLChainType & chainType, int & indexMin, int & indexMax);
    bool getIndexList(const ACLChainType & chainType, vector<int> **indexListPtrPtr);
    bool findIndexPositionFromPool(const ACLChainType & chainType, const vector<int> & indexListToFind, vector<int> & indexPosListFound);
         
    vector<ACLRule> ruleList;
    vector<vector<int> > indexPool; /* 2 index list corresponding to input, output dscp and output others */
};

#endif
