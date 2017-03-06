
#ifndef ACL_RULE_MANAGER_HPP_
#define ACL_RULE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/optional.hpp>
#include <boost/asio.hpp>
#include "CommonFunc.hpp"

using namespace std;
using namespace boost;


template <class T>
using Optional = boost::optional<T>;

template <class T>
using In = const T&;

template <class T>
using Out = T&;

template <class T> /**< Same C++ syntax as Out */
using InOut = T&;



using IndexList    = vector<int>;
using IpAddress    = asio::ip::address;


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



struct PortRange
{
    int lowValue;
    int highValue;
};

using PortRanges = vector<PortRange>;


typedef enum{
   Input,
   OutputDscp,
   //OutputOthers,
   /*new type should be added above this line*/
   endOfAclChainType
}ACLChainType;

//#define IndexList (vector<int>)

struct Rule
{
    int flowType;
    optional<int> transportNetworkType;
    optional<int> precedence;
    optional<IpAddress> srcIp;
    optional<int> srcPrefixLength;
    optional<PortRanges> srcPortRanges;
    optional<IpAddress> dstIp;
    optional<int> dstPrefixLength;
    optional<PortRanges> dstPortRanges;
};



#if 0
struct ACLRule
{
    ACLChainType chainType;
    int rule;
    int dscp;
    vector<int> ruleIndexList; 
};
#endif

struct ACLRule
{
    ACLChainType chainType;
    Rule rule;
    int dscp;
    IndexList ruleIndexList; 
};




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
         
    vector<ACLRule> ruleList;
    vector<IndexList > indexPool; /* 2 index list corresponding to input, output dscp and output others */
};

#endif
