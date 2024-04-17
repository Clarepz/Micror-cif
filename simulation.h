//
// simulation.h, Provenaz Clarence, version 1
//


#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"

class Simulation{
public:
    Simulation(char* inputFile);
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
