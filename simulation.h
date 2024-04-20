//
// simulation.h, Provenaz Clarence, version 1
//


#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"

class Simulation{
public:
    Simulation();
    Simulation(char* inputFile);
    void saveAs(std::string const & fileName) const;
    void display() const ;
    void update(bool algBirthOn);
    int getNbSim() {return nbSim;}
    int getNbAlg() {return nbAlg;}
    int getNbCor() {return nbCor;}
    int getNbSca() {return nbSca;}
private:
    int nbSim;
    int nbAlg;
    int nbCor;
    int nbSca;

    bool initSuccess;

    std::vector<Alg> algs;
    std::vector<Cor> cors;
    std::vector<Sca> scas;

    bool readFile(char* fileName);

    bool corIdUnicityCheck() const;
    bool corCollisionCheck() const;
    bool scaTargetCheck() const;


};

#endif
