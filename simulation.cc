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

    initSuccess =
            readFile(inputFile) and
            corIdUnicityCheck() and
            corCollisionCheck() and
            scaTargetCheck();

    if(initSuccess) {
        std::cout << message::success();
    }else{
        nbCor = 0;
        nbAlg = 0;
        nbSca = 0;
        cors.clear();
        algs.clear();
        scas.clear();
    }
}

void Simulation::saveAs(std::string const & fileName) const {
    ofstream file(fileName);
    if(file.fail()) exit(EXIT_FAILURE);

    file<<nbAlg<<endl;
    for(auto anAlg : algs ){
        anAlg.writeFile(file);
    }
    file<<nbCor<<endl;
    for(auto aCor : cors){
        aCor.writeFile(file);
    }
    file<<nbSca<<endl;
    for(auto aSca : scas){
        aSca.writeFile(file);
    }

}

bool Simulation::readFile(char* fileName){

    ifstream file(fileName);
    if(file.fail()) return false;

    istringstream line = nextLine(file);
    line>>nbAlg;
    for(int i=0; i<nbAlg; i++){
        line = nextLine(file);
        Alg anAlg = readAlg(line);
        if(!anAlg.getInitSuccess()){
            return false;
        }
        algs.push_back(anAlg);
    }

    line = nextLine(file);
    line>>nbCor;
    for(int i=0; i<nbCor; i++){
        Cor aCor(readCor(file));
        if(!aCor.getInitSuccess()){
            return false;
        }
        cors.push_back(aCor);
    }

    line = nextLine(file);
    line>>nbSca;
    for(int i=0; i<nbSca; i++){
        line = nextLine(file);
        Sca aSca(readSca(line));
        if(!aSca.getInitSuccess()){
            return false;
        }
        scas.push_back(aSca);
    }
    return true;
}




bool Simulation::corIdUnicityCheck() const {
    //for all cor check with the ones before
    for(int i = 1; i < nbCor; i++){
        for(int k = 0; k < i; k++){
            if(cors[k].getId()==cors[i].getId()){
                std::cout << message::lifeform_duplicated_id(cors[i].getId());
                return false;
            }
        }
    }
    return true;
}

bool Simulation::corCollisionCheck() const {
    for(int i=0; i<nbCor; i++){
        for(int k = 0; k<= i ;k++){
            if(!cors[i].collisionCheck(cors[k])){
                return false;
            }
        }
    }
    return true;
}

bool Simulation::scaTargetCheck() const {
    for(auto sca : scas){
        unsigned target = sca.getTarget();
        if(sca.getStatus()==EATING){
            bool targetExists(false);
            for(auto const & cor : cors){
                if(cor.getId()==target){
                    targetExists = true;
                }
            }
            if(!targetExists){
                std::cout << message::lifeform_invalid_id(target);
                return false;
            }
        }
    }
    return true;
}