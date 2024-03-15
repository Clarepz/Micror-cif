//
// Created by Clarence Provenaz on 13.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>


#include "simulation.h"
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
void Simulation::readLine(string& line){
    istringstream lineStream(line);

    enum ReadStatus{NBALG,ALGS,NBCOR,CORS,NBSCA,SCAS};

    static ReadStatus state(NBALG); //initial state
    //std::cout<<line<<std::endl;

    switch (state) {
        case NBALG:
            lineStream>>nbAlg;
            if(nbAlg == 0) state = NBCOR;
            else state = ALGS;
            break;
        case ALGS:
            for(int i=0; i < nbAlg ; i++){
                S2d pos;
                int age;
                lineStream >> pos.x >> pos.y >> age;
                Alg anAlg(pos,age);
                algs.push_back(anAlg);
            }
            state=NBCOR;
            break;
        case NBCOR:
            exit(0);
    }
}