#include "Wire.h"

int main(int argc, char** argv)
{   
    if (argc != 2)
    {
        std::cout << "[+] Usage: " << argv[0] << " <CIRCUIT FILE> <VECTOR FILE>" << std::endl;
        return 1;
    }
    return 0;
}