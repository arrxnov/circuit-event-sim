#include "Wire.h"

Wire::Wire(char* n)
{
    std::string data = n;
    std::stringstream ss;
    std::string type;
    char name_cnp; // Character not-pointer. Clearest syntax award goes to...
    ss << data;
    ss >> std::skipws >> type >> name_cnp >> index;
    name = new char;
    *name = name_cnp;
    name[1] = '\0'; // Fix output for strings
    // std::cout << "[WIRE CONSTR] Type: " << type << ", Name: " << name << ", Index: " << index << std::endl;  
}   

void Wire::appendHist(int h, int time)
{
    // std::cout << "[GATEHIST] History length: " << time << std::endl;
    for (int i = 0; i < time; i++)
    {
        history.push_back(h);
        // std::cout << "[GATEHIST] Added " << h << std::endl;
    }
}

void Wire::editDrives(Gate* g, int e=ADD)
{
    if (e == ADD) drives.push_back(g);
    else if (e == REMOVE)
    {
        for (int i = 0; i < drives.size(); i++)
        {
            if (drives.at(i) == g)
            {
                auto iterator = drives.begin();
                drives.erase(iterator+i);
            }
        }
    }
}

void Wire::printHistory() const // Does not implement endl!!
{
    std::cout << "[Wire: " << *name << "] ";
    
    for (int i = 0; i < history.size(); i++)
    {
        if (history.at(i) == HIGH) std::cout << "-";
        else if (history.at(i) == LOW) std::cout << "_";
        else if (history.at(i) == UNKNOWN) std::cout << "X";

        if (i > 90) break;
    }
}