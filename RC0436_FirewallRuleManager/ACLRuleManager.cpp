#include <iostream>
#include <vector>
#include <algorithm>
#include "ACLRuleManager.hpp"
#include "CommonFunc.hpp"

using namespace std;





    bool isless(const int & a, const int & b)
    {
        return (a < b);
    }
   


    bool ACLRuleManager::getIndexList(const ACLChainType & chainType, vector<int> **indexListPtrPtr)
    {

        if(chainType >= endOfAclChainType)
        {
            //cout<<"ERR|chainType:"<<chainType<<endl;
            return false;
        }
        (*indexListPtrPtr) = &indexPool[static_cast<int>(chainType)];
        return true;
    }

    bool ACLRuleManager::getIndexMinMax(const ACLChainType & chainType, int & indexMin, int & indexMax)
    {
        if(chainType >= endOfAclChainType)
        {
            //cout<<"ERR|chainType:"<<chainType<<endl;
            return false;
        }

        if(OuputDscp == chainType)
        {
            indexMax = ACL_OUTPUT_CHAIN_DSCP_INDEX_MAX;
            indexMin = ACL_OUTPUT_CHAIN_DSCP_INDEX_MIN;
        }
        /*else if(OutputOthers == chainType)
        {
            indexMax = ACL_OUTPUT_CHAIN_OTHER_INDEX_MAX;
            indexMin = ACL_OUTPUT_CHAIN_OTHER_INDEX_MIN;
        }*/
        else
        {
            indexMax = ACL_INPUT_CHAIN_INDEX_MAX;
            indexMin = ACL_INPUT_CHAIN_INDEX_MIN;
        }

        return true;
    }
   
   
    bool ACLRuleManager::findIndexPositionFromPool(const ACLChainType & chainType, const vector<int> & indexListToFind, vector<int> & indexPosListFound)
    {

        vector<int> *indexListPtr;
        vector<int>::iterator iter;
        int indexMax,indexMin;
        int i,pos;
        int indexNumberToFind = indexListToFind.size();

        if(false == getIndexMinMax(chainType,indexMin,indexMax))
        {
            return false;
        }
        
        if(false == getIndexList(chainType,&indexListPtr) || 
            (0 == indexNumberToFind) || 
            (true == indexListToFind.empty()) ||
            (false == indexPosListFound.empty()))
        {
            //cout<<"ERR|chainType:"<<chainType<<" indexNumber2Find:"<<indexNumber2Find<<" indexList2Find empty:"<<indexList2Find.empty()<<" indexPosListFound empty:"<<indexPosListFound.empty()<<endl;
            return false;
        }

        if(indexNumberToFind > (*indexListPtr).size())
        {
            //cout<<"ERR|indexNumber:"<<indexNumber2Find<<" size:"<<(*indexListPtr).size()<<endl;
            return false;
        }


        for(i=0; i<indexNumberToFind; i++)
        {
            for(iter=(*indexListPtr).begin(),pos=0; iter!=(*indexListPtr).end(); iter++,pos++)
            {
                if((*iter)==indexListToFind[i])
                {
                    indexPosListFound.push_back(pos);
                    //cout<<"DBG|found pos:"<<pos<<" (*iter):"<<(*iter)<<endl;
                }
            }

        }

        if(indexPosListFound.size() != indexNumberToFind)
        {
            //cout<<"ERR|ruleFoundNum:"<<indexPosListFound.size()<<" indexNumber2Find:"<<indexNumber2Find<<endl;
            return false;
        }
        else
        {
            /*ascending sorted for further deletion*/
            sort(indexPosListFound.begin(),indexPosListFound.end(),isless);
            return true;
        }
    }
   
   
    bool ACLRuleManager::delPoolIndex(const ACLChainType & chainType, const vector<int> & indexListToDel)
    {
        vector<int> * indexListPtr;
        vector<int>::iterator iter;
        int i,numOfRuleFound =0;
        vector<int> indexPosListFound;

        if(false == getIndexList(chainType,&indexListPtr))
        {
            cout<<"ERR|getIndexList failure:"<<endl;
            return false;
        }

        if(true == findIndexPositionFromPool(chainType, indexListToDel, indexPosListFound))
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
            cout<<"ERR|delPoolIndex failure:"<<endl;
            return false;
        }
    }
   
   
    bool ACLRuleManager::getFreePoolIndex(const ACLChainType & chainType, const int & indexNumberToAdd, vector<int> & indexListToAdd)
    {
        vector<int> * indexListPtr;
        vector<int>::iterator iter;
        int indexMax,indexMin;
        int i,j,lastIndex;
   
        if(false == getIndexMinMax(chainType,indexMin,indexMax))
        {
            return false;
        }
        
        if(false == getIndexList(chainType,&indexListPtr) || 
           (0 == indexNumberToAdd))
        {
            cout<<"ERR|chainType:"<<chainType<<" indexNumberToAdd:"<<indexNumberToAdd<<endl;
            return false;
        }

        if(((*indexListPtr).size()+indexNumberToAdd)>(indexMax - indexMin))
        {
            cout<<"ERR|curSize:"<<(*indexListPtr).size()<<" indexNumberToAdd:"<<indexNumberToAdd<<" indexMax:"<<indexMax<<" indexMin:"<<indexMin<<endl;
            return false;
        }

        if(false == indexListToAdd.empty())
        {
            /*cleanup output list if it's not empty*/
            std::vector <int>().swap(indexListToAdd);
        }
        
        if((*indexListPtr).empty())
        {
            for(i=0; i<indexNumberToAdd; i++)
            {
             (*indexListPtr).push_back(indexMin+i);
             indexListToAdd.push_back(indexMin+i);
            }
            return true;
        }
        else
        {
            /*check if there is enough gap to meet requirement*/
            for(i=0; i<((*indexListPtr).size()-1); i++)
            {     
             if(((*indexListPtr)[i+1] - (*indexListPtr)[i]) > indexNumberToAdd)
             {
                 /*found available position in the middle of list*/
                 for(int j=1; j<(indexNumberToAdd+1); j++)
                 {
                  (*indexListPtr).insert((*indexListPtr).begin()+i+j, (*indexListPtr)[i]+j);
                  indexListToAdd.push_back((*indexListPtr)[i]+j);
                 }
                 return true;
             }
            }

            if((indexMax-(*((*indexListPtr).rbegin())))<indexNumberToAdd)
            {
                //cout<<"ERR|(*((*indexListPtr).rbegin())):"<<(*((*indexListPtr).rbegin()))<<" indexMax:"<<indexMax<<" indexNumberToAdd:"<<indexNumberToAdd<<endl;
                return false;
            }
   
            /*found available position in the end of list*/
            for(i=1; i<(indexNumberToAdd+1); i++)
            {
             lastIndex = (*indexListPtr).back();
             //cout<<"end:"<<((*currentIndexListPtr).back())<<endl;
             (*indexListPtr).push_back(lastIndex+1);
             indexListToAdd.push_back(lastIndex+1);
            }
            
            return true;
        }
    }
   
   
   
   
    bool ACLRuleManager::addACLRule(ACLRule & rule, const int & indexNumberToAdd)
    {
        if(0 == getFreePoolIndex(rule.chainType,indexNumberToAdd,rule.ruleIndexList))
        {
            //cout<<"ERR|addPoolIndex failure"<<endl;
            return false;
        }
     
        ruleList.push_back(rule);
        return true;
    }

    bool ACLRuleManager::compareACLRule(const ACLRule & aclRule1, const ACLRule & aclRule2)
    {
        int i;
        if((aclRule1.chainType == aclRule2.chainType)&&
            (aclRule1.rule == aclRule2.rule)&&
            (aclRule1.dscp == aclRule2.dscp))
        {
            return true;
        }
        return false;
    }

   
   
    bool ACLRuleManager::getACLRulePositionByRule(ACLRule & rule, int & pos)
    {
        std::vector<ACLRule>::iterator iter;
        int i;
     
        for(iter=ruleList.begin(),i=0; iter!=ruleList.end();iter++,i++)
        {
            if(compareACLRule((*iter),rule))
            {
                pos=i;
                rule.ruleIndexList = (*iter).ruleIndexList;
                return true;
            }
        }
        return false;
    }
   
   
    bool ACLRuleManager::delACLRule(ACLRule & rule)
    {
        std::vector<ACLRule>::iterator iter;
        int numOfRuleFound = 0;
        vector<int> indexPosListFound;
        int rulePos;

        if(false == getACLRulePositionByRule(rule, rulePos))
        {
            cout<<"ERR|getACLRulePositionByRule failure"<<endl;
            return false;
        }

        /*rule.ruleIndexList is asigned in getACLRulePositionByRule()*/
        if(false == findIndexPositionFromPool(rule.chainType, rule.ruleIndexList, indexPosListFound))
        {
            cout<<"ERR|findIndexPositionFromPool failure"<<endl;
            return false;
        }
        /*delete index*/
        delPoolIndex(rule.chainType, rule.ruleIndexList);
        /*delete rule*/
        ruleList.erase(ruleList.begin()+rulePos);
        //cout<<"end of delACLRule"<<endl;
        return true;
    }



