//
// Created by Clarence Provenaz on 13.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>


#include "simulation.h"
#include "shape.h"
#include "message.h"








Simulation::Simulation(char * inputFile) {
    std::cout<< inputFile<< std::endl;
    readFile(inputFile);
    nbSim = 0 ;
}


void Simulation::readFile(char* fileName){
    string line;
    ifstream file(fileName);
    if(file.fail()) exit(1);
    while(getline(file >> ws,line)){
        if(line[0]=='#') continue;
        readLine(line);
    }
}
void Simulation::readLine(string line){
    enum State{NBALG,ALGS,NBCOR,CORS,NBSCA,SCAS};

    std::cout<<line<<std::endl;
}