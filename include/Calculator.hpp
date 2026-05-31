#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "Parser.hpp"
#include "Evaluator.hpp"
#include <string>

class Calculator {
public:
    Calculator() = default;
    
    void calculate(const std::string& input);

private:
    Parser parser;
    Evaluator evaluator;
    double lastResult = 0.0;
    
    void displayResult(const std::string& message, double result) const;
    void handleError(const std::string& message) const;
};

#endif