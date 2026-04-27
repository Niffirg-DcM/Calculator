//
//  calculator.h
//  calc
//
//  Created by Griffin McDonald on 2026-04-26.
//
struct operation {
    double param1;
    double param2;
    
    int operand;
    double output;
};


class Calculator {
    double param1;
    double param2;
    
    int operand;
    
    double output;
    
    std::vector<operation> history;
private:
    void reset();
    
public:
    bool call();
    
    void add();

    void sub();

    void mul();

    void div();

    void pow();
    
    double getOp();
    
    double getParam1();
    
    double getParam2();
    
    void setOp(int i);
    
    void setParam1(double x);
    
    void setParam2(double y);
};
