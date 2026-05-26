#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "Parser.hpp"
#include <stack>
#include <queue>

class Evaluator {
public:
    double evaluate(std::queue<Token> postfixQueue);

private:
    double applyOperator(double leftVal, double rightVal,const std::string& op);
    double applyFunction(double inputVal, const std::string& func);
};

#endif