#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "Parser.hpp"
#include <stack>
#include <queue>

class Evaluator {
public:
    double evaluate(std::queue<Token> postfixQueue);

private:
    void applyOperator(std::stack<double>& values, const std::string& op);
    void applyFunction(std::stack<double>& values, const std::string& func);
};

#endif