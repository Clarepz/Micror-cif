//
// Created by Clarence Provenaz on 13.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>


#include "simulation.h"
#include "message.h"

Alg readAlg(istringstream& lineStream);
Cor readCor(istringstream& lineStream, bool isASeg = false);

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

    enum ReadStatus{NBALG,ALGS,NBCOR,COR,SEG,NBSCA,SCAS};

    static ReadStatus state(NBALG); //initial state
    static int i(0);
    //std::cout<<line<<std::endl;

    switch (state) {
        case NBALG:
            lineStream>>nbAlg;
            state = ALGS;
            break;
        case ALGS:
            if(i < nbAlg ){
                algs.push_back(readAlg(lineStream));
                i++;
            }else{
                i = 0;
                state=NBCOR;
            }
            break;
        case NBCOR:
            lineStream>>nbCor;
            state = ALGS;
            break;
        case COR:
            if(i < nbCor ){
                readCor();
                i++;
                state = SEG;
            }else{
                i = 0;
                state=NBCOR;
            }
            break;
    }
}

Alg readAlg(istringstream& lineStream){
    S2d pos;
    int age;
    lineStream >> pos.x >> pos.y >> age;
    Alg anAlg(pos,age);
    return anAlg;
}

Cor readCor(istringstream& lineStream, bool isASeg = false){

}