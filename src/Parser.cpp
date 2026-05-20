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
    std::vector<Token> tokens = tokenize(expression);
    std::queue<Token> outputQueue;
    std::stack<Token> opStack;

    for (const Token& current : tokens) {
        if (current.type == TokenType::Number) {
            outputQueue.push(current);
        } 
        else if (current.type == TokenType::Function) {
            opStack.push(current);
        }
        else if (current.type == TokenType::LeftParen) {
            opStack.push(current);
        }
        else if (current.type == TokenType::RightParen) {
            while (opStack.top().type == TokenType::LeftParen) {
                outputQueue.push(opStack.top());
                opStack.pop();
            }

        }
        else if (current.type == TokenType::Operator) {
            while(!opStack.empty() && opStack.top().type != TokenType::LeftParen && opStack.top().precedence >= current.precedence) {
                outputQueue.push(opStack.top());
                opStack.pop();
            }
            opStack.push(current);
        }
    }

    while(!opStack.empty()) {
        outputQueue.push(opStack.top());
        opStack.pop();
    }

    return outputQueue;
}

//Breaks string into individual units (numbers, ops, parens)
std::vector<Token> Parser::tokenize(const std::string& expression) {
    std::vector<Token> tokens;
    std::vector<std::string> hold;
    std::stringstream ss(expression);
    std::string segment;

    while (ss >> segment) {
        hold.push_back(segment);
    }

    for (int i =0;i<hold.size();i++){
        Token t;
        int op;
        if (hold[i].size() ==1) {
            op = isOperator(hold[i][0]);
            if (op!=0) {
                t.type = TokenType::Operator;
                t.symbol = hold[i];
                t.precedence = op;
            } else {
                t.type = TokenType::Number;
                t.value = std::strtod(hold[i].c_str(), nullptr);
            }
        } else {
            op = isFunction(hold[i]);
            if (op!=0) {
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
int Parser::isOperator(char c) {
    switch(c) {
        case '-': return 1;
        case '+': return 2;
        case '*': return 3;
        case '/': return 4;
        case '^': return 5;
    }
    return 0;
}

int Parser::isParen(char c) {
    switch(c) {
        case '(': return 1;
        case ')': return 2;
    }
    return 0;
}

//Helper to check if a string is a math function
bool Parser::isFunction(const std::string& s) {
    if (s =="sin"||s=="log"||s=="cos"||s=="sininv"||s=="cosinv") return true;
    return false;
}