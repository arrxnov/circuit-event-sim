#include "Gate.h"
#include <fstream>

int main(int argc, char** argv)
{   
    using namespace std;
    if (argc != 2)
    {
        cout << "[+] Usage: " << argv[0] << " <CIRCUIT FILE> <VECTOR FILE>" << endl;
        return 1;
    }
    ifstream circuitFile;
    circuitFile.open(argv[1]);
    if (circuitFile.fail())
    {
        cout << "[X] Failed to open Circuit File!" << endl;
        return 2;
    }
    ifstream vectorFile;
    vectorFile.open(argv[2]);
    if (vectorFile.fail())
    {
        cout << "[X] Failed to open Vector File!" << endl;
        return 2;
    }
    
    vector<Gate*> gates;
    vector<Wire*> wires;
    vector<string> events;
    string name;
    string type;
    string data;
    
    // Load in circuit data and construct

    circuitFile >> type >> name;
    while (!circuitFile.eof())
    {
        circuitFile >> data;
        if (data.find("INPUT") != string::npos || data.find("OUTPUT") != string::npos)
        {
            char* data_c = new char[data.length() + 1]; // Hack solution to make the c_string param in Wire() work with a string 
            strcpy(data_c, data.c_str());
            Wire* newWire = new Wire(data_c); // construct new wire with data, constructor interprets data
            wires.push_back(newWire);
        }
        else
        {
            stringstream ss;
            string type, delay;
            int type_i, i1, i2, o;
            Wire* i1_wp;
            Wire* i2_wp;
            Wire* o_wp; // VS complained if these didn't have their own lines. Not sure why that makes a difference
            ss << data;
            ss >> type >> delay >> i1 >> i2 >> o;
            if (type == "NOT")
            {
                o = i2; // edge case housekeeping
                                     type_i =  NOT;
            }
            else if (type == "AND" ) type_i =  AND;
            else if (type == "OR"  ) type_i =   OR;
            else if (type == "XOR" ) type_i =  XOR;
            else if (type == "NAND") type_i = NAND;
            else if (type == "NOR" ) type_i =  NOR;
            else if (type == "XNOR") type_i = XNOR;
            // search for matching wires
            for (int i = 0; i < wires.size(); i++)
            {
                if (wires.at(i)->getIndex() == i1) i1_wp = wires.at(i); // all if statements without else to protect against improper handling of gates with both inputs tied to the same wire
                if (wires.at(i)->getIndex() == i2) i2_wp = wires.at(i);
                if (wires.at(i)->getIndex() == o ) o_wp  = wires.at(i);
            }
        }
    }

    // Load in vector data and construct

    string type, name;
    int time, val;

    while (!vectorFile.eof()) 
    {
        vectorFile >> data;
        events.push_back(data);
    }

    while (true)
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
    }

    return 0;
}