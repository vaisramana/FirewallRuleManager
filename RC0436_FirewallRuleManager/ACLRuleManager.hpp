#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define ACL_CHAIN_INDEX_MAX (30000)
#define ACL_CHAIN_INDEX_MIN (3000)
#define ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX (6000)
#define ACL_OUTPUT_CHAIN_OTHERS_INDEX_MIN (ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX+1)


typedef enum{
   Input,
   OuputDscp,
   OutputOthers,
   /*new type should be added above this line*/
   endOfAclChainType
}ACLChainType;

struct ACLRule
{
    ACLChainType chainType;
    int rule;
	int dscp;
	vector<int> ruleIndexList; 
};

void printVector(vector<int> v);
void printVectorVector(vector<vector<int> > v);
void printACLRuleList(vector<ACLRule>  ruleList);
bool compare(int a, int b);



class ACLRuleManager
{
public:
	ACLRuleManager():indexList(3){}
	bool ACLRuleManager::addPoolIndex(ACLChainType chainType, int indexNumber2Add, vector<int> &indexList2Add);
	bool delPoolIndex(ACLChainType chainType, int indexNumber2Del, vector<int> indexList2Del);
    bool findPoolIndex(ACLChainType chainType, int indexNumber2Find, vector<int> indexList2Find, vector<int> &indexPosListFound);
    bool addACLRule(ACLRule &rule, int indexNumber2Add);
    bool delACLRule(ACLRule rule);
    bool findACLRule(ACLRule rule, int &pos);
    bool compareACLRule(ACLRule rule1, ACLRule rule2);

    bool getIndexList(ACLChainType chainType, vector<int> **indexListPtrPtr);
    

	vector<ACLRule> ruleList;
	vector<vector<int> > indexList; /* 3 index list corresponding to input, output dscp and output others */
};

