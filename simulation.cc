//
// Created by Clarence Provenaz on 13.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>

#include "simulation.h"
#include "shape.h"
#include "message.h"

enum State{NBALG,ALGS,NBCOR,CORS,NBSCA,SCAS};

bool readFile(char* fileName, int& nbAlg, int& nbCor, int& nbSca);

Simulation::Simulation(char * inputFile) {
    std::cout<< inputFile<< std::endl;
    readFile(inputFile, nbAlg, nbCor, nbSca);
    nbSim = 0 ;
}


void readFile(char* fileName, int& nbAlg, int& nbCor, int& nbSca){
    string line;
    ifstream file(filename);
    if(!fichier.fail()) exit(1);
    while(getline(fichier >> ws,line)){
        if(line[0]=='#') continue;
        read_ligne(line);
    }
    cout << "fin de la lecture" << endl;
}
