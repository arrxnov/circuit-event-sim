#include "Gate.h"

Wire* Gate::getInput(int i) const 
{
    if (i == 1) return input1;
    else return input2;
}

int Gate::evaluate() const {
    std::cout << "[GTEVL] Evaluating gate at " << this << std::endl;
    int i1 = input1->getValue();
    int i2 = input2->getValue();
    int returnVal;
    if (i1 == UNKNOWN && type == NOT) returnVal = UNKNOWN; // edge cases. no reason for their order
    else if (i1 + i2 == 4) returnVal = UNKNOWN; // two unknowns
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
        if (type == NOT) returnVal = !i1; // the stacked else ifs here actually take less space than a switch
        else if (type == AND) returnVal = i1 & i2;
        else if (type == OR) returnVal = i1 | i2;
        else if (type == XOR) returnVal = i1 ^ i2;
        else if (type == NAND) returnVal = !(i1 & i2);
        else if (type == NOR) returnVal = !(i1 | i2);
        else if (type == XNOR) returnVal = !(i1 ^ i2);
    }
    // output->setValue(returnVal); // Let main function take care of this so gate delay can be handled there as well
    std::cout << "[GTEVAL] Finished evaluating" << std::endl;
    return returnVal;
}