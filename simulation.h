//
// Created by Clarence Provenaz on 13.03.2024.
//

#ifndef MICRORECIF_SIMULATION_H
#define MICRORECIF_SIMULATION_H

class Simulation{
public:
    Simulation(char* inputFile); //constructeur lecture fichier
private:
    int nbSim;
    int nbAlg;
    int nbSca;
    int nbCor;
};
#endif //MICRORECIF_SIMULATION_H
