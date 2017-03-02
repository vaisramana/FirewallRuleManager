#include <iostream>
#include <vector>
#include <algorithm>
#include "ACLRuleManager.hpp"
using namespace std;



bool compare(int a, int b)
{
	return (a < b);
}



bool ACLRuleManager::getIndexList(ACLChainType chainType, vector<int> **indexListPtrPtr)
{
	cout<<"getIndexList"<<endl;

	if(chainType >= endOfAclChainType)
	{
		cout<<"ERR|chainType:"<<chainType<<endl;
		return false;
	}
	(*indexListPtrPtr) = &indexList[static_cast<int>(chainType)];
	//cout<<"DBG|size:"<<(*(*indexListPtrPtr)).size()<<" (*indexListPtrPtr):"<<(*indexListPtrPtr)<<endl;
	return true;
}


bool ACLRuleManager::findPoolIndex(ACLChainType chainType, int indexNumber2Find, vector<int> indexList2Find, vector<int> &indexPosListFound)
{
	cout<<"findPoolIndex"<<endl;
	
	vector<int> *indexListPtr;
	vector<int>::iterator iter;
	int indexMax = ACL_CHAIN_INDEX_MAX;
	int indexMin = ACL_CHAIN_INDEX_MIN;
	int i,pos;

	//cout<<"indexList2Find.size(): "<<indexList2Find.size()<<" indexList2Find.empty():"<<indexList2Find.empty()<<endl;
	//printVector(indexList2Find);
	if(OuputDscp == chainType)
	{
		indexMax = ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX;
	}
	else if(OutputOthers == chainType)
	{
		indexMin = ACL_OUTPUT_CHAIN_OTHERS_INDEX_MIN;
	}

	
	if(!getIndexList(chainType,&indexListPtr) || 
	   (0 == indexNumber2Find) || 
	   (true == indexList2Find.empty()) ||
	   (false == indexPosListFound.empty()))
	{
		cout<<"ERR|chainType:"<<chainType<<" indexNumber2Find:"<<indexNumber2Find<<" indexList2Find empty:"<<indexList2Find.empty()<<" indexPosListFound empty:"<<indexPosListFound.empty()<<endl;
		return false;
	}

	//cout<<"DBG|size:"<<(*indexListPtr).size()<<" indexListPtr:"<<&(*indexListPtr)<<endl;
	
	if(indexNumber2Find > (*indexListPtr).size())
	{
		//cout<<"ERR|indexNumber:"<<indexNumber2Find<<" size:"<<(*indexListPtr).size()<<endl;
		return false;
	}

	
	for(i=0; i<indexNumber2Find; i++)
	{
		//cout<<"DBG|to find  i:"<<i<<" size:"<<indexList.size()<<endl;
		for(iter=(*indexListPtr).begin(),pos=0; iter!=(*indexListPtr).end(); iter++,pos++)
		{
			//cout<<"DBG|fo through  pos:"<<pos<<endl;
			if((*iter)==indexList2Find[i])
			{
				indexPosListFound.push_back(pos);
				//cout<<"DBG|found pos:"<<pos<<" (*iter):"<<(*iter)<<endl;
				//cout<<"found "<<indexList2Del[i]<<endl;
			}
		}
	
	}

	if(indexPosListFound.size() != indexNumber2Find)
	{
		cout<<"ERR|ruleFoundNum:"<<indexPosListFound.size()<<" indexNumber2Find:"<<indexNumber2Find<<endl;
		return false;
	}
	else
	{
		/*ascending sorted for further deletion*/
		sort(indexPosListFound.begin(),indexPosListFound.end(),compare);
		return true;
	}

	
}


bool ACLRuleManager::delPoolIndex(ACLChainType chainType, int indexNumber2Del, vector<int> indexList2Del)
{
	cout<<"delPoolIndex"<<endl;

	vector<int> * indexListPtr;
	vector<int>::iterator iter;
	int indexMax = ACL_CHAIN_INDEX_MAX;
	int indexMin = ACL_CHAIN_INDEX_MIN;
	int i,numOfRuleFound =0;
	vector<int> indexPosListFound;

	if(false == getIndexList(chainType,&indexListPtr))
	{
		cout<<"ERR|getIndexList failure:"<<endl;
		return false;
	}
	
	//printVector(indexList2Del);

	if(true == findPoolIndex(chainType, indexNumber2Del, indexList2Del, indexPosListFound))
	{
		for(i=0; i<indexPosListFound.size(); i++)
		{
			/*indexPosListFound should be ascending sorted*/
			//cout<<"DBG|i:"<<i<<" indexPosListFound[i]:"<<indexPosListFound[i]<<endl;
			(*indexListPtr).erase((*indexListPtr).begin()+indexPosListFound[i]-i);
		}
		return true;
	}
	else
	{
		cout<<"ERR|findPoolIndex failure:"<<endl;
		return false;
	}

	/*
	for(i=0; i<indexNumber2Del; i++)
	{
		for(iter=(*currentIndexListPtr).begin(); iter!=(*currentIndexListPtr).end();)
		{
			if((*iter)==indexList2Del[i])
			{
				(*currentIndexListPtr).erase(iter);
			}
			else
			{
				iter++;
			}
		}
	
	}
	*/
	//cout<<"end of delPoolIndex"<<endl;
}


bool ACLRuleManager::addPoolIndex(ACLChainType chainType, int indexNumber2Add, vector<int> &indexList2Add)
{
    cout<<"addPoolIndex"<<endl;

	vector<int> * indexListPtr;
	vector<int>::iterator iter;
	int indexMax = ACL_CHAIN_INDEX_MAX;
	int indexMin = ACL_CHAIN_INDEX_MIN;
	int i,j,lastIndex;

	if(OuputDscp == chainType)
	{
		indexMax = ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX;
	}
	else if(OutputOthers == chainType)
	{
		indexMin = ACL_OUTPUT_CHAIN_OTHERS_INDEX_MIN;
	}

	if(!getIndexList(chainType,&indexListPtr) || 
	   (0 == indexNumber2Add) ||
	   (false == indexList2Add.empty()))
	{
		cout<<"ERR|chainType:"<<chainType<<" indexNumber:"<<indexNumber2Add<<" empty:"<<indexList2Add.empty()<<endl;
		return false;
	}

	//cout<<"DBG|size:"<<(*indexListPtr).size()<<" indexListPtr:"<<&(*indexListPtr)<<endl;

	//currentIndexListPtr= &indexList[static_cast<int>(chainType)];
	//currentIter = indexList[static_cast<int>(chainType)].begin;

	if(((*indexListPtr).size()+indexNumber2Add)>(indexMax - indexMin))
	{
		cout<<"ERR|curSize:"<<(*indexListPtr).size()<<" indexNumber:"<<indexNumber2Add<<" indexMax:"<<indexMax<<" indexMin:"<<indexMin<<endl;
		return false;
	}
	
	if((*indexListPtr).empty())
	{
		for(i=0; i<indexNumber2Add; i++)
		{
			(*indexListPtr).push_back(indexMin+i);
			indexList2Add.push_back(indexMin+i);
		}
		//printVector((*indexList2AddPtr));
		return true;
	}
	else
	{
		/*check if there is enough gap to meet requirement*/
		for(i=0; i<((*indexListPtr).size()-1); i++)
		{		
			if(((*indexListPtr)[i+1] - (*indexListPtr)[i]) > indexNumber2Add)
			{
				/*found available position in the middle of list*/
				for(int j=1; j<(indexNumber2Add+1); j++)
				{
					(*indexListPtr).insert((*indexListPtr).begin()+i+j, (*indexListPtr)[i]+j);
					indexList2Add.push_back((*indexListPtr)[i]+j);
				}
				return true;
			}
		}

		//printVector((*currentIndexListPtr));
		/*found available position in the end of list*/
		for(i=1; i<(indexNumber2Add+1); i++)
		{
			lastIndex = (*indexListPtr).back();
			//cout<<"end:"<<((*currentIndexListPtr).back())<<endl;
			(*indexListPtr).push_back(lastIndex+1);
			indexList2Add.push_back(lastIndex+1);
		}
		
		return true;
	}
	//printVector((*indexList2AddPtr));
	//printVectorVector(indexList);
	//cout<<"end of addPoolIndex"<<endl;
	
	
}




bool ACLRuleManager::addACLRule(ACLRule &rule, int indexNumber2Add)
{
	cout<<"addACLRule"<<endl;

	if(0 == addPoolIndex(rule.chainType,indexNumber2Add,rule.ruleIndexList))
	{
		cout<<"ERR|addPoolIndex failure"<<endl;
		return false;
	}
	
	ruleList.push_back(rule);
	return true;
	
	//cout<<"end of setACLRule"<<endl;
}



bool ACLRuleManager::compareACLRule(ACLRule rule1, ACLRule rule2)
{
	cout<<"compareACLRule"<<endl;
	int i;
	
	if((rule1.chainType == rule2.chainType)&&
		(rule1.rule == rule2.rule)&&
		(rule1.dscp == rule2.dscp)&&
		(rule1.ruleIndexList.size() == rule2.ruleIndexList.size()))
	{
		for(i=0; i<rule1.ruleIndexList.size(); i++)
		{
			if(rule1.ruleIndexList[i]!=rule2.ruleIndexList[i])
			{
				/*not same*/
				return false;
			}
			
		}
		return true;
	}
	return false;
}


bool ACLRuleManager::findACLRule(ACLRule rule, int &pos)
{
	vector<ACLRule>::iterator iter;
	int i;
	
	for(iter=ruleList.begin(),i=0; iter!=ruleList.end();iter++,i++)
	{
		if(compareACLRule((*iter),rule))
		{
			pos=i;
			return true;
		}
	}
	
	return false;
}


bool ACLRuleManager::delACLRule(ACLRule rule)
{
	vector<ACLRule>::iterator iter;
	int numOfRuleFound = 0;
	vector<int> indexPosListFound;
	int rulePos;
	
	//cout<<"delACLRule"<<endl;
	
	//cout<<"indexPosListFound.size(): "<<indexPosListFound.size()<<" indexPosListFound.empty():"<<indexPosListFound.empty()<<endl;
	//printVector(rule.ruleIndexList);
	if(0 == findPoolIndex(rule.chainType, rule.ruleIndexList.size(), rule.ruleIndexList, indexPosListFound))
	{
		cout<<"ERR|findPoolIndex failure"<<endl;
		return false;
	}
	if(0 == findACLRule(rule, rulePos))
	{
		cout<<"ERR|findACLRule failure"<<endl;
		return false;
	}

	/*delete index*/
	delPoolIndex(rule.chainType, rule.ruleIndexList.size(), rule.ruleIndexList);
	/*delete rule*/
	ruleList.erase(ruleList.begin()+rulePos);
	//cout<<"end of delACLRule"<<endl;
	return true;
}



