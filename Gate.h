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
    Gate::Gate(int t, int d, Wire* i1, Wire* i2, Wire* o) : type(t), delay(d), input1(i1), input2(i2), output(o) {};
    int getDelay() const { return delay; };
    Wire* getInput(int) const;
    Wire* getOutput() const { return output; };
    unsigned int evaluate() const;
    
private:
    unsigned int type;
    int delay;
    Wire* input1;
    Wire* input2;
    Wire* output;
};

#endif