#ifndef GENERATOR
#define GENERATOR

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

class Generator {
public:
    int tempCounter, ifLabelCounter, forLabelCounter, whileLabelCounter;
    std::stack<std::string> breakLabels;

	bool declareFuncParams;

    Generator() {
        ifLabelCounter = 0;
        forLabelCounter = 0;
        whileLabelCounter = 0;
        tempCounter = -1;
		declareFuncParams = false;
    }

    std::string generateTemp(){
        return "t"+std::to_string(++tempCounter);
    }

    std::string getCurrTemp(){
        return "t"+std::to_string(tempCounter);
    }

    std::string getIfLabelIdx(){
        return std::to_string(ifLabelCounter++);
    }
    
    std::string getForLabelIdx(){
        return std::to_string(forLabelCounter++);
    }
    
    std::string getWhileLabelIdx(){
        return std::to_string(whileLabelCounter++);
    }
};

#endif