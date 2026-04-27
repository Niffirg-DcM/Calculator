//
//  main.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//

#include <cmath>
#include <iostream>

#include "calculator.h"

int main(int argc, const char * argv[]) {
    Calculator* calc = new Calculator();
    bool q = true;
    while(q) {
        double hold;
        int hold1;
        std::cout<<"0 for quit, 1 for add, 2 for subtract, 3 for mult, 4 for divide, 5 for power: ";
        std::cin>>hold1;
        if (hold1 == 0) {
            q = false;
            break;
        }
        calc->setOp(hold1);
        std::cout<<"number 1: ";
        std::cin>>hold;
        calc->setParam1(hold);
        std::cout<<"number 2: ";
        std::cin>>hold;
        calc->setParam2(hold);
        q=calc->call();
        
        
    }
    return 0;
}
