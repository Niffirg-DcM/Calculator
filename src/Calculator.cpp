//
//  Calculator.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//
#include <vector>
#include <iostream>
#include "../include/Calculator.hpp"

// Parser parser;
// Evaluator evaluator;
// double lastResult = 0.0;


void Calculator::calculate(const std::string& input){
    std::queue<Token> rpn = parser.parseToPostfix(input);
    double currentResult = evaluator.evaluate(rpn);
    displayResult(currentResult);
    lastResult = currentResult;
}
    
void Calculator::displayResult(double result) const {
    std::cout << result << std::endl;
    return;
}

void Calculator::handleError(const std::string& message) const {
    return;
}
