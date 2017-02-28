#include <iostream>
#include <vector>
#include <algorithm>
#include "ACLRuleManager.hpp"

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
    printVector(v);
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




int main()
{
    cout<<"main"<<endl;
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
	sort(v2[0].begin(),v2[0].end(),compare);
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
	rulemanager.addPoolIndex(Input,6,v1);
	//printVector(v1);
	vector <int>().swap(v1);
	rulemanager.addPoolIndex(OuputDscp,8,v1);
	vector <int>().swap(v1);
	rulemanager.addPoolIndex(OutputOthers,1,v1);
	printVectorVector(rulemanager.indexList);

	cout<<"\n\n\ntest case 2, add again element to Input chain"<<endl;
	vector <int>().swap(v1);
	rulemanager.addPoolIndex(Input,3,v1);
	printVectorVector(rulemanager.indexList);

	cout<<"\n\n\ntest case 3, del some elements in Input chain"<<endl;
	vector <int>().swap(v1);
	v1.push_back(3008);
	v1.push_back(3006);
	v1.push_back(3007);
	v1.push_back(3005);
	v1.push_back(3002);
	rulemanager.delPoolIndex(Input,v1.size(),v1);
	printVectorVector(rulemanager.indexList);

	cout<<"\n\n\ntest case 4, add again some elements to Input chain"<<endl;
	vector <int>().swap(v1);
	rulemanager.addPoolIndex(Input,1,v1);
	printVectorVector(rulemanager.indexList);
	cout<<"output allocated index: "<<endl;
	printVector(v1);

	
	cout<<"\n\n\ntest case 5, add rule to Input chain"<<endl;
	printACLRuleList(rulemanager.ruleList);
	printVectorVector(rulemanager.indexList);
	ACLRule rule1;
	rule1.chainType = OuputDscp;
	rule1.dscp = 1;
	rule1.rule = 1;
	//rule1.ruleIndexList = v1;
	rulemanager.addACLRule(rule1,2);
	printACLRuleList(rulemanager.ruleList);
	printVectorVector(rulemanager.indexList);
	cout<<"output rule1.ruleIndexList: "<<endl;
	printVector(rule1.ruleIndexList);
	vector <int>().swap(rule1.ruleIndexList);
	rulemanager.addACLRule(rule1,5);
	printACLRuleList(rulemanager.ruleList);
	printVectorVector(rulemanager.indexList);
	cout<<"output rule1.ruleIndexList: "<<endl;
	printVector(rule1.ruleIndexList);

	cout<<"\n\n\ntest case 6, del rule to Input chain"<<endl;
	rulemanager.delACLRule(rule1);
	printACLRuleList(rulemanager.ruleList);
	printVectorVector(rulemanager.indexList);
	
}

