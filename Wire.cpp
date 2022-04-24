#include "Wire.h"

Wire::Wire(char* n = "Unnamed")
{
    std::string data = n;
    std::stringstream ss;
    std::string type;
    ss << data;
    ss >> type >> name >> index;
    std::cout << "[WIRE CONSTR] Type: " << type << ", Name: " << name << ", Index: " << index << std::endl;
}

void Wire::appendHist(int h, int time)
{
    for (int i = 0; i < time; i++) history.push_back(h);
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
    for (int i = 0; i < history.size(); i++)
    {
        if (history.at(i) == HIGH) std::cout << "-";
        else if (history.at(i) == LOW) std::cout << "_";
        else if (history.at(i) == UNKNOWN) std::cout << "X";
    }
}