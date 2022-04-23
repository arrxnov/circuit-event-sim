#include "Gate.h"

Gate::Gate(char* data_c)
{
    std::string data = data_c;
    if (data.find("NOT") != std::string::npos) type = NOT;
    else if (data.find("AND") != std::string::npos) type = AND;
    else if (data.find("OR") != std::string::npos) type = OR;
    else if (data.find("XOR") != std::string::npos) type = XOR;
    else if (data.find("NAND") != std::string::npos) type = NAND;
    else if (data.find("NOR") != std::string::npos) type = NOR;
    else if (data.find("XNOR") != std::string::npos) type = XNOR;

    std::stringstream data_s;
    data_s << data;
}

Wire* Gate::getInput(int i) const 
{
    if (i == 1) return input1;
    else return input2;
}

int Gate::evaluate() const {
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
    output->setValue(returnVal);
    return returnVal;
}