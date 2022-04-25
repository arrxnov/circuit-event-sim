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
    string type;
    string data;
    
    // Load in circuit data and construct

    getline(circuitFile, type); // Grab irrelevant data from top
    while (!circuitFile.eof())
    {
        getline(circuitFile, data);
        if (data.find("INPUT") != string::npos || data.find("OUTPUT") != string::npos)
        {
            cout << "[+] Identified Wire" << endl;
            char* data_c = new char[(int)(data.length() + 1)]; // Hack solution to make the c_string param in Wire() work with a string
            cout << "[+] Finished making new pointer" << endl;
            strcpy(data_c, data.c_str());
            cout << "[+] Finished copy: " << data_c << endl;
            Wire* newWire = new Wire(data_c); // construct new wire with data, constructor interprets data
            wires.push_back(newWire);
            cout << "[+] Added Wire ::: " << data << endl;
            // delete[] data_c;
            // cout << "[+] Finished delete" << endl;
        }
        else if (data.find("NOT") != string::npos || data.find("AND") != string::npos ||
        data.find("OR") != string::npos || data.find("XOR") != string::npos || 
        data.find("NAND") != string::npos || data.find("NOR") != string::npos ||
        data.find("XNOR") != string::npos) // Because trailing lines in files... Reasons
        {
            cout << "[+] Identified Gate" << endl;
            stringstream ss;
            string type_s, delay;
            int type_i, delay_i, i1, i2, o;
            Wire* i1_wp = nullptr;
            Wire* i2_wp = nullptr;
            Wire* o_wp = nullptr; // VS complained if these didn't have their own lines. Not sure why that makes a difference
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
            i1_wp->editDrives(newGate, ADD);
            if (type_i != NOT) i2_wp->editDrives(newGate, ADD);
        }
        else {} // do nothing lol
    }
    circuitFile.close();
    
    // Load in vector data

    while (!vectorFile.eof()) 
    {
        getline(vectorFile, data);
        if (data == "") continue; // protect against empty lines
        events.push_back(data);
        cout << "[+] Added event ::: " << data << endl;
    }
    vectorFile.close();
    // Evaluate and print circuit behavior
    int time = 0;
    for (int i = 0; i < wires.size(); i++) wires.at(i)->setValue(UNKNOWN); // Make sure all wires start unknown, 0-time wires will be handled by the regular logic just fine
    cout << "[+] Finished Xing all wires" << endl;
    for (int i = 0; i < events.size(); i++)
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
        cout << "[+] Starting loop " << i << endl;
        stringstream ss;
        string io;
        char name_cnp;
        char* name_p = nullptr;
        int timeChanged, val = 0;
        ss << events.at(i);
        cout << "[+] Finished streaming data: " << events.at(i) << endl;
        ss >> std::skipws >> io >> name_cnp >> timeChanged >> val;
        name_p = new char;
        *name_p = name_cnp;
        name_p[1] = '\0';
        cout << "[+] IO: " << io << ", Name: " << name_p << ", timeChanged: " << timeChanged << ", Value: " << val << endl; 
        // Add history as neccessary
        for (int i = 0; i < wires.size(); i++)
        {
            int histLen = timeChanged - time;
            cout << "[+] Length of time: " << histLen << endl;
            wires.at(i)->appendHist(wires.at(i)->getValue(), histLen); // Rewrite history to serve my purposes... Append history of proper length to wires
        }
        // For all wires in wires<>, check for an event
        for (int i = 0; i < wires.size(); i++)
        {
            Wire* wire = wires.at(i);
            if (*(wire->getName()) == *name_p)
            {
                // Change status appropriately
                wire->setValue(val);
                // Evaluate affected gates and construct new events as necessary
                vector<Gate*> drives = wire->getDrives();
                for (int j = 0; j < drives.size(); j++)
                {
                    int newEvTime = time + drives.at(j)->getDelay();
                    string event = "OUTPUT "; // Is this even relevant? Not in this implementation...
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
                        if (oldEvTime > newEvTime) // I drew a picture to write this part. This means there is roughly a 20-30% HIGHER chance that it is in fact functional
                        {
                            events.insert(babyIt, event);
                            cout << "[+] Added event ::: " << event << endl;
                        }
                    }
                }
            }
        }
    }

    // Do the print history thingy

    for (int i = 0; i < wires.size(); i++)
    {
        wires.at(i)->printHistory();
        std::cout << std::endl;
    }
    return 0;
}