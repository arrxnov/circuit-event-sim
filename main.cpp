#include "Gate.h"
#include <fstream>

int main(int argc, char** argv)
{   
    using namespace std;
    if (argc != 3)
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

    getline(circuitFile, type); // Grab irrelevant data from top
    while (!circuitFile.eof())
    {
        getline(circuitFile, data);
        cout << data << endl;
        if (data.find("INPUT") != string::npos || data.find("OUTPUT") != string::npos)
        {
            cout << "[+] Identified Wire" << endl;
            char* data_c = new char[data.length() + 1]; // Hack solution to make the c_string param in Wire() work with a string 
            strcpy(data_c, data.c_str());
            Wire* newWire = new Wire(data_c); // construct new wire with data, constructor interprets data
            wires.push_back(newWire);
            cout << "[+] Added Wire ::: " << data << endl;
            delete[] data_c;
        }
        else
        {
            cout << "[+] Identified Gate" << endl;
            stringstream ss;
            string type_s, delay;
            int type_i, delay_i, i1, i2, o;
            Wire* i1_wp;
            Wire* i2_wp;
            Wire* o_wp; // VS complained if these didn't have their own lines. Not sure why that makes a difference
            ss << data;
            ss >> type_s >> delay >> i1 >> i2 >> o;
            if (type_s == "NOT") // Fill type_i for constructor
            {
                o = i2; // edge case housekeeping
                                       type_i =  NOT;
            }
            else if (type_s == "AND" ) type_i =  AND;
            else if (type_s == "OR"  ) type_i =   OR;
            else if (type_s == "XOR" ) type_i =  XOR;
            else if (type_s == "NAND") type_i = NAND;
            else if (type_s == "NOR" ) type_i =  NOR;
            else if (type_s == "XNOR") type_i = XNOR;
        
            ss << delay;
            ss >> delay_i >> type_s; // reuse type to save like a whole byte of memory

            for (int i = 0; i < wires.size(); i++) // fill Wire* data types for constructor
            {
                if (wires.at(i)->getIndex() == i1) i1_wp = wires.at(i); // all if statements without else to protect against improper handling of gates with both inputs tied to the same wire
                if (wires.at(i)->getIndex() == i2) i2_wp = wires.at(i);
                if (wires.at(i)->getIndex() == o ) o_wp  = wires.at(i);
            }

            Gate* newGate = new Gate(type_i, delay_i, i1_wp, i2_wp, o_wp);
        }
    }
    circuitFile.close();
    
    // Load in vector data

    while (!vectorFile.eof()) 
    {
        getline(vectorFile, data);
        events.push_back(data);
        cout << "[+] Added event ::: " << data << endl;
    }
    vectorFile.close();
    // Evaluate and print circuit behavior
    auto it = events.begin();
    int time = 0;
    for (int i = 0; i < wires.size(); i++) wires.at(i)->setValue(UNKNOWN); // Make sure all wires start unknown, 0-time wires will be handled by the regular logic just fine
    while (it != events.end())
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
        stringstream ss;
        string io;
        char* name;
        int timeChanged, val;
        ss << data;
        ss >> io >> name >> timeChanged >> val;
        // Add history as neccessary
        for (int i = 0; i < wires.size(); i++)
        {
            int histLen = timeChanged - time;
            wires.at(i)->appendHist(wires.at(i)->getValue(), histLen); // Rewrite history to serve my purposes... Append history of proper length to wires
        }
        // For all wires in wires<>, check for an event
        for (int i = 0; i < wires.size(); i++)
        {
            Wire* wire = wires.at(i);
            if (*(wire->getName()) == *name)
            {
                // Change status appropriately
                wire->setValue(val);
                // Evaluate affected gates and construct new events as necessary
                vector<Gate*> drives = wire->getDrives();
                for (int j = 0; j < drives.size(); j++)
                {
                    int newEvTime = time + drives.at(j)->getDelay();
                    string event = "OUTPUT ";
                    event.append(drives.at(j)->getOutput()->getName());
                    event.append(" ");
                    event.append(to_string(newEvTime));
                    event.append(" ");
                    event.append(to_string(drives.at(j)->evaluate()));
                    // Step through events and add when appropriate
                    int k = 0;
                    for (auto babyIt = events.begin(); babyIt != events.end(); babyIt++, k++)
                    {
                        stringstream ss;
                        string temp1, temp2;
                        int oldEvTime, temp3;
                        ss << events.at(k);
                        ss >> temp1 >> temp2 >> oldEvTime >> temp3;
                        if (oldEvTime > newEvTime)
                        {
                            events.insert(babyIt, event);
                            cout << " [+] Added event ::: " << event << endl;
                        }
                    }
                }
            }
        }
        it++; // advance to next event
    }

    // Do the print history thingy

    for (int i = 0; i < wires.size(); i++)
    {
        wires.at(i)->printHistory();
        std::cout << std::endl;
    }
    return 0;
}