#include "Gate.h"

Wire* Gate::getInput(int i) const 
{
    if (i == 1) return input1;
    else if (i == 2) return input2;
    else return nullptr;
}

unsigned int Gate::evaluate() const {
    unsigned int i1 = input1->getValue();
    unsigned int i2 = input2->getValue();
    unsigned int returnVal = UNKNOWN; // defaulting causes much easier edge case handling
    if (i1 + i2 == 3) // One high one unknown input
    {
        if      (type ==   OR) returnVal = HIGH;
        else if (type ==  NOR) returnVal = LOW;
        else if (type ==  NOT) returnVal = LOW; // Only possible instance of one high and one unknown is known
    }
    else if ((i1 == UNKNOWN || i2 == UNKNOWN) && i1 + i2 == 2) // One low one unknown input
    {
        if      (type ==  AND) returnVal = LOW;
        else if (type == NAND) returnVal = HIGH;
        else if (type ==  NOT) returnVal = HIGH; // Again, output is known
    }
    else // Do bitwise math on i1 and i2 to save the code
    {
        if      (type ==  NOT) returnVal = (unsigned int)(!i1); // the stacked else ifs here actually take less space than a switch
        else if (type ==  AND) returnVal = (unsigned int)(i1 & i2);
        else if (type ==   OR) returnVal = (unsigned int)(i1 | i2);
        else if (type ==  XOR) returnVal = (unsigned int)(i1 ^ i2);
        else if (type == NAND) returnVal = (unsigned int)(!(i1 & i2));
        else if (type ==  NOR) returnVal = (unsigned int)(!(i1 | i2));
        else if (type == XNOR) returnVal = (unsigned int)(!(i1 ^ i2));
    }
    return returnVal;
}