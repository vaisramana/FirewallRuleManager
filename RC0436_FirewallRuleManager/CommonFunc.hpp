#ifndef COMMON_FUNC_HPP_
#define COMMON_FUNC_HPP_


#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/optional.hpp>
#include <boost/asio.hpp>

//#include "ACLRuleManager.hpp"



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

/*
3000                    |MIN
[3000,4000]          |DEFAULT TOP RULE
(4000,5000)          |MARGIN
[5000,9000]          |OUTPUT DSCP
(9000,10000)        |MARGIN
[10000,14000]      |OUTPUT OTHER
(14000,15000)      |MARGIN
[15000,19000]      |INPUT
(19000,20000)      |MARGIN
[20000,21000]      |DEFAULT BOTTOM RULE
(21000,22000)      |MARGIN
22000                  |MAX
*/

#define ACL_CHAIN_INDEX_MIN (3000)
#define ACL_CHAIN_INDEX_RANGE (4000)
#define ACL_CHAIN_INDEX_MARGIN (1000)

#define ACL_DEFAULT_TOP_INDEX_MIN (ACL_CHAIN_INDEX_MIN)
#define ACL_DEFAULT_TOP_INDEX_MAX (ACL_DEFAULT_TOP_INDEX_MIN+ACL_CHAIN_INDEX_MARGIN)

#define ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN (ACL_DEFAULT_TOP_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX (ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)

#define ACL_OUTPUT_CHAIN_OTHER_INDEX_MIN (ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_OUTPUT_CHAIN_OTHER_INDEX_MAX (ACL_OUTPUT_CHAIN_OTHER_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)

#define ACL_INPUT_CHAIN_INDEX_MIN (ACL_OUTPUT_CHAIN_OTHER_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_INPUT_CHAIN_INDEX_MAX (ACL_INPUT_CHAIN_INDEX_MIN+ACL_CHAIN_INDEX_RANGE)

#define ACL_DEFAULT_BOTTOM_INDEX_MIN (ACL_INPUT_CHAIN_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)
#define ACL_DEFAULT_BOTTOM_INDEX_MAX (ACL_DEFAULT_BOTTOM_INDEX_MIN+ACL_CHAIN_INDEX_MARGIN)

#define ACL_CHAIN_INDEX_MAX (ACL_DEFAULT_BOTTOM_INDEX_MAX+ACL_CHAIN_INDEX_MARGIN)

enum class FlowType
{
    Bfd,
    CmpCrl,
    Dns,
    Icmp,
    IcmpEchoResponse,
    Igmp,
    Ike,
    IpSec,
    Mplane,
    Ntp,
    RndPort,
    SecurityOverEthernet,
    SiteSupportEquipment,
    Ssh,
    Syslog,
    Top,
    TwampReflector,
    TwampSender,
    LteCplane,
    LteUplane,
    WcdmaCplane,
    WcdmaSmallIp,
    WcdmaUplane,
    GsmCplane,
    GsmE2eDelayMeasurememt,
    GsmMplane,
    GsmUcsplane,
    GsmUpsplane
};


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


struct Rule
{
    FlowType flowType;
    optional<int> transportNetworkType;
    optional<int> precedence;
    optional<IpAddress> srcIp;
    optional<int> srcPrefixLength;
    optional<PortRanges> srcPortRanges;
    optional<IpAddress> dstIp;
    optional<int> dstPrefixLength;
    optional<PortRanges> dstPortRanges;
};


struct ACLRule
{
    ACLChainType chainType;
    Rule rule;
    int dscp;
    IndexList ruleIndexList; 
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




void printACLRuleList(vector<ACLRule>  ruleList);
void printVector(vector<int> v);
void printVectorVector(vector<vector<int> > v);

#endif


