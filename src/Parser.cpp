//
//  Parser.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-05-10.
//
#include <iostream>
#include <cctype>
#include <stack>
#include "../include/Parser.hpp"
#include <regex>

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

    /*
    while loop
    skip whitespace
    check for nums
    check for operators
    check for parenth
    check for funcs
    base case for unknown chars
    */
   int i = 0;
   int n = expression.length();

    while(i<n) {
        char c = expression[i];
        if (std::isspace(c)) {
            i++;
            continue;
        }

        if (std::isdigit(c) || c=='.') {
            int start = i;
            bool has_dec = false;

            while (i<n && (std::isdigit(expression[i]) || expression[i]=='.')) {
                if (expression[i]=='.') {
                    if (has_dec) break;
                    has_dec = true;
                }
                i++
            }
            Token t;
            t.type = TokenType::Number;
            t.value = std::stod(expression.substr(start,i-start));
            tokens.push_back(t);
        }

        else if (isOperator(c)>0) {
            Token t;
            t.type == TokenType::Operator;
            t.symbol = c;
            tokens.push_back(t);
        }
        else if (isFunction(c)>0) {
            Token t;
            t.type == TokenType::Function;
            t.symbol = c;
            tokens.push_back(t);
        }
        else if ((int v = isParen(c))>0) {
            Token t;    
            if (v==1) {
                t.type == TokenType::LeftParen;
            } else {
                t.type == TokenType::RightParen;
            }
            t.symbol = c;
            tokens.push_back(t);
        } else {
            
        }

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