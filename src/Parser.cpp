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


    //parse through each token in the vector, place it into its appropriate order, return it.
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
   int v;
   bool lastWasOp = true;
   int toBeNeg = 1;

   //char by char determine if a sequence is a valid number, operator or function, tokenize them and add them to the vector to be returned
    while(i<n) {
        char c = expression[i];
        if (std::isspace(c)) {
            i++;
            continue;
        }

        //check for decimal start
        if (std::isdigit(c) || c=='.') {
            int start = i;
            bool has_dec = false;

            //test for dec and digit, if >1 decimal in expression then it is not valid here.
            while (i<n && (std::isdigit(expression[i]) || expression[i]=='.')) {
                if (expression[i]=='.') {
                    if (has_dec) break;
                    has_dec = true;
                }
                i++;
            }
            Token t;
            t.type = TokenType::Number;
            t.value = toBeNeg * std::stod(expression.substr(start,i-start));
            toBeNeg = 1;
            tokens.push_back(t);
            lastWasOp = false;

            //check for an operator
        } else if ((v=isOperator(c))>0) {
            //if the last action was an operator/leftParen and this is neg ie '(-...' then we know the following will be a neg number.
            if(lastWasOp && v==1) {
                toBeNeg = -1;
            } else {
                Token t;
                t.type = TokenType::Operator;
                t.symbol = std::string(1, c);;
                tokens.push_back(t);
            }
            i++;
            lastWasOp = true;

            //check for parenths
        } else if ((v = isParen(c))>0) {
            Token t;    
            if (v==1) {
                t.type = TokenType::LeftParen;
                lastWasOp = true;
            } else {
                t.type = TokenType::RightParen;
                lastWasOp = false;
            }
            t.symbol = c;
            tokens.push_back(t);
            i++;

            //check for functions by testing if this is a valid 'character' and looping until we encapsulate it entirely
        } else if (std::isalpha(c)) {
            int start = i;
            while (i < n && std::isalpha(expression[i])) {
                i++;
            }

            Token t;
            t.type = TokenType::Function;
            t.symbol = expression.substr(start, i - start);
            tokens.push_back(t);
            lastWasOp = false;
        } else {
            std::cout<<"no idea what this is means"<<std::endl;
            i++;
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

//Helper to check if a character is a parenthesis
int Parser::isParen(char c) {
    switch(c) {
        case '(': return 1;
        case ')': return 2;
    }
    return 0;
}

//Helper to check if a string is a math function
bool Parser::isFunction(const std::string& s) {
    if (s =="sin"||s=="log"||s=="cos"||s=="tan"||s=="sqrt"||s=="sqr") return true;
    return false;
}