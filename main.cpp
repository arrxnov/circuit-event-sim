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
            // cout << "[+] Finished making new pointer" << endl;
            strcpy(data_c, data.c_str());
            // cout << "[+] Finished copy: " << data_c << endl;
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
            string type_s, delay, junk;
            int type_i, delay_i, i1, i2, o;
            Wire* i1_wp = nullptr;
            Wire* i2_wp = nullptr;
            Wire* o_wp = nullptr; // VS complained if these didn't have their own lines. Not sure why that makes a difference
            ss << data;
            cout << data << endl;
            ss >> std::skipws >> type_s >> delay >> i1 >> i2 >> o;
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

            ss.str("");
            ss << delay;
            // Store the bit before ns in delay_i to be used by the rest of the logic
            // get string from delay, store the int before ns in delay_i
            char values[3];
            int i;
            int counter = 0;
            for(i = 0; i < 3; i++)
            {
                if(!isalpha(delay[i]))
                {
                    values[i] = delay[i];
                }
                else break;
                counter++;
            }

            if(counter == 1)
            {
                values[1] = '\0';
            }
            else if(counter == 2)
            {
                values[2] = '\0';
            }
            delay_i = stoi(values);
            
            for (int i = 0; i < wires.size(); i++) // fill Wire* data types for constructor
            {
                if (wires.at(i)->getIndex() == i1) i1_wp = wires.at(i); // all if statements without else to protect against improper handling of gates with both inputs tied to the same wire
                if (wires.at(i)->getIndex() == i2) i2_wp = wires.at(i);
                if (wires.at(i)->getIndex() == o ) o_wp  = wires.at(i);
            }

            Gate* newGate = new Gate(type_i, 3, i1_wp, i2_wp, o_wp); // DELAY FORCED TO 3 CHANGEME LATER
            cout << "[+] New gate at " << newGate << " with driving wire " << i1_wp->getName();

            if (type_i != NOT) cout << " and " << i2_wp->getName();
            cout << endl;
            cout << "::: Delay: " << delay_i << endl;
            
            i1_wp->editDrives(newGate, ADD);
            if (type_i != NOT) i2_wp->editDrives(newGate, ADD);
        }
        else {} // do nothing lol
    }
    circuitFile.close();
    
    // Load in vector data
    getline(vectorFile, data); // Remove first line of datafile
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
    // cout << "[+] Finished Xing all wires" << endl;
    for (int i = 0; i < events.size(); i++)
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
        // cout << "[+] Starting loop " << i << endl;
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
        for (int j = 0; j< wires.size(); j++)
        {
            int histLen = timeChanged - time;
            // cout << "[+] Length of time: " << histLen << endl;
            wires.at(j)->appendHist(wires.at(j)->getValue(), histLen); // Rewrite history to serve my purposes... Append history of proper length to wirestim
            cout << "[+] Appended history of len " << histLen << " of value " << val << " to " << wires.at(j)->getName() << endl;
        }
        // For all wires in wires<>, check for an event
        for (int j = 0; j < wires.size(); j++)
        {
            Wire* wire = wires.at(j);
            if (*(wire->getName()) == *name_p)
            {
                // Change status appropriately
                wire->setValue(val);
                // Evaluate affected gates and construct new events as necessary
                vector<Gate*> drives = wire->getDrives();
                for (int k = 0; k < drives.size(); k++)
                {
                    int newEvTime = time + drives.at(k)->getDelay();
                    bool added = false;
                    // cout << "[+] Current time: " << time << ", New Event Time: " << newEvTime << endl;
                    string event = "OUTPUT "; // Is this even relevant? Not in this implementation...
                    event.append(drives.at(k)->getOutput()->getName());
                    event.append(" ");
                    event.append(to_string(newEvTime));
                    event.append(" ");
                    event.append(to_string(drives.at(k)->evaluate()));
                    // Step through events and add when appropriate
                    int l = 0;
                    for (auto babyIt = events.begin(); babyIt != events.end(); babyIt++, l++)
                    {
                        stringstream ss;
                        string temp1, temp2;
                        int oldEvTime, temp3;
                        ss << events.at(l);
                        ss >> temp1 >> temp2 >> oldEvTime >> temp3;
                        if (oldEvTime > newEvTime) // I drew a picture to write this part. This means there is roughly a 20-30% HIGHER chance that it is in fact functional
                        {
                            events.insert(babyIt, event);
                        }
                    }
                    if (!added) 
                    {
                        cout << "[+] Attempting to insert at end" << endl;
                        events.insert(events.end(), event);
                    }
                    cout << "[+] Added event ::: " << event << endl;
                    time = timeChanged;
                }
            }
        }
    }

    for (int i = 0; i < events.size(); i++)
    {
        cout << events.at(i) << endl;
    }
    
    // Do the print history thingy

    for (int i = 0; i < wires.size(); i++)
    {
        wires.at(i)->printHistory();
        std::cout << std::endl;
        
    }
    std::cout << "[ Time >> 0    5    10   15   20   25" << std::endl;
    return 0;
}