#ifndef GATE_H
#define GATE_H
#include "Wire.h"

#define NOT 0
#define AND 1
#define OR 2
#define XOR 3
#define NAND 4
#define NOR 5
#define XNOR 6

#define LOW 0
#define HIGH 1
#define UNKNOWN 2

class Gate {    
public:
    Gate::Gate(int type, int delay, Wire* input1, Wire* input2, Wire* output): type(type), delay(delay), input1(input1), input2(input2), output(output){};
    int getDelay() const;
    Wire* getInput(int) const;
    Wire* getOutput() const;
    int evaluate() const;
    
private:
    unsigned int type;
    int delay;
    Wire* input1;
    Wire* input2;
    Wire* output;   
};

#endif