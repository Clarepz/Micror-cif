//
// Created by Clarence Provenaz on 13.03.2024.
//


#ifndef MICRORECIF_SIMULATION_H
#define MICRORECIF_SIMULATION_H
#include "lifeform.h"
#include <string>
using namespace std;

class Simulation{
public:
    Simulation(char* inputFile); //constructeur lecture fichier
private:
    int nbSim;

    int nbAlg;
    int nbCor;
    int nbSca;

    vector<Alg> algs;
    vector<Cor> cors;
    vector<Sca> scas;

    void readFile(char* fileName);

    void readAlg(ifstream& file);
    void readCor(ifstream& file);
    void readSca(ifstream& file);

};
#endif //MICRORECIF_SIMULATION_H
