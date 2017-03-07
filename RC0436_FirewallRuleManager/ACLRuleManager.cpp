#include <iostream>
#include <vector>
#include <algorithm>
#include "ACLRuleManager.hpp"
#include "CommonFunc.hpp"

using namespace std;





    bool islessOwn(In<int> a, In<int> b)
    {
        return (a < b);
    }
   
   
   /**
     *
     * @brief get index list according to chain type
     *
     * @details return failure if:\n
     *  - input chainType is invalid
     */
    bool ACLRuleManager::getIndexList(In<ACLChainType> chainType, IndexList **indexListPtrPtr)
    {

        if(chainType >= endOfAclChainType)
        {
            //cout<<"ERR|chainType:"<<chainType<<endl;
            return false;
        }
        (*indexListPtrPtr) = &indexPool[static_cast<int>(chainType)];
        return true;
    }
    
    
    /**
      *
      * @brief get index max and min value according to chain type
      *
      * @details return failure if:\n
      *  - input chainType is invalid
      */
    bool ACLRuleManager::getIndexMinMax(In<ACLChainType> chainType, Out<int> indexMin, Out<int> indexMax)
    {
        if(chainType >= endOfAclChainType)
        {
            //cout<<"ERR|chainType:"<<chainType<<endl;
            return false;
        }

        if(OutputDscp == chainType)
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
   
   /**
     *
     * @brief find given index from index list
     *
     * @details return found index list in indexPosListFound, return failure if:\n
     *  - input chainType is invalid
     *  - input chainType is not empty
     * -  input indexListToFind is empty
     * -  input indexNumberToFind is greater than current size
     */
    bool ACLRuleManager::findIndexPositionFromPool(In<ACLChainType> chainType, In<IndexList> indexListToFind, Out<IndexList> indexPosListFound)
    {

        IndexList *indexListPtr;
        IndexList::iterator iter;
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
            sort(indexPosListFound.begin(),indexPosListFound.end(),islessOwn);
            return true;
        }
    }
   
   
    bool ACLRuleManager::delPoolIndex(In<ACLChainType> chainType, In<IndexList> indexListToDel)
    {
        IndexList * indexListPtr;
        IndexList::iterator iter;
        int i,numOfRuleFound =0;
        IndexList indexPosListFound;

        if(false == getIndexList(chainType,&indexListPtr))
        {
            //cout<<"ERR|getIndexList failure:"<<endl;
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
            //cout<<"ERR|findPoolIndex failure:"<<endl;
            return false;
        }
    }
    
    /**
      *
      * @brief find given number of free index
      *
      * @details return index list in indexListToAdd, in case of:\n
      *  - the first addtion, reserve index from head
      *  - gap in current list is large enough, reserve from gap
      *  - gap in current list is not large enough, reserve from tail\n
      */
    bool ACLRuleManager::getFreePoolIndex(In<ACLChainType> chainType, In<int> indexNumberToAdd, Out<IndexList> indexListToAdd)
    {
        IndexList * indexListPtr;
        IndexList::iterator iter;
        int indexMax,indexMin;
        int i,j,lastIndex;
   
        if(false == getIndexMinMax(chainType,indexMin,indexMax))
        {
            return false;
        }
        
        if(false == getIndexList(chainType,&indexListPtr) || 
           (0 == indexNumberToAdd))
        {
            //cout<<"ERR|chainType:"<<chainType<<" indexNumber:"<<indexNumber2Add<<endl;
            return false;
        }

        if(((*indexListPtr).size()+indexNumberToAdd)>(indexMax - indexMin))
        {
            //cout<<"ERR|curSize:"<<(*indexListPtr).size()<<" indexNumber:"<<indexNumber2Add<<" indexMax:"<<indexMax<<" indexMin:"<<indexMin<<endl;
            return false;
        }

        if(false == indexListToAdd.empty())
        {
            /*cleanup output list if it's not empty*/
            indexListToAdd.clear();
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
   
   
   /**
     *
     * @brief add one ACL rule
     *
     * @details add one ACL rules with given index number and index list will be retured in rule.ruleIndexList, addition failure occurs if\n
     *  - remain free index number is smaller than given index number\n
     */
    bool ACLRuleManager::addACLRule(InOut<ACLRule> rule, In<int> indexNumberToAdd)
    {
        if(0 == getFreePoolIndex(rule.chainType,indexNumberToAdd,rule.ruleIndexList))
        {
            //cout<<"ERR|addPoolIndex failure"<<endl;
            return false;
        }
     
        ruleList.push_back(rule);
        return true;
    }

#if 0
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
#endif
    bool ACLRuleManager::compareACLRule(In<ACLRule> aclRule1, In<ACLRule> aclRule2)
    {
        int i;
        Rule rule1,rule2;
        PortRanges portRanges1,portRanges2;
        PortRange portRange1,portRange2;
        
        
        if(aclRule1.chainType != aclRule2.chainType)
        {
            return false;
        }

        rule1 = aclRule1.rule;
        rule2 = aclRule2.rule;
        
        if(OutputDscp == aclRule1.chainType)
        {
            if((rule1.flowType == rule2.flowType)&&
                (aclRule1.dscp == aclRule2.dscp))
            {
                return true;
            }
        }
        else
        {
            if((rule1.flowType == rule2.flowType)&&
                (rule1.transportNetworkType == rule2.transportNetworkType)&&
                (rule1.precedence == rule2.precedence)&&
                (rule1.srcIp == rule2.srcIp)&&
                (rule1.srcPrefixLength == rule2.srcPrefixLength)&&
                (rule1.dstIp == rule2.dstIp)&&
                (rule1.dstPrefixLength == rule2.dstPrefixLength))
            {
                /*check srcPortRanges*/
                if ((rule1.srcPortRanges.is_initialized())^(rule2.srcPortRanges.is_initialized()))
                {
                    /*either one is non-initialized*/
                    return false;
                }
                else if((rule1.srcPortRanges.is_initialized())&(rule2.srcPortRanges.is_initialized()))
                {
                    /*both two are initialized*/
                    portRanges1 = rule1.srcPortRanges.get();
                    portRanges2 = rule2.srcPortRanges.get();
                    /*
                    cout<<"portRanges1[0].lowValue:"<<portRanges1[0].lowValue<<" "
                        <<"portRanges2[0].lowValue:"<<portRanges2[0].lowValue<<" "
                        <<"portRanges1[0].highValue:"<<portRanges1[0].highValue<<" "
                        <<"portRanges2[0].highValue:"<<portRanges2[0].highValue<<endl;*/
                    if((portRanges1[0].lowValue != portRanges2[0].lowValue)||
                        (portRanges1[0].highValue != portRanges2[0].highValue))
                    {
                        return false;
                    }
                }
                /*else both two are non-initialized, check pass*/


                /*check dstPortRanges*/
                if ((rule1.dstPortRanges.is_initialized())^(rule2.dstPortRanges.is_initialized()))
                {
                    /*either one is non-initialized*/
                    return false;
                }
                else if((rule1.dstPortRanges.is_initialized())&(rule2.dstPortRanges.is_initialized()))
                {
                    /*both two are initialized*/
                    portRanges1 = rule1.dstPortRanges.get();
                    portRanges2 = rule2.dstPortRanges.get();
                    if((portRanges1[0].lowValue != portRanges2[0].lowValue)||
                        (portRanges1[0].highValue != portRanges2[0].highValue))
                    {
                        return false;
                    }
                }
                /*else both two are non-initialized, check pass*/
                //cout<<"DBG input chain match"<<endl;
                /*cout<<"rule1.srcPortRanges.is_initialized(): "<<rule1.srcPortRanges.is_initialized()<<" "
                    <<"rule2.srcPortRanges.is_initialized(): "<<rule2.srcPortRanges.is_initialized()<<" "
                    <<endl;*/
                return true;
            }
        }
        return false;
    }
   
   /**
     *
     * @brief find rule position in rule database by given rule
     *
     */
    bool ACLRuleManager::getACLRulePositionByRule(InOut<ACLRule> rule, Out<int> pos)
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
   
   /**
     *
     * @brief delete one ACL rule
     *
     * @details delete one ACL rule and index list will be retured in rule.ruleIndexList, deletion failure occurs if\n
     *  - given rule is not found in rule database\n
     *  - index list along with rule is not found in index database\n
     */
    bool ACLRuleManager::delACLRule(InOut<ACLRule> rule)
    {
        std::vector<ACLRule>::iterator iter;
        int numOfRuleFound = 0;
        IndexList indexPosListFound;
        int rulePos;

        if(false == rule.ruleIndexList.empty())
        {
            /*cleanup output list if it's not empty*/
            rule.ruleIndexList.clear();
        }

        if(false == getACLRulePositionByRule(rule, rulePos))
        {
            //cout<<"ERR|findACLRule failure"<<endl;
            return false;
        }

        /*rule.ruleIndexList is asigned in getACLRulePositionByRule()*/
        if(false == findIndexPositionFromPool(rule.chainType, rule.ruleIndexList, indexPosListFound))
        {
            //cout<<"ERR|findPoolIndex failure"<<endl;
            return false;
        }
        /*delete index*/
        delPoolIndex(rule.chainType, rule.ruleIndexList);
        /*delete rule*/
        ruleList.erase(ruleList.begin()+rulePos);
        //cout<<"end of delACLRule"<<endl;
        return true;
    }


    bool ACLRuleManager::getRuleListByFlowtype(In<FlowType> flowType, Out<std::vector<ACLRule> > foundRuleList)
    {
        std::vector<ACLRule>::iterator iter;

        if(false == foundRuleList.empty())
        {
            /*cleanup output list if it's not empty*/
            foundRuleList.clear();
        }
     
        for(iter=ruleList.begin(); iter!=ruleList.end();iter++)
        {
            if((*iter).rule.flowType == flowType)
            {
                foundRuleList.push_back(*iter);
            }
        }
        
        return true;
    }


