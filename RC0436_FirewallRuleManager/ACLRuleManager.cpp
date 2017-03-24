#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
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
        //here specific Qos rule compare(Qos: dscp + rule)
        if( (OutputDscp == aclRule1.chainType)&&(aclRule1.dscp != aclRule2.dscp) )
        {
            return false ;
        }
        
        rule1 = aclRule1.rule;
        rule2 = aclRule2.rule;
        
        //common process to compare the collumns of rule
        //(Filter rule just include rule , follow common process )
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

    bool ACLRuleManager::convertIpAddress(In<string> inputAddress, Out<string> outputAddress)
     {
         vector<string> numbers;
         vector<string> numbersModified;
         int i,mask;
         unsigned int ip=0;
 
         cout<<"convertIpAddress() input: "<<inputAddress<<endl;
         boost::split(numbers, inputAddress, boost::is_any_of("./"), boost::token_compress_on);
         if(5 != numbers.size())
         {
             cout<<"WRN/ convertIpAddress() with wrong input"<<endl;
             return false;
         }
 
         mask = boost::lexical_cast<int>(numbers[4]);
         for(i=0; i<(numbers.size()-1); i++)
         {
             ip = ip|((boost::lexical_cast<int>(numbers[i]))<<(24-8*i));
             //cout<<"ip = "<<ip<<" numbers[i] = "<<(boost::lexical_cast<int>(numbers[i]))<<endl;
         }
         ip = (ip>>(32-mask))<<(32-mask);
         //cout<<"int ip = "<<ip<<endl;
 
         for(i=0; i<4; i++)
         {
             numbersModified.push_back(boost::lexical_cast<string>((ip<<i*8)>>24));
         }
         outputAddress = boost::join(numbersModified, ".");
         outputAddress = outputAddress + "/" + (boost::lexical_cast<string>(numbers[4]));
         cout<<"convertIpAddress() outputAddress = "<<outputAddress<<endl;
         return true;
     }

     bool ACLRuleManager::getIpAndIfMap(Out<std::map<string, string> >ipMap, Out<std::map<string, string> >ifMap)
     {
         //string ipInfo = "ip-info=management,fa:16:3e:9f:56:bc,192.168.1.3/24,|internal,fa:16:3e:73:f3:2d,169.254.0.20/19,|intmsg0,fa:16:3e:d1:d0:fb,169.254.64.2/24,|oam,fa:16:3e:79:bf:15,10.56.66.77/24,";
         string ipInfo = "ip-info=management,fa:16:3e:8d:a8:f5,192.168.1.7/24,|internal,fa:16:3e:a5:79:e8,169.254.0.23/19,|intmsg0,fa:16:3e:c6:13:c9,169.254.64.5/24,|ext0,fa:16:3e:1a:08:45,100.1.7.9/24,|ext1,fa:16:3e:15:43:47,100.1.8.6/24,|ext2,fa:16:3e:a2:85:07,100.1.9.6/24,";
         //string ifInfo = "interface-info=management,internal,1500,9000|internal,internal,1500,9000|intmsg0,internal,1500,9000|oam,oam,1500,9000";
         string ifInfo = "interface-info=management,internal,1500,9000|internal,internal,1500,9000|intmsg0,internal,1500,9000|ext0,external,1500,9000|ext1,external,1500,9000|ext2,external,1500,9000";
         vector<string> lines;
         vector<string>::iterator iterString;
         string replacedString;
         //string ipInfo,ifInfo; 
         std::map<string, string>::iterator iterMap;
         //ErrorCode ret;
         string ns = "MEI-0";
         int i;
         std::string cmd;
         std::vector<string> cmdList; 


         /*fetch IP and IF info*/
         /*
        cmd = "ssh " + ns + " \"getinitarg ip-info\"";
        ret = tpl::exec_fscli_directly(cmd, "Firewall", pErrCategory.get());
        if (ret.value() != static_cast<int>(tpi::ErrorValue::Success))
        {
              return ret;
        }
        ipInfo = pErrCategory.get()->message(0);

        cmd = "ssh " + ns + " \"getinitarg interface-info\"";
        ret = tpl::exec_fscli_directly(cmd, "Firewall", pErrCategory.get());
        if (ret.value() != static_cast<int>(tpi::ErrorValue::Success))
        {
              return ret;
        }
        ifInfo = pErrCategory.get()->message(0);
         */
 
         
         /*parse IP info*/
         boost::split(lines, ipInfo, boost::is_any_of(",=|"), boost::token_compress_on);
         for(i=0,iterString=lines.begin(); iterString!=lines.end(); iterString++,i++)
         {
             if(boost::contains(*iterString, "/"))
             {
                 if(false == convertIpAddress(*iterString,replacedString))
                 {
                     return false;
                 }
                 *iterString = replacedString;
             }
             cout<<lines[i]<<endl;
             if(i)
             {
                 if((i-1)%3==2)
                 {
                     ipMap[*(iterString-2)]=(*iterString);
                     // cout<<"map "<<*(iterString-2)<<" to "<<(*iterString)<<endl;
                 }
             }
         }
         /*print ipMap*/
         cout<<"ipMap with size "<<ipMap.size()<<":"<<endl;
         for(iterMap=ipMap.begin(); iterMap!=ipMap.end(); iterMap++)
         {
             cout<<iterMap->first <<"->"<<iterMap->second<<" ";
         }
         cout<<endl;
 
         /*parse IF info*/
         boost::split(lines, ifInfo, boost::is_any_of(",=|"), boost::token_compress_on);
         for(i=0,iterString=lines.begin(); iterString!=lines.end(); iterString++,i++)
         {
             
             if(i)
             {
                 if((i-1)%4==1)
                 {
                     ifMap[*(iterString-1)]=(*iterString);
                 }
             }
         }
 
         /*print ifMap*/
         cout<<"ifMap with size "<<ifMap.size()<<":"<<endl;
         for(iterMap=ifMap.begin(); iterMap!=ifMap.end(); iterMap++)
         {
             cout<<iterMap->first <<"->"<<iterMap->second<<" ";
         }
         cout<<endl;
         return true;
     }

     bool ACLRuleManager::addDefaultRules()
    {
        std::string cmd;
        std::vector<string> cmdList; 
        bool ret;
        std::vector<std::string> chainTypeList;
        std::vector<std::string> icmpTypeList;
        std::vector<std::string> addrTypeList;
        std::vector<std::string> ifTypeList;
        int i,j;
        int index = ACL_DEFAULT_BOTTOM_INDEX_MIN;
        int spoofIndex = ACL_DEFAULT_TOP_INDEX_MIN;
        string ns = "MEI-0";
        //std::string ipInfo,ifInfo;
        std::map<string, string> ipMap;
        std::map<string, string> ifMap;
        std::map<string, string>::iterator iterIpMap;
        std::map<string, string>::iterator iterIfMap,iterIfMapTmp;

        chainTypeList.push_back("input");
        chainTypeList.push_back("output");
        icmpTypeList.push_back("0");
        icmpTypeList.push_back("3");
        icmpTypeList.push_back("8");
        icmpTypeList.push_back("11");
        icmpTypeList.push_back("12");
        addrTypeList.push_back("srcaddr");
        addrTypeList.push_back("dstaddr");
        ifTypeList.push_back("input-iface");
        ifTypeList.push_back("output-iface");
        
        /*Disable iptables in fastpath, to improve the fastpath throughput*/
        cmd.clear();
        cmd += "ssh ";
        cmd += ns;
        cmd += " \"fpdebug nf-switch all_tables all_hooks off\"";
        /*
        ret = tpl::exec_fscli_directly(cmd, "Firewall", pErrCategory.get());
        if (ret.value() != static_cast<int>(tpi::ErrorValue::Success))
        {
            return ret;
        }*/
        cmdList.push_back(cmd);
        

        cout<<"addDefaultRules() done"<<endl;

        /*Ingress packet filtering and rate limiting rules for ICMP*/
        cmd.clear();

        for(i=0; i<chainTypeList.size(); i++)
        {
            for(j=0; j<icmpTypeList.size(); j++)
            {
                cmd = "add networking aclrule /" 
                      + ns
                      + " index " + std::to_string(index)
                      + " vrf default chain " + chainTypeList[i]
                      + " protocol icmp icmp-type " + icmpTypeList[j]
                      + " limit 25 limit-burst 8 accept";
                cmdList.push_back(cmd);
                index++;
                cmd = "add networking aclrule /" 
                      + ns
                      + " index " + std::to_string(index)
                      + " vrf default chain " + chainTypeList[i]
                      + " protocol icmp icmp-type " + icmpTypeList[j]
                      + " drop";
                cmdList.push_back(cmd);
                index++;
            }
        }

        /*Ingress rate limiting rules for TWAMP reflector and UDP echo server*/
        cmd = "add networking firewall twamp limit 100 limit-burst 100";
        cmdList.push_back(cmd);

        /*Ingress rate limiting rules for ARP request and reply*/
        cmd = "add networking firewall arp limit 300 limit-burst 2";
        cmdList.push_back(cmd);

        /*Spoof rule*/
        ret = getIpAndIfMap(ipMap,ifMap);
        if (ret != true)
        {
              return ret;
        }

        /*Spoof prevention - internal IP address/external interface/INPUT&OUTPUT chain*/
         for(iterIfMap=ifMap.begin(); iterIfMap!=ifMap.end(); iterIfMap++)
         {
             if("internal" != iterIfMap->second)
             {
                 /*found external IF*/
                 for(iterIpMap=ipMap.begin(); iterIpMap!=ipMap.end(); iterIpMap++)
                 {
                    iterIfMapTmp = ifMap.find(iterIpMap->first);
                    if((ifMap.end() != iterIfMapTmp)&&
                        ("internal" == iterIfMapTmp->second))
                    {
                        /*found internal IP*/
                        for(i=0; i<chainTypeList.size(); i++)
                        {
                            for(j=0; j<addrTypeList.size(); j++)
                            {
                                cmd = "add networking aclrule /" 
                                      + ns
                                      + " index " + std::to_string(spoofIndex)
                                      + " chain " + chainTypeList[i]
                                      + " " + ifTypeList[i] + " " + iterIfMap->first
                                      + " "+ addrTypeList[j] + " " + iterIpMap->second
                                      + " drop";
                                spoofIndex++;
                                cmdList.push_back(cmd);
                            }
                        }
                        /*
                        cmd = "add networking aclrule /" 
                              + ns
                              + " index " + std::to_string(spoofIndex)
                              + " vrf default chain input"
                              + " input-iface " + iterIfMap->first
                              + " srcaddr " + iterIpMap->second
                              + " drop";
                        
                        
                        cmd = "add networking aclrule /" 
                              + ns
                              + " index " + std::to_string(spoofIndex)
                              + " vrf default chain input"
                              + " input-iface " + iterIfMap->first
                              + " dstaddr " + iterIpMap->second
                              + " drop";
                        spoofIndex++;
                        cmdList.push_back(cmd);*/
                    }
                 }
                 /*add 4 more rules with IP 127.0.0.1 which is not specified in ip info*/

                 for(i=0; i<chainTypeList.size(); i++)
                 {
                     for(j=0; j<addrTypeList.size(); j++)
                     {
                         cmd = "add networking aclrule /" 
                               + ns
                               + " index " + std::to_string(spoofIndex)
                               + " chain " + chainTypeList[i]
                               + " " + ifTypeList[i] + " " + iterIfMap->first
                               + " "+ addrTypeList[j] + " 127.0.0.1"
                               + " drop";
                         spoofIndex++;
                         cmdList.push_back(cmd);
                     }
                 }
                 /*
                 cmd = "add networking aclrule /" 
                      + ns
                      + " index " + std::to_string(spoofIndex)
                      + " vrf default chain input"
                      + " input-iface " + iterIfMap->first
                      + " srcaddr 127.0.0.1"
                      + " drop";
                spoofIndex++;
                cmdList.push_back(cmd); 
                cmd = "add networking aclrule /" 
                      + ns
                      + " index " + std::to_string(spoofIndex)
                      + " vrf default chain input"
                      + " input-iface " + iterIfMap->first
                      + " dstaddr 127.0.0.1"
                      + " drop";
                spoofIndex++;
                cmdList.push_back(cmd);*/
                 
             }
         }

         /*drop all rules*/
         cmd = "add networking aclrule /" 
               + ns
               + " index " + std::to_string(ACL_DEFAULT_BOTTOM_INDEX_MAX)
               + " vrf default drop";
         cmdList.push_back(cmd);
         
         
        /*print all rules*/
        cout<<"all default rules:"<<endl;
        for(i=0; i<cmdList.size();i++)
        {
            cout<<cmdList[i]<<endl;
        }
        
        /*ret = tpl::exec_transaction_commands(cmdList, "Firewall", pErrCategory.get());
        if(ret.value() != static_cast<int>(tpi::ErrorValue::Success))
        {
            return ret;
        }*/
        return true;
        
        
    }


