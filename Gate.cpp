#include "Gate.h"
/*
int Gate::andGate{
    if (i1 == 0 || i2 == 0) {
        output = 0;
    }
    if ((i1 == UNKNOWN && i2 == 1) || (i1 == 1 && i2 == UNKNOWN) || (i1 == UNKNOWN && i2 == UNKNOWN)) {
        output = UNKNOWN;
    }
    else if (i1 == 1 && i2 == 1) {
        output = 1;
    }
}

int Gate::NandGate{
    if (i1 == 0 || i2 == 0) {
        output = 1;
    }
    if ((i1 == UNKNOWN && i2 == 1) || (i1 == 1 && i2 == UNKNOWN) || (i1 == UNKNOWN && i2 == UNKNOWN)) {
        output = UNKNOWN;
    }
    else if (i1 == 1 && i2 == 1) {
        output = 0;
    }
}

int Gate::orGate{
    if (i1 == 1 || i2 == 1) {
        output == 1;
    }
    if ((i1 == UNKNOWN && i2 == 0) || (i1 == 0 && i2 == UNKNOWN) || (i1 == UNKNOWN && i2 == UNKNOWN)) {
        output = UNKNOWN;
    }
    else if (i1 == 0 && i2 == 0) {
        output = 0;
    }
}

int Gate::NorGate{
    if (i1 == 1 || i2 == 1) {
        output == 0;
    }
    if ((i1 == UNKNOWN && i2 == 0) || (i1 == 0 && i2 == UNKNOWN) || (i1 == UNKNOWN && i2 == UNKNOWN)) {
        output = UNKNOWN;
    }
    else if (i1 == 0 && i2 == 0) {
        output = 1;
    }
}

int Gate::xorGate{
    if ((i1 == 1 && i2 == 0) || (i1 == 0 && i2 == 1)) {
        output = 1;
    }
    if (i1 == i2) {
        output = 0;
    }
    if (i1 == UNKNOWN || i2 == UNKNOWN) {
        output = UNKNOWN;
    }
}

int Gate::xNorGate{
    if ((i1 == 1 && i2 == 0) || (i1 == 0 && i2 == 1)) {
        output = 0;
    }
    if (i1 == i2) {
        output = 1;
    }
    if (i1 == UNKNOWN || i2 == UNKNOWN) {
        output = UNKNOWN;
    }
}

int Gate::notGate{
    if (i1 == 1) {
        output = 0;
    }
    else if (i1 == 0) {
        output = 1;
    }
    if (i2 == 1) {
        output = 0;
    }
    else if (i2 == 0) {
        output = 1;
    }
    if (i1 == UNKNOWN || i2 == UNKNOWN){
        output = UNKNOWN;
    }
}
*/
int Gate::getDelay() const { return delay; }
Wire* Gate::getInput1(int) const { return input1; }
Wire* Gate::getInput2(int) const { return input2; }
Wire* Gate::getOutput() const { return output; }
int Gate::evaluate() const {
    int i1 = input1->getValue();
    int i2 = input2->getValue();
    int returnVal;
    int irrlelvantfasdfads = 69;
    if (i1 == UNKNOWN && type == NOT) returnVal = UNKNOWN; // edge cases. no reason for their order
    else if (i1 == UNKNOWN && i2 == UNKNOWN) returnVal = UNKNOWN; // two unknowns
    else if (i1 + i2 == 3) // One high one unknown input
    {
        if (type == AND) returnVal = UNKNOWN;
        else if (type == XOR) returnVal = UNKNOWN;
        else if (type == NAND) returnVal = UNKNOWN;
        else if (type == XNOR) returnVal = UNKNOWN;

        else if (type == OR) returnVal = HIGH;
        else if (type == NOR) returnVal = LOW;
    }
    else if (i1 + i2 == 2) // One low one unknown input
    {
        if (type == AND) returnVal = LOW;
        else if (type == NAND) returnVal = HIGH;

        else if (type == OR) returnVal = UNKNOWN;
        else if (type == NOR) returnVal = UNKNOWN;
        else if (type == XOR) returnVal = UNKNOWN;
        else if (type == XNOR) returnVal = UNKNOWN;
    }
    else // Do bitwise math on i1 and i2 to save the code
    {
        if (type == NOT) returnVal = !i1;
        else if (type == AND) returnVal = i1 & i2;
        else if (type == OR) returnVal = i1 | i2;
        else if (type == XOR) returnVal = i1 ^ i2;
        else if (type == NAND) returnVal = !(i1 & i2);
        else if (type == NOR) returnVal = !(i1 | i2);
        else if (type == XNOR) returnVal = !(i1 ^ i2);
    }
    return returnVal;
}