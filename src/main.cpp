//
//  main.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//

#include <iostream>
#include <string>
#include "../include/Calculator.hpp"

//main function to launch calculator
int main() {
    Calculator calc;
    std::string input;

    std::cout << "=== C++ Scientific Calculator ===" << std::endl;
    std::cout << "Type 'exit' to quit." << std::endl;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        calc.calculate(input);
    }

    return 0;
}