#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "ACLRuleManager.hpp"
#include "CommonFunc.hpp"

using namespace std;





void test1(vector<int> v)
{
    cout<<"test1"<<endl;
    vector<int>::iterator iter;
    for(iter=v.begin(); iter!=(v.end()-1); iter++)
    {
        cout<<" "<<(*(iter+1) - *iter);
        if((*(iter+1) - *iter) > 5)
        {
            v.insert(iter+1, 0);
        }
    }
    cout<<endl;
    //printVector(v);
    cout<<"end of test1"<<endl;
}

void func1(int b)
{
    b = 1;
}

void func2(int &b)
{
    b = 2;
}

void func3(int *b)
{
    *b = 3;
}

struct TestInnerClass
{
    void setInnerData(int input);
    int getInnerData();
    private:
    int innerData;
};


struct TestOuterClass
{
    void setInnerData(int input);
    int getInnerData();
    TestInnerClass * getInner_pointer();
    TestInnerClass & getInner_quote();
    private:
    int outerData;
    TestInnerClass inner;
};

void TestInnerClass::setInnerData(int input)
{
    innerData=input;
}

int TestInnerClass::getInnerData()
{
    return innerData;
}



TestInnerClass * TestOuterClass::getInner_pointer()
{
    return &inner;
}

TestInnerClass & TestOuterClass::getInner_quote()
{
    return inner;
}

void TestOuterClass::setInnerData(int input)
{
    inner.setInnerData(input);
}

int TestOuterClass::getInnerData()
{
    return inner.getInnerData();
}


void testcase_index_1()
{
    ACLRuleManager rulemanager;
    vector<int> v;
    bool result;

    cout<<"START testcase_index_1: ";
    cout<<"get index from beginning"<<endl;
    result = rulemanager.getFreePoolIndex(Input,4,v);
    assert(true == result);
    assert(v[0]==ACL_INPUT_CHAIN_INDEX_MIN);
    assert(v[1]==(ACL_INPUT_CHAIN_INDEX_MIN+1));
    assert(v[2]==(ACL_INPUT_CHAIN_INDEX_MIN+2));
    assert(v[3]==(ACL_INPUT_CHAIN_INDEX_MIN+3));
    cout<<"PASS testcase_index_1"<<endl;
}

void testcase_index_2()
{
    ACLRuleManager rulemanager;
    vector<int> v;
    bool result;

    cout<<"START testcase_index_2: ";
    cout<<"get more than maximum index number"<<endl;
    result = rulemanager.getFreePoolIndex(Input,ACL_CHAIN_INDEX_RANGE+1,v);
    assert(false == result);
    assert(true == rulemanager.indexPool[Input].empty());
    assert(true == v.empty());
    cout<<"PASS testcase_index_2"<<endl;
}

void testcase_index_3()
{
    ACLRuleManager rulemanager;
    vector<int> v;
    bool result;

    cout<<"START testcase_index_3: ";
    cout<<"delete all index"<<endl;
    rulemanager.getFreePoolIndex(Input,4,v);
    result = rulemanager.delPoolIndex(Input, v);

    assert(true == result);
    assert(true == rulemanager.indexPool[Input].empty());
    cout<<"PASS testcase_index_3"<<endl;
}

void testcase_index_4()
{
    ACLRuleManager rulemanager;
    vector<int> v;
    bool result;

    cout<<"START testcase_index_4: ";
    cout<<"delete part of indexs"<<endl;
    rulemanager.getFreePoolIndex(Input,4,v);
    v.erase(v.begin()+1);
    v.erase(v.begin()+1);
    /*v: 0,1,2,3 -> 0,3, index 1,2 left after deletion*/
    result = rulemanager.delPoolIndex(Input, v);
    assert(result == result);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+2) == rulemanager.indexPool[Input][1]);
    cout<<"PASS testcase_index_4"<<endl;
}

void testcase_index_5()
{
    ACLRuleManager rulemanager;
    vector<int> v;
    bool result;

    cout<<"START testcase_index_5: ";
    cout<<"delete more indexs than it have"<<endl;
    rulemanager.getFreePoolIndex(Input,4,v);
    v.push_back(ACL_INPUT_CHAIN_INDEX_MIN+10);
    result = rulemanager.delPoolIndex(Input, v);
    assert(false == result);
    assert((ACL_INPUT_CHAIN_INDEX_MIN) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+2) == rulemanager.indexPool[Input][2]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+3) == rulemanager.indexPool[Input][3]);
    cout<<"PASS testcase_index_5"<<endl;
}



void testcase_index_6()
{
    ACLRuleManager rulemanager;
    vector<int> v,v1;
    bool result;

    cout<<"START testcase_index_6: ";
    cout<<"add, delete and add again in the middle"<<endl;
    result = rulemanager.getFreePoolIndex(Input,7,v);
    v.erase(v.begin());
    v.erase(v.begin());
    v.erase(v.begin()+4);

    /*v: 0,1,2,3,4,5,6 -> 2,3,4,5 index 0,1,6 left after deletion*/
    result = rulemanager.delPoolIndex(Input, v);
    assert(true == result);
    assert((ACL_INPUT_CHAIN_INDEX_MIN) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+6) == rulemanager.indexPool[Input][2]);

    result = rulemanager.getFreePoolIndex(Input,4,v1);
    /*index 0,1,6 -> 0,1,2,3,4,5,6 */
    assert(true == result);
    assert(7 == rulemanager.indexPool[Input].size());
    assert((ACL_INPUT_CHAIN_INDEX_MIN) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+2) == rulemanager.indexPool[Input][2]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+3) == rulemanager.indexPool[Input][3]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+4) == rulemanager.indexPool[Input][4]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+5) == rulemanager.indexPool[Input][5]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+6) == rulemanager.indexPool[Input][6]);
    assert(4 == v1.size());
    assert((ACL_INPUT_CHAIN_INDEX_MIN+2) == v1[0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+3) == v1[1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+4) == v1[2]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+5) == v1[3]);
    cout<<"PASS testcase_index_6"<<endl;
}


void testcase_index_7()
{
    ACLRuleManager rulemanager;
    vector<int> v,v1;
    bool result;

    cout<<"START testcase_index_7: ";
    cout<<"add, delete and add again in the end"<<endl;
    result = rulemanager.getFreePoolIndex(Input,7,v);
    v.erase(v.begin());
    v.erase(v.begin());
    v.erase(v.begin()+4);

    /*v: 0,1,2,3,4,5,6 -> 2,3,4,5 index 0,1,6 left after deletion*/
    result = rulemanager.delPoolIndex(Input, v);
    assert(true == result);
    assert((ACL_INPUT_CHAIN_INDEX_MIN) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+6) == rulemanager.indexPool[Input][2]);

    result = rulemanager.getFreePoolIndex(Input,5,v1);
    /*index 0,1,6 -> 0,1,6,7,8,9,10,11 */
    assert(true == result);
    assert(8 == rulemanager.indexPool[Input].size());
    assert((ACL_INPUT_CHAIN_INDEX_MIN) == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+6) == rulemanager.indexPool[Input][2]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+7) == rulemanager.indexPool[Input][3]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+8) == rulemanager.indexPool[Input][4]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+9) == rulemanager.indexPool[Input][5]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+10) == rulemanager.indexPool[Input][6]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+11) == rulemanager.indexPool[Input][7]);
    assert(5 == v1.size());
    assert((ACL_INPUT_CHAIN_INDEX_MIN+7) == v1[0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+8) == v1[1]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+9) == v1[2]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+10) == v1[3]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+11) == v1[4]);
    cout<<"PASS testcase_index_7"<<endl;
}

void testcase_rule_1()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_1: ";
    cout<<"add two rules"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    result = rulemanager.addACLRule(aclRule,2);
    assert(true == result);
    assert(2 == aclRule.ruleIndexList.size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == aclRule.ruleIndexList[0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == aclRule.ruleIndexList[1]);
    assert(2 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    
    aclRule.dscp = 2;
    result = rulemanager.addACLRule(aclRule,2);
    assert(2 == aclRule.ruleIndexList.size());
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+2) == aclRule.ruleIndexList[0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+3) == aclRule.ruleIndexList[1]);
    assert(4 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+2) == rulemanager.indexPool[1][2]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+3) == rulemanager.indexPool[1][3]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    assert(OutputDscp == rulemanager.ruleList[1].chainType);
    assert(2 == rulemanager.ruleList[1].dscp);
    cout<<"PASS testcase_rule_1"<<endl;
}


void testcase_rule_2()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_2: ";
    cout<<"fail to add rules with invalid chaintype"<<endl;

    aclRule.chainType = endOfAclChainType;
    aclRule.dscp = 1;
    result = rulemanager.addACLRule(aclRule,2);
    assert(false == result);
    assert(true == aclRule.ruleIndexList.empty());
    assert(true == rulemanager.indexPool[0].empty());
    assert(true == rulemanager.indexPool[1].empty());
    assert(true == rulemanager.ruleList.empty());
    
    cout<<"PASS testcase_rule_2"<<endl;
}

void testcase_rule_3()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_3: ";
    cout<<"fail to add rules with invalid index number"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    result = rulemanager.addACLRule(aclRule,ACL_CHAIN_INDEX_RANGE+1);
    assert(false == result);
    assert(true == aclRule.ruleIndexList.empty());
    assert(true == rulemanager.indexPool[0].empty());
    assert(true == rulemanager.indexPool[1].empty());
    assert(true == rulemanager.ruleList.empty());
    
    cout<<"PASS testcase_rule_3"<<endl;
}

void testcase_rule_4()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_4: ";
    cout<<"delete one rule"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,2);
    vector <int>().swap(aclRule.ruleIndexList);
    result = rulemanager.delACLRule(aclRule);
    assert(true == result);
    assert(2 == aclRule.ruleIndexList.size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == aclRule.ruleIndexList[0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == aclRule.ruleIndexList[1]);
    assert(true == rulemanager.indexPool[0].empty());
    assert(true == rulemanager.indexPool[1].empty());
    assert(true == rulemanager.ruleList.empty());
    
    cout<<"PASS testcase_rule_4"<<endl;
}


void testcase_rule_5()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_5: ";
    cout<<"fail to delete one rule with wrong chaintype"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,2);
    aclRule.chainType = Input;
    vector <int>().swap(aclRule.ruleIndexList);
    result = rulemanager.delACLRule(aclRule);
    assert(false == result);
    assert(2 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    
    cout<<"PASS testcase_rule_5"<<endl;
}

void testcase_rule_6()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_6: ";
    cout<<"fail to delete one rule with wrong dscp"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,2);
    aclRule.dscp = 2;
    vector <int>().swap(aclRule.ruleIndexList);
    result = rulemanager.delACLRule(aclRule);
    assert(false == result);
    assert(2 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    
    cout<<"PASS testcase_rule_6"<<endl;
}


void testcase_rule_7()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_7: ";
    cout<<"fail to delete one rule with wrong rule"<<endl;

    aclRule.chainType = Input;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,2);
    aclRule.rule.srcIp = IpAddress::from_string("192.168.255.1");
    vector <int>().swap(aclRule.ruleIndexList);
    result = rulemanager.delACLRule(aclRule);
    assert(false == result);
    assert(2 == rulemanager.indexPool[Input].size());
    assert(ACL_INPUT_CHAIN_INDEX_MIN == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert(Input == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    assert(false == rulemanager.ruleList[0].rule.srcIp.is_initialized());
    
    cout<<"PASS testcase_rule_7"<<endl;
}

void testcase_rule_8()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_8: ";
    cout<<"add three rules, delete one, add one with index in the tail"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,3);
    aclRule.dscp = 2;
    rulemanager.addACLRule(aclRule,1);
    aclRule.dscp = 3;
    rulemanager.addACLRule(aclRule,2);
    /*indexPool: 0,1,2,3,4,5*/
    aclRule.dscp = 2;
    vector <int>().swap(aclRule.ruleIndexList);
    rulemanager.delACLRule(aclRule);
    /*indexPool: 0,1,2,4,5*/
    aclRule.dscp = 4;
    result = rulemanager.addACLRule(aclRule,2);
    /*indexPool: 0,1,2,4,5,6,7*/
    assert(2 == aclRule.ruleIndexList.size());
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+6) == aclRule.ruleIndexList[0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+7) == aclRule.ruleIndexList[1]);
    assert(7 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+2) == rulemanager.indexPool[1][2]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+4) == rulemanager.indexPool[1][3]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+5) == rulemanager.indexPool[1][4]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+6) == rulemanager.indexPool[1][5]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+7) == rulemanager.indexPool[1][6]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    assert(OutputDscp == rulemanager.ruleList[1].chainType);
    assert(3 == rulemanager.ruleList[1].dscp);
    assert(OutputDscp == rulemanager.ruleList[1].chainType);
    assert(4 == rulemanager.ruleList[2].dscp);
    cout<<"PASS testcase_rule_8"<<endl;
}

void testcase_rule_9()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    cout<<"START testcase_rule_9: ";
    cout<<"add three rules, delete one, add one with index in middle"<<endl;

    aclRule.chainType = OutputDscp;
    aclRule.dscp = 1;
    rulemanager.addACLRule(aclRule,3);
    aclRule.dscp = 2;
    rulemanager.addACLRule(aclRule,5);
    aclRule.dscp = 3;
    rulemanager.addACLRule(aclRule,2);
    /*indexPool: 0,1,2,3,4,5,6,7,8,9*/
    aclRule.dscp = 2;
    rulemanager.delACLRule(aclRule);
    /*indexPool: 0,1,2,8,9*/
    aclRule.dscp = 4;
    result = rulemanager.addACLRule(aclRule,3);
    /*indexPool: 0,1,2,3,4,5,8,9*/
    assert(3 == aclRule.ruleIndexList.size());
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+3) == aclRule.ruleIndexList[0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+4) == aclRule.ruleIndexList[1]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+5) == aclRule.ruleIndexList[2]);
    assert(8 == rulemanager.indexPool[1].size());
    assert(ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN == rulemanager.indexPool[1][0]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+1) == rulemanager.indexPool[1][1]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+2) == rulemanager.indexPool[1][2]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+3) == rulemanager.indexPool[1][3]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+4) == rulemanager.indexPool[1][4]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+5) == rulemanager.indexPool[1][5]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+8) == rulemanager.indexPool[1][6]);
    assert((ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN+9) == rulemanager.indexPool[1][7]);
    assert(OutputDscp == rulemanager.ruleList[0].chainType);
    assert(1 == rulemanager.ruleList[0].dscp);
    assert(OutputDscp == rulemanager.ruleList[1].chainType);
    assert(3 == rulemanager.ruleList[1].dscp);
    assert(OutputDscp == rulemanager.ruleList[1].chainType);
    assert(4 == rulemanager.ruleList[2].dscp);
    cout<<"PASS testcase_rule_9"<<endl;
}



void testcase_rule_10()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    cout<<"START testcase_rule_10: ";
    cout<<"succeed to delete one rule with right rule"<<endl;

    aclRule.chainType = Input;
    /*set IP addresses*/
    aclRule.rule.srcIp = IpAddress::from_string("192.168.255.1");
    aclRule.rule.dstIp = IpAddress::from_string("192.168.255.3");
    /*set src port*/
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule.rule.srcPortRanges = portRanges1;
    /*set dst port*/
    portRange.lowValue = 30;
    portRange.highValue = 40;
    portRanges2.push_back(portRange);
    aclRule.rule.dstPortRanges = portRanges2;

    aclRule.rule.transportNetworkType = 3;
    aclRule.rule.precedence = 5;
    aclRule.rule.srcPrefixLength = 24;
    aclRule.rule.dstPrefixLength = 24;
    
    rulemanager.addACLRule(aclRule,2);
    vector <int>().swap(aclRule.ruleIndexList);
    result = rulemanager.delACLRule(aclRule);
    assert(true == result);
    assert(ACL_INPUT_CHAIN_INDEX_MIN == aclRule.ruleIndexList[0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == aclRule.ruleIndexList[1]);
    assert(true == rulemanager.indexPool[0].empty());
    assert(true == rulemanager.indexPool[1].empty());
    assert(true == rulemanager.ruleList.empty());
    
    cout<<"PASS testcase_rule_10"<<endl;
}


void testcase_rule_11()
{
    ACLRuleManager rulemanager;
    ACLRule aclRule;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    cout<<"START testcase_rule_11: ";
    cout<<"fail to delete one rule with different port"<<endl;

    aclRule.chainType = Input;
    /*set IP addresses*/
    aclRule.rule.srcIp = IpAddress::from_string("192.168.255.1");
    aclRule.rule.dstIp = IpAddress::from_string("192.168.255.3");
    /*set src port*/
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule.rule.srcPortRanges = portRanges1;
    /*set dst port*/
    portRange.lowValue = 30;
    portRange.highValue = 40;
    portRanges2.push_back(portRange);
    aclRule.rule.dstPortRanges = portRanges2;

    aclRule.rule.transportNetworkType = 3;
    aclRule.rule.precedence = 5;
    aclRule.rule.srcPrefixLength = 24;
    aclRule.rule.dstPrefixLength = 24;
    
    rulemanager.addACLRule(aclRule,2);
    vector <int>().swap(aclRule.ruleIndexList);
    vector <PortRange>().swap(portRanges2);
    /*modify port range*/
    portRange.lowValue = 50;
    portRange.highValue = 60;
    portRanges2.push_back(portRange);
    aclRule.rule.dstPortRanges = portRanges2;
    result = rulemanager.delACLRule(aclRule);
    assert(false == result);
    assert(2 == rulemanager.indexPool[Input].size());
    assert(ACL_INPUT_CHAIN_INDEX_MIN == rulemanager.indexPool[Input][0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.indexPool[Input][1]);
    assert(Input == rulemanager.ruleList[0].chainType);
    assert(IpAddress::from_string("192.168.255.1") == rulemanager.ruleList[0].rule.srcIp.get());
    assert(IpAddress::from_string("192.168.255.3") == rulemanager.ruleList[0].rule.dstIp.get());
    assert(3 == rulemanager.ruleList[0].rule.transportNetworkType.get());
    assert(5 == rulemanager.ruleList[0].rule.precedence.get());
    assert(24 == rulemanager.ruleList[0].rule.srcPrefixLength.get());
    assert(24 == rulemanager.ruleList[0].rule.dstPrefixLength.get());
    assert(ACL_INPUT_CHAIN_INDEX_MIN == rulemanager.ruleList[0].ruleIndexList[0]);
    assert((ACL_INPUT_CHAIN_INDEX_MIN+1) == rulemanager.ruleList[0].ruleIndexList[1]);
    assert(10 == (rulemanager.ruleList[0].rule.srcPortRanges.get())[0].lowValue);
    assert(20 == (rulemanager.ruleList[0].rule.srcPortRanges.get())[0].highValue);
    assert(30 == (rulemanager.ruleList[0].rule.dstPortRanges.get())[0].lowValue);
    assert(40 == (rulemanager.ruleList[0].rule.dstPortRanges.get())[0].highValue);
    
    cout<<"PASS testcase_rule_11"<<endl;
}



void testcase_compare_1()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_1: ";
    cout<<"different chain type"<<endl;
    
    aclRule1.chainType = OutputDscp;
    aclRule1.dscp = 10;

    aclRule2.chainType = Input;
    aclRule2.dscp = 10;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_1"<<endl;
}


void testcase_compare_2()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_2: ";
    cout<<"OutputDscp identical"<<endl;
    
    aclRule1.chainType = OutputDscp;
    aclRule1.dscp = 10;

    aclRule2.chainType = OutputDscp;
    aclRule2.dscp = 10;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(true == result);
    cout<<"PASS testcase_compare_2"<<endl;
}

void testcase_compare_3()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_3: ";
    cout<<"Input chain with different transportNetworkType"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.transportNetworkType= 10;

    aclRule2.chainType = Input;
    aclRule2.rule.transportNetworkType= 11;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_3"<<endl;
}

void testcase_compare_4()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_4: ";
    cout<<"Input chain with rule is not initialized"<<endl;
    
    aclRule1.chainType = Input;

    aclRule2.chainType = Input;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(true == result);
    cout<<"PASS testcase_compare_4"<<endl;
}


void testcase_compare_5()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_5: ";
    cout<<"Input chain with different IP addresses"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.3");

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_5"<<endl;
}


void testcase_compare_6()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_6: ";
    cout<<"Input chain with one of IP address not initialized"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");

    aclRule2.chainType = Input;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_6"<<endl;
}

void testcase_compare_7()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_7: ";
    cout<<"Input chain with identical IP addresses"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.1");

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(true == result);
    cout<<"PASS UT_casetestcase_compare_7_7"<<endl;
}

void testcase_compare_8()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_8: ";
    cout<<"Input chain with identical IP addresses and port ranges"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule1.rule.srcPortRanges = portRanges1;

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges2.push_back(portRange);
    aclRule2.rule.srcPortRanges = portRanges2;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(true == result);
    cout<<"PASS testcase_compare_8"<<endl;
}

void testcase_compare_9()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_9: ";
    cout<<"Input chain with identical IP addresses and different port ranges"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule1.rule.srcPortRanges = portRanges1;

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 30;
    portRanges2.push_back(portRange);
    aclRule2.rule.srcPortRanges = portRanges2;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_9"<<endl;
}



void testcase_compare_10()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_10: ";
    cout<<"Input chain with one port ranges not initialized"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule1.rule.srcPortRanges = portRanges1;

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.1");

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(false == result);
    cout<<"PASS testcase_compare_10"<<endl;
}




void testcase_compare_11()
{
    ACLRule aclRule1;
    ACLRule aclRule2;
    bool result;
    PortRange portRange;
    PortRanges portRanges1,portRanges2;
    ACLRuleManager ruleManager;

    cout<<"START testcase_compare_11: ";
    cout<<"Input chain with identical src IP addresses and dst port ranges"<<endl;
    
    aclRule1.chainType = Input;
    aclRule1.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges1.push_back(portRange);
    aclRule1.rule.dstPortRanges = portRanges1;

    aclRule2.chainType = Input;
    aclRule2.rule.srcIp = IpAddress::from_string("192.168.255.1");
    portRange.lowValue = 10;
    portRange.highValue = 20;
    portRanges2.push_back(portRange);
    aclRule2.rule.dstPortRanges = portRanges2;

    result = ruleManager.compareACLRule(aclRule1,aclRule2);
    assert(true == result);
    cout<<"PASS testcase_compare_11"<<endl;
}



int main()
{
    testcase_index_1();
    testcase_index_2();
    testcase_index_3();
    testcase_index_4();
    testcase_index_5();
    testcase_index_6();
    testcase_index_7();
    testcase_rule_1();
    testcase_rule_2();
    testcase_rule_3();
    testcase_rule_4();
    testcase_rule_5();
    testcase_rule_6();
    testcase_rule_7();
    testcase_rule_8();
    testcase_rule_9();
    testcase_rule_10();
    testcase_rule_11();
    testcase_compare_1();
    testcase_compare_2();
    testcase_compare_3();
    testcase_compare_4();
    testcase_compare_5();
    testcase_compare_6();
    testcase_compare_7();
    testcase_compare_8();
    testcase_compare_9();
    testcase_compare_10();
    testcase_compare_11();
    cout<<"ALL CASES PASSED"<<endl;
#if 0
    cout<<"main"<<endl;
    cout<<"testing pointer and quote"<<endl;
    TestOuterClass outer;
    outer.setInnerData(100);
    cout<<"(outer.getInner_pointer())->getInnerData="<<(outer.getInner_pointer())->getInnerData()<<endl;
    outer.setInnerData(200);
    cout<<"(outer.getInner_quote()).getInnerData="<<(outer.getInner_quote()).getInnerData()<<endl;

    (outer.getInner_pointer())->setInnerData(333);
    cout<<"outer.getInnerData="<<outer.getInnerData()<<endl;
    (outer.getInner_quote()).setInnerData(444);
    cout<<"outer.getInnerData="<<outer.getInnerData()<<endl;

    cout<<"end of test \n\n\n"<<endl;
    cout<<"testing & *"<<endl;
    int a = 0;
    func1(a);
    cout<<"after calling func1, a is "<<a<<endl;
    func2(a);
    cout<<"after calling func2, a is "<<a<<endl;
    func3(&a);
    cout<<"after calling func3, a is "<<a<<endl;
    cout<<"end of testing & *\n\n\n"<<endl;
    vector<int> v1;
    cout<<"vector size= "<<v1.size()<<" vector capacity= "<<v1.capacity()<<" vector empty= "<<v1.empty()<<endl;
    vector<vector<int> > v2(3);
    v2[0].push_back(1);
    v2[0].push_back(6);
    v2[0].push_back(3);
    v2[0].push_back(5);
    v2[0].push_back(4);
    v2[0].push_back(2);
    printVectorVector(v2);
    sort(v2[0].begin(),v2[0].end(),isless);
    printVectorVector(v2);
    v2[0].erase(v2[0].begin());
    printVectorVector(v2);
    v2[0].erase(v2[0].begin()+2);
    printVectorVector(v2);
    v2[0].erase(v2[0].begin()+3);
    printVectorVector(v2);
    cout<<"\n\n\n\n"<<endl;
    //printVector(v1);
    //test1(v1);
    //printVector(v1);
    cout<<"\n\n\ntest case 1, add element to all 3 chains"<<endl;
    ACLRuleManager rulemanager;
    vector <int>().swap(v1);
    rulemanager.getFreePoolIndex(Input,6,v1);
    //printVector(v1);
    vector <int>().swap(v1);
    rulemanager.getFreePoolIndex(OuputDscp,8,v1);
    vector <int>().swap(v1);
    rulemanager.getFreePoolIndex(OutputOthers,1,v1);
    printVectorVector(rulemanager.indexPool);

    cout<<"\n\n\ntest case 2, add again element to Input chain"<<endl;
    vector <int>().swap(v1);
    rulemanager.getFreePoolIndex(Input,3,v1);
    printVectorVector(rulemanager.indexPool);

    cout<<"\n\n\ntest case 3, del some elements in Input chain"<<endl;
    vector <int>().swap(v1);
    v1.push_back(3008);
    v1.push_back(3006);
    v1.push_back(3007);
    v1.push_back(3005);
    v1.push_back(3002);
    rulemanager.delPoolIndex(Input,v1.size(),v1);
    printVectorVector(rulemanager.indexPool);

    cout<<"\n\n\ntest case 4, add again some elements to Input chain"<<endl;
    vector <int>().swap(v1);
    rulemanager.getFreePoolIndex(Input,1,v1);
    printVectorVector(rulemanager.indexPool);
    cout<<"output allocated index: "<<endl;
    printVector(v1);


    cout<<"\n\n\ntest case 5, add rule to Input chain"<<endl;
    printACLRuleList(rulemanager.ruleList);
    printVectorVector(rulemanager.indexPool);
    ACLRule rule1;
    rule1.chainType = OuputDscp;
    rule1.dscp = 1;
    rule1.rule = 1;
    //rule1.ruleIndexList = v1;
    rulemanager.addACLRule(rule1,2);
    printACLRuleList(rulemanager.ruleList);
    printVectorVector(rulemanager.indexPool);
    cout<<"output rule1.ruleIndexList: "<<endl;
    printVector(rule1.ruleIndexList);
    vector <int>().swap(rule1.ruleIndexList);
    rulemanager.addACLRule(rule1,5);
    printACLRuleList(rulemanager.ruleList);
    printVectorVector(rulemanager.indexPool);
    cout<<"output rule1.ruleIndexList: "<<endl;
    printVector(rule1.ruleIndexList);

    cout<<"\n\n\ntest case 6, del rule to Input chain"<<endl;
    rulemanager.delACLRule(rule1);
    printACLRuleList(rulemanager.ruleList);
    printVectorVector(rulemanager.indexPool);
#endif

}

