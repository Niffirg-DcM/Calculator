//
//  Evaluator.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-05-10.
//
#include "../include/Evaluator.hpp"
#include <cmath>
#include <iostream>

//assuming valid RPN input
//evaluate postfixqueue for result
double Evaluator::evaluate(std::queue<Token> postfixQueue) {
    double result;
    std::stack<double> active = std::stack<double>();
    std::string op;
    Token curr;

    //while there are values in the queue, parse through them.
    while (!postfixQueue.empty()) {
        curr = postfixQueue.front();
        postfixQueue.pop();

        //add numbers immediately
        if (curr.type == TokenType::Number) {
            active.push(curr.value);
            //if there is an operator check if there are enough values to apply it or not. then apply op to last two values and append to result
        } else if (curr.type == TokenType::Operator) {
            if (active.size() < 2) {
                std::cerr<<"not enough active stack members for operation"<<std::endl;
                return 0.0;
            }
            double rightVal = active.top();
            active.pop();
            double leftVal = active.top();
            active.pop();
            result = applyOperator(leftVal,rightVal,curr.symbol);
            active.push(result);
            //if there is function, make sure there are enough values to apply it or not, then apply it and append to result
        } else if (curr.type == TokenType::Function) {
            if (active.empty()) {
                std::cerr<<"not enough active stack members for function"<<std::endl;
                return 0.0;
            }
            double inputVal = active.top();
            active.pop();
            result = applyFunction(inputVal,curr.symbol);
            active.push(result);

        }
    }
    if (active.empty()) return 0.0;

    return active.top();
}

//Helper to apply opperators
double Evaluator::applyOperator(double leftVal, double rightVal,const std::string& op) {    
    if(op=="+") {
        return leftVal + rightVal;
    } else if (op =="-") {
       return leftVal - rightVal;
    } else if (op =="*") {
        return leftVal * rightVal;
    } else if (op =="/") {
        return leftVal / rightVal;
    } else {
        return std::pow(leftVal,rightVal);
    }
}

//Helper to apply functions
double Evaluator::applyFunction(double inputVal, const std::string& func) {
    if (func=="log") {
        return std::log(inputVal);
    } else if (func=="sin") {
        return std::sin(inputVal);
    } else if (func=="cos") {
        return std::cos(inputVal);
    } else if (func=="tan") {
        return std::tan(inputVal);
    } else if (func=="sqrt") {
        return std::sqrt(inputVal);
    }else if (func=="sqr") {
        return std::pow(inputVal,2);
    }
    return 0;

}

