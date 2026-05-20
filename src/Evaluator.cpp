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
double Evaluator::evaluate(std::queue<Token> postfixQueue) {
    Token result;
    std::stack<Token> active = std::stack<Token>();
    std::string op;

    while (!postfixQueue.empty()) {
        active.push(postfixQueue.front());
        result = postfixQueue.front();
        std::cout<<result.value()<<std::endl;
        postfixQueue.pop();
        if (active.size() > 2) {
            if(active.top().type == TokenType::Operator) {
                op = active.top().symbol;
                active.pop();
                applyOperator(active,op);
            }
        }
    }
    result = active.top();

    return result.value;
}

void Evaluator::applyOperator(std::stack<Token>& values, const std::string& op) {
    Token value;
    if(op=="+") {
        value.value = values.top().value;
        values.pop();
        value.value = value.value+values.top().value;
        values.pop();
        value.type = TokenType::Number;
        values.push(value);
        } else if (op =="-") {
        value.value = values.top().value;
        values.pop();
        value.value = value.value-values.top().value;
        values.pop();
        value.type = TokenType::Number;
        values.push(value);
        } else if (op =="*") {
        value.value = values.top().value;
        values.pop();
        value.value = value.value*values.top().value;
        values.pop();
        value.type = TokenType::Number;
        values.push(value);
        } else if (op =="/") {
        value.value = values.top().value;
        values.pop();
        value.value = value.value/values.top().value;
        values.pop();
        value.type = TokenType::Number;
        values.push(value);
        } else if (op =="^") {
        value.value = values.top().value;
        values.pop();
        value.value = pow(value.value,values.top().value);
        values.pop();
        value.type = TokenType::Number;
        values.push(value);
    }

}
void Evaluator::applyFunction(std::stack<Token>& values, const std::string& func) {

}

