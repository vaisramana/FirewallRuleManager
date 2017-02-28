run : main.o ACLRuleManager.o
	gcc -o run main.o ACLRuleManager.o -lstdc++ -std=c++11
main.o : main.cpp
	gcc -c main.cpp
ACLRuleManager.o : ACLRuleManager.cpp ACLRuleManager.hpp
	gcc -c ACLRuleManager.cpp