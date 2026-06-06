//
//  main.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//

#include <iostream>
#include <string>
#include "../include/Calculator.hpp"
#include "../include/McpIO.hpp"

//main function to launch calculator
int main() {
    Calculator calc;
    McpIO handler;
    std::string input;

    std::cout << "=== C++ Scientific Calculator ===" << std::endl;
    std::cout << "Type 'exit' to quit. or 'query' to make a query to Gemini" << std::endl;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        if (input == "query") {
            std::cout << ">> Query Here: ";
            std::getline(std::cin, input);
            //send query to llm
            std::cout<<handler.handleRequest(input)<<std::endl;
        } else {
            calc.calculate(input);
        }
    }

    return 0;
}