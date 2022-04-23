#include "Wire.h"
#include <fstream>
#include <queue>

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
           }
           else
           {
               
           }
    }

    // Load in vector data and construct

    while (true)
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
    }

    return 0;
}