#include <iostream>
#include "simulation.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2) exit(0);
    Simulation simulation(argv[1]); //initialise une simulation a partir du fichier d'entree

    return 0;
}
//