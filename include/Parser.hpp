#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <queue>

enum class TokenType { Number, Operator, Function, LeftParen, RightParen };

struct Token {
    TokenType type;
    double value;
    std::string symbol;
    int precedence;
};

class Parser {
public:
    std::queue<Token> parseToPostfix(const std::string& expression);

private:
    std::vector<Token> tokenize(const std::string& expression);
    bool isOperator(char c);
    bool isFunction(const std::string& s);
};

#endif