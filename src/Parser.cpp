//
//  Parser.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-05-10.
//
#include <iostream>
#include <sstream>
#include <cctype>
#include <stack>
#include "../include/Parser.hpp"

//Converts raw string into RPN queue
std::queue<Token> Parser::parseToPostfix(const std::string& expression) {
    int ind1 =0;
    std::queue<Token> rpn = std::queue<Token>();
    std::vector<Token> tokenize();


    return std::queue<Token>(); 
}

//Breaks string into individual units (numbers, ops, parens)
std::vector<Token> Parser::tokenize(const std::string& expression) {
    std::vector<Token> tokens;
    std::vector<std::string> hold;
    std::stringstream ss(expression);
    std::string segment;

    //TODO: Loop through string and identify TokenTypes
    while (ss >> segment) {
        hold.push_back(segment);
    }

    for (int i =0;i<hold.size();i++){
        Token t;
        if (hold[i].size() ==1) {
            if (isOperator(hold[i][0])) {
                t.type = TokenType::Operator;
                t.symbol = hold[i];
            } else {
                t.type = TokenType::Number;
                t.value = std::strtod(hold[i].c_str(), nullptr);
            }
        } else {
            if (isFunction(hold[i])) {
                t.type = TokenType::Function;
                t.symbol = hold[i];
            } else {
                t.type = TokenType::Number;
                t.value = std::strtod(hold[i].c_str(), nullptr);
            }
        }
        tokens.push_back(t);
    }
    return tokens;
}

// struct Token {
//     TokenType type;
//     double value;
//     std::string symbol;
//     int precedence;
// };

//Helper to check if a character is a supported operator
bool Parser::isOperator(char c) {
    //TODO: Return true if c is +, -, *, /, ^
    if (c=='+'||c=='-'||c=='*'||c=='/'||c=='^') return true;
    return false;
}

//Helper to check if a string is a math function
bool Parser::isFunction(const std::string& s) {
    //TODO: Check against a list of supported function names
    if (s =="sin"||s=="log"||s=="cos"||s=="sininv"||s=="cosinv") return true;
    return false;
}