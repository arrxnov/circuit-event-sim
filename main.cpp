#include "Gate.h"
#include <fstream>
typedef unsigned int ui; // unsigned i realize in retrospect doesn't actually make a difference, but it is a working solution atm, so I'm not moving them
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
    vector<string> events; // this was a mistake, though not the biggest one
    string type;
    string data;
    
    // Load in circuit data and construct

    getline(circuitFile, type); // Grab irrelevant data from top
    while (!circuitFile.eof())
    {
        getline(circuitFile, data);
        if (data.find("INPUT") != string::npos || data.find("OUTPUT") != string::npos)
        {
            char* data_c = new char[(int)(data.length() + 1)]; // Hack solution to make the c_string param in Wire() work with a string
            strcpy(data_c, data.c_str());
            Wire* newWire = new Wire(data_c); // construct new wire with data, constructor interprets data
            wires.push_back(newWire);
        }
        else if (data.find("NOT") != string::npos || data.find("AND") != string::npos ||
        data.find("OR") != string::npos || data.find("XOR") != string::npos || 
        data.find("NAND") != string::npos || data.find("NOR") != string::npos ||
        data.find("XNOR") != string::npos) // Because trailing lines in files... Reasons
        {
            stringstream ss;
            string type_s, delay, junk;
            int type_i, delay_i, i1, i2, o;
            Wire* i1_wp = nullptr;
            Wire* i2_wp = nullptr;
            Wire* o_wp  = nullptr; // VS complained if these didn't have their own lines. Not sure why that makes a difference
            ss << data;
            ss >> std::skipws >> type_s >> delay >> i1 >> i2 >> o;
            if (type_s ==       "NOT") // Fill type_i for constructor
            {
                                       o = i2; // edge case housekeeping
                                       type_i =  NOT;
            }
            else if (type_s ==  "AND") type_i =  AND;
            else if (type_s ==   "OR") type_i =   OR;
            else if (type_s ==  "XOR") type_i =  XOR;
            else if (type_s == "NAND") type_i = NAND;
            else if (type_s ==  "NOR") type_i =  NOR;
            else if (type_s == "XNOR") type_i = XNOR;

            // Store the bit before ns in delay_i to be used by the rest of the logic
            // get string from delay, store the int before ns in delay_i
            char values[3];
            int i;
            int counter = 0;
            for(i = 0; i < 3; i++)
            {
                if(!isalpha(delay[i])) values[i] = delay[i];
                else break;
                counter++;
            }
            if(counter == 1) values[1] = '\0';
            else if(counter == 2) values[2] = '\0';
            delay_i = stoi(values);
            delay_i--; // Quick fix for delay issues
            // Test for uninputted wires connecting gates
            bool inWires1, inWires2, inWiresO = false;
            for (int j = 0; j < wires.size(); j++) if (i1 == wires.at(j)->getIndex()) inWires1 = true;
            for (int j = 0; j < wires.size(); j++) if (i2 == wires.at(j)->getIndex()) inWires2 = true;
            for (int j = 0; j < wires.size(); j++) if (o == wires.at(j)->getIndex()) inWiresO = true;
            data = "OUTPUT ";
            char newName = 'A';
            for (int i = 0; i < wires.size(); i++) if (*(wires.at(i)->getName()) == newName) {
                i = 0;
                newName++;
            }
            data += newName;
            data += " ";
            if (!inWires1)
            {
                data += to_string(i1);
                char* data_c = new char[(int)(data.length() + 1)];
                strcpy(data_c, data.c_str());
                Wire* newWire = new Wire(data_c);
                wires.push_back(newWire);
                cout << data << endl;
            }
            if (!inWires2)
            {
                data += to_string(i2);
                char* data_c = new char[(int)(data.length() + 1)];
                strcpy(data_c, data.c_str());
                Wire* newWire = new Wire(data_c);
                wires.push_back(newWire);
                cout << data << endl;
            }
            if (!inWiresO)
            {
                data += to_string(o);
                char* data_c = new char[(int)(data.length() + 1)];
                strcpy(data_c, data.c_str());
                Wire* newWire = new Wire(data_c);
                wires.push_back(newWire);
                cout << data << endl;
            }
            for (int i = 0; i < wires.size(); i++) // fill Wire* data types for constructor
            {
                if (wires.at(i)->getIndex() == i1) i1_wp = wires.at(i);
                if (wires.at(i)->getIndex() == i2) i2_wp = wires.at(i); 
                if (wires.at(i)->getIndex() == o ) o_wp  = wires.at(i);
            }
            Gate* newGate = new Gate(type_i, delay_i, i1_wp, i2_wp, o_wp);
            i1_wp->editDrives(newGate, ADD);
            if (type_i != NOT) i2_wp->editDrives(newGate, ADD);
        }
    }
    circuitFile.close();
    
    // Load in vector data
    getline(vectorFile, data); // Remove first line of datafile
    while (!vectorFile.eof()) 
    {
        getline(vectorFile, data);
        if (data == "") continue; // protect against empty lines
        events.push_back(data);
    }
    vectorFile.close();
    // Evaluate and print circuit behavior
    int time = 0; // Keeps track of ns timestamp for execution
    for (int i = 0; i < wires.size(); i++) wires.at(i)->setValue(UNKNOWN); // Make sure all wires start unknown, 0-time wires will be handled by the regular logic just fine
    for (int i = 0; i < events.size(); i++)
    {
        // Evaluate() events and add events appropriately
        // Ex.: OUTPUT 4 1 1 :: Sets wire 4 high at time 1
        stringstream ss; // stream for parsing
        string io; // string for useless leading data
        char name_cnp; // read in a char
        char* name_p = nullptr; // and convert to a pointer
        int timeChanged, val = 0; 
        ss << events.at(i);
        ss >> std::skipws >> io >> name_cnp >> timeChanged >> val;
        name_p = new char;
        *name_p = name_cnp;
        name_p[1] = '\0'; // string handling
        // Add history as neccessary
        for (int j = 0; j< wires.size(); j++)
        {
            int histLen = timeChanged - time;
            wires.at(j)->appendHist(wires.at(j)->getValue(), histLen); // Rewrite history to serve my purposes... Append history of proper length to wirestim
        }
        // For all wires in wires<>, check for an event
        for (int j = 0; j < wires.size(); j++)
        {
            Wire* wire = wires.at(j);
            if (*(wire->getName()) == *name_p)
            {
                // Change status appropriately
                wire->setValue(ui(val)); // Set value first
                // Evaluate affected gates and construct new events as necessary
                vector<Gate*> drives = wire->getDrives(); //  Check for affected driven gates
                for (int k = 0; k < drives.size(); k++)
                {
                    int newEvTime = time + drives.at(k)->getDelay();
                    bool added = false;
                    string event = "OUTPUT "; // Is this even relevant? Not in this implementation...
                    event.append(drives.at(k)->getOutput()->getName()); // Get output wire and set value
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
                            added = true;
                            break;
                        }
                    }
                    if (!added) events.insert(events.end(), event);
                    time = timeChanged;
                }
            }
        }

        if (time > 90) break;
    }
    
    // Do the print history thingy

    for (int i = 0; i < wires.size(); i++)
    {
        wires.at(i)->appendHist(wires.at(i)->getValue(), 1);
        wires.at(i)->printHistory();
        std::cout << std::endl;
    }
    cout << "[ Time >>>> 0    5    10   15   20   25" << endl;
    return 0;
}