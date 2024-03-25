//
// simulation.cc, Provenaz Clarence, version 1
//
#include <iostream>
#include <fstream>
#include <sstream>
#include "simulation.h"
#include "message.h"

using namespace std;

Simulation::Simulation(char * inputFile): nbSim(0){
    readFile(inputFile);
    corIdUnicityCheck();
    corCollisionCheck();
    scaTargetCheck();
    std::cout << message::success();
    exit(0);
}

void Simulation::readFile(char* fileName){
    ifstream file(fileName);
    if(file.fail()) exit(EXIT_FAILURE);

    istringstream line = nextLine(file);
    line>>nbAlg;
    for(int i=0; i<nbAlg; i++){
        line = nextLine(file);
        algs.push_back(readAlg(line));
    }

    line = nextLine(file);
    line>>nbCor;
    for(int i=0; i<nbCor; i++){
        cors.push_back(readCor(file));
    }

    line = nextLine(file);
    line>>nbSca;
    for(int i=0; i<nbSca; i++){
        line = nextLine(file);
        scas.push_back(readSca(line));
    }

}




bool Simulation::corIdUnicityCheck() const {
    //for all cor check with the ones before
    for(int i = 1; i < nbCor; i++){
        for(int k = 0; k < i; k++){
            if(cors[k].getId()==cors[i].getId()){
                std::cout << message::lifeform_duplicated_id(cors[i].getId());
                exit(EXIT_FAILURE);
                //return false;
            }
        }
    }
    return true;
}

bool Simulation::corCollisionCheck() const {
    for(int i=0; i<nbCor; i++){
        for(int k = 0; k<= i ;k++){
            cors[i].collisionCheck(cors[k]);
        }
    }
}

bool Simulation::scaTargetCheck() const {
    for(auto sca : scas){
        int target = sca.getTarget();
        if(sca.getStatus()==MANGE){
            for(auto cor : cors){
                if(cor.getId()==target) return true;
            }
            std::cout << message::lifeform_invalid_id(target);
            exit(EXIT_FAILURE);
        }
    }
}