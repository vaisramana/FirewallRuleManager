run : main.o ACLRuleManager.o CommonFunc.o
	gcc -o run main.o ACLRuleManager.o CommonFunc.o -lstdc++ -std=c++11
main.o : main.cpp
	gcc -c main.cpp
CommonFunc.o : CommonFunc.cpp CommonFunc.hpp
	gcc -c CommonFunc.cpp
ACLRuleManager.o : ACLRuleManager.cpp ACLRuleManager.hpp
	gcc -c ACLRuleManager.cpp