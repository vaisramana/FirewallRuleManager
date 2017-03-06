run : main.o ACLRuleManager.o CommonFunc.o
	g++ -o run main.o ACLRuleManager.o CommonFunc.o -std=c++11 -lboost_system
main.o : main.cpp
	g++ -c main.cpp -std=c++11
CommonFunc.o : CommonFunc.cpp CommonFunc.hpp
	g++ -c CommonFunc.cpp -std=c++11
ACLRuleManager.o : ACLRuleManager.cpp ACLRuleManager.hpp
	g++ -c ACLRuleManager.cpp -std=c++11