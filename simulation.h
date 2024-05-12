//
// simulation.h, Provenaz Clarence, Royer Yann
// conventions ok


#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"

class Simulation {
public:
    Simulation();
    Simulation(char* inputFile);
    void saveAs(std::string const & fileName) const;
    void display() const;
    void update(bool algBirthOn);
    int getNbSim() const {return nbSim;}
    int getNbAlg() const {return nbAlg;}
    int getNbCor() const {return nbCor;}
    int getNbSca() const {return nbSca;}
    void updateNbSca ();
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
    void setAllocatedIdOpenningFiles();

    bool corIdUnicityCheck() const;
    bool corCollisionCheck() const;
    bool scaTargetCheck() const;
    Cor* findCorById (int id);
    Segment corLastSegmentById(int id);
    void allocateTargetToScavenger(const std::vector<Cor*> &freeDeadCor);
    void scaIsDoneEatingCoral(const int id, const unsigned indexSca);
    void addToScas(const std::vector<Sca> newScas);
    //les fonctions kill copient le dernier élément dans l'élément[index] et popBack
    void killCoral(int index);
    void killScavenger(int index, std::vector<Sca> &scavengers);
    void killAlg(int index, std::vector<Alg> &algs);

};

#endif
