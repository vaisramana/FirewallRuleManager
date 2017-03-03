#ifndef COMMON_FUNC_HPP_
#define COMMON_FUNC_HPP_


#include <functional>
#include <string>
#include <vector>
//#include <boost/optional.hpp>
//#include <boost/filesystem.hpp>


/*
template <class T>
using Optional = boost::optional<T>;

template <class T>
using In = const T&;

template <class T>
using Out = T&;

template <class T> 
using InOut = T&;
*/

void printACLRuleList(vector<ACLRule>  ruleList);
void printVector(vector<int> v);
void printVectorVector(vector<vector<int> > v);

#endif


