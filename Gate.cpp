#include "Gate.h"

Wire* Gate::getInput(int i) const 
{
    if (i == 1) return input1;
    else return input2;
}

unsigned int Gate::evaluate() const {
    std::cout << "[GTEVL] Evaluating gate at " << this << std::endl;
    unsigned int i1 = input1->getValue();
    unsigned int i2 = input2->getValue();
    std::cout << "i1: " << i1 << " i2: " << i2 << std::endl;
    unsigned int returnVal;
    if (i1 == UNKNOWN && type == NOT) returnVal = UNKNOWN; // edge cases. no reason for their order
    else if (i1 + i2 == 4) returnVal = UNKNOWN; // two unknowns
    else if (i1 + i2 == 3) // One high one unknown input
    {
        if      (type ==  AND) returnVal = UNKNOWN;
        else if (type ==  XOR) returnVal = UNKNOWN;
        else if (type == NAND) returnVal = UNKNOWN;
        else if (type == XNOR) returnVal = UNKNOWN;

        else if (type ==   OR) returnVal = HIGH;
        else if (type ==  NOR) returnVal = LOW;

        else if (type ==  NOT) returnVal = LOW; // Only possible instance of one high and one unknown is known
        else std::cout << "[GTEVAL] Unrecognized gate type" << std::endl;
    }
    else if ((i1 == UNKNOWN || i2 == UNKNOWN) && i1 + i2 == 2) // One low one unknown input
    {
        if (type ==       AND) returnVal = LOW;
        else if (type == NAND) returnVal = HIGH;

        else if (type ==   OR) returnVal = UNKNOWN;
        else if (type ==  NOR) returnVal = UNKNOWN;
        else if (type ==  XOR) returnVal = UNKNOWN;
        else if (type == XNOR) returnVal = UNKNOWN;
        else if (type ==  NOT) returnVal = HIGH;
        else std::cout << "[GTEVAL] Unrecognized gate type" << std::endl;
    }
    else // Do bitwise math on i1 and i2 to save the code
    {
        if      (type ==  NOT) returnVal = unsigned int(!i1); // the stacked else ifs here actually take less space than a switch
        else if (type ==  AND) returnVal = unsigned int(i1 & i2);
        else if (type ==   OR) returnVal = unsigned int(i1 | i2);
        else if (type ==  XOR) returnVal = unsigned int(i1 ^ i2);
        else if (type == NAND) returnVal = unsigned int(!(i1 & i2));
        else if (type ==  NOR) returnVal = unsigned int(!(i1 | i2));
        else if (type == XNOR) returnVal = unsigned int(!(i1 ^ i2));
        else std::cout << "[GTEVAL] Unrecognized gate type" << std::endl;
    }
    // output->setValue(returnVal); // Let main function take care of this so gate delay can be handled there as well
    std::cout << "[GTEVAL] Finished evaluating: " << returnVal << std::endl;
    return returnVal;
}