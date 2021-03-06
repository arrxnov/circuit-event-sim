#ifndef WIRE_H
#define WIRE_H
#define REMOVE 0
#define ADD 1
#define HIGH 1
#define LOW 0
#define UNKNOWN 2

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

class Gate;

class Wire
{
    private:
        unsigned int value;
        char* name;
        std::vector <Gate*> drives;
        int index;
        std::vector<int> history;
    public:
        Wire(char* n);
        void setValue(unsigned int v) { value = v; };
        void appendHist(int h, int time);
        void editDrives(Gate* g, int e);
        unsigned int getValue() const { return value; };
        char* getName() const { return name; };
        std::vector<Gate*> getDrives() const { return drives; };
        int getIndex() const { return index; };
        std::vector<int> getHistory() const { return history; };
        void printHistory() const;
};

#endif