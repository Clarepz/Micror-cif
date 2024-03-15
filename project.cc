#include <iostream>
#include "simulation.h"

using namespace std;

int main(int argc, char* argv[]){
    std::cout<< "hello";
    if(argc != 2) exit(0);
    Simulation simulation(argv[1]);

    return 0;
}
