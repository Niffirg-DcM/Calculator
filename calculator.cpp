//
//  calculator.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//
#include <vector>
#include <iostream>
#include "calculator.h"


void Calculator::reset() {
    param1=0;
    param2=0;
    operand = 0;
    output = 0;
    return;
}

bool Calculator::call() {
    switch(operand) {
        case 1: {
            add();
            break;
        }
        case 2: {
            sub();
            break;
        }
        case 3: {
            mul();
            break;
        }
        case 4: {
            div();
            break;
        }
        case 5: {
            pow();
            break;
        }
        default:
            reset();
            return false;
            
    }
    std::cout<<output<<std::endl;
    history.push_back(operation(param1,param2,operand,output));
    reset();
    return true;
    
}

void Calculator::add() {
    output=param1+param2;
}

void Calculator::sub() {
    output=param1-param2;
}

void Calculator::mul() {
    output= param1*param2;
}

void Calculator::div() {
    output= param1/param2;
}

void Calculator::pow() {
    output=1;
    for (int i =0;i<param2;i++) output*=param1;
}

double Calculator::getOp() {
    return operand;
}

double Calculator::getParam1() {
    return param1;
}

double Calculator::getParam2() {
    return param2;
}

void Calculator::setOp(int i) {
    operand = i;
}

void Calculator::setParam1(double x) {
    param1 = x;
}

void Calculator::setParam2(double y) {
    param2 = y;
}

