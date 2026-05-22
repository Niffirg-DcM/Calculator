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
    //Trash Code...
    // std::vector<Token> tokens;
    // std::vector<std::string> hold;
    // std::stringstream ss(expression);
    // std::string segment;

    // while (ss >> segment) {
    //     hold.push_back(segment);
    // }

    // for (int i =0;i<hold.size();i++){
    //     Token t;
    //     int op;
    //     if (hold[i].size() ==1) {
    //         op = isOperator(hold[i][0]);
    //         if (op!=0) {
    //             t.type = TokenType::Operator;
    //             t.symbol = hold[i];
    //             t.precedence = op;
    //         } else {
    //             t.type = TokenType::Number;
    //             t.value = std::strtod(hold[i].c_str(), nullptr);
    //         }
    //     } else {
    //         op = isFunction(hold[i]);
    //         if (op!=0) {
    //             t.type = TokenType::Function;
    //             t.symbol = hold[i];
    //         } else {
    //             t.type = TokenType::Number;
    //             t.value = std::strtod(hold[i].c_str(), nullptr);
    //         }
    //     }
    //     tokens.push_back(t);
    // }

    std::vector<Token> tokens;

    /*
    Index from start
    maintain first separately and use current index.
    when no longer digit make it an int and pass it to tokens.
    when switches from non int to int, pass it to tokens
    */
   int last =0;
   std::regex e("^[-+]?[0-9]*\\.?[0-9]+$");

    for (int i = 1; i < expressions.length(); i++) {
        Token t;
        //if substring expressions at i is digit and substring from last to i-1 isnt digit, turn last:i-1 into token of paren/op/func type
        //if substring expressions at i isn't digit and substring from last to i-1 is digit, turn last:i-1 into token of number type
        if (std::regex_match(expressions.substring(last, i-last-1)),e) {
            if (std::regex_match(expressions.substring(last, i-last)),e) {
                continue;
            } else {
                t.value = double(expressions.substring(last, i-1-last));
                t.type = TokenType::Number;
            }
        } else {
            if (std::regex_match(expressions.substring(i, 1)),e) {
                std::string hold = expressions.substring(last, i-1-last);
                t.type = TokenType::operation;

            } else {
                continue;
            }
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