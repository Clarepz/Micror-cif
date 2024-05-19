//
// simulation.h, Provenaz Clarence, Royer Yann
// conventions ok


#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"

class Simulation {
public:
    //constructeurs
    Simulation();
    Simulation(char* inputFile);
    //fonctions pour le module gui
    void saveAs(std::string const & fileName) const;
    void display() const;
    void update(bool algBirthOn);
    //getters
    int getNbSim() const {return nbSim;}
    int getNbAlg() const {return nbAlg;}
    int getNbCor() const {return nbCor;}
    int getNbSca() const {return nbSca;}
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
    void createRandomAlg();
    void setAllocatedIdOpenningFiles();
    void updateTheAlgs(bool algBirthOn);
    void updateTheCorals();
    void updateTheScavengers();
    bool corIdUnicityCheck() const;
    bool corCollisionCheck() const;
    bool scaTargetCheck() const;
    Cor* findCorById (int id);
    //this fonction is called when a coral's dead to allocate his ID to scavengers
    void aCoralIsDead(const std::vector<Cor*> &freeDeadCor);
    void allocateTargetToScavenger(bool oneDead, const std::vector<Cor*> &freeDeadCor);
    void scaIsDoneEatingCoral(const int id, const unsigned indexSca);
};

#endif
