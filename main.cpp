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
    
    // Load in circuit data and construct

    queue<Wire*> events;

    // Load in vector data and construct

    while (!events.empty())
    {
        // Evaluate() events and add events appropriately. Pop front after done etc, etc.
    }

    return 0;
}