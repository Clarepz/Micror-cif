//
// simulation.cc, Provenaz Clarence 100%, Royer Yann 0%
// conventions ok

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include "simulation.h"
#include "message.h"

using namespace std;

static default_random_engine randomEngine ;

Simulation::Simulation(): nbSim(0), nbCor(0), nbSca(0), nbAlg(0), initSuccess(true) {
    randomEngine.seed(1);
}

Simulation::Simulation(char* inputFile): nbSim(0) {
    randomEngine.seed(1);

    initSuccess = readFile(inputFile) and corIdUnicityCheck() and
                  corCollisionCheck() and scaTargetCheck();

    if(initSuccess) {
        std::cout << message::success();
    }
    else {
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

    for(auto &anAlg : algs ) {
        anAlg.writeFile(file);
    }

    file<<nbCor<<endl;

    for(auto &aCor : cors) {
        aCor.writeFile(file);
    }

    file<<nbSca<<endl;

    for(auto &aSca : scas) {
        aSca.writeFile(file);
    }

}

void Simulation::display() const {
    for(auto const& alifeForm : algs) {
        alifeForm.display();
    }
    for(auto const& alifeForm : cors) {
        alifeForm.display();
    }
    for(auto const& alifeForm : scas) {
        alifeForm.display();
    }
}

void Simulation::update(bool algBirthOn) {
    nbSim++;
    bool isDead = false;

    for(int i(0); i<nbAlg; i++) {
        isDead = false;
        algs[i].update(isDead);
        if(isDead){
            algs.erase(algs.begin()+i);
            nbAlg--;
        }
    }

    if(algBirthOn) {
        //create random alg
        bernoulli_distribution randomBool(alg_birth_rate);
        uniform_int_distribution<unsigned> randomCoordinate(1,dmax-1);
        if(randomBool(randomEngine)) {
            nbAlg++;
            double x = randomCoordinate(randomEngine);
            double y = randomCoordinate(randomEngine);
            S2d randomPosition = {x,y};
            algs.emplace_back(randomPosition,1);
        }
    }

    for(int i=0; i<nbCor; i++) {
        bool coralIsDead(false), noScavengerFree(false);
        cors[i].update(cors, algs, coralIsDead);
        if(coralIsDead and !cors[i].isIdAllocated()) allocateTargetToScavenger(cors[i]);
        nbAlg = algs.size(); //in case some algs died
        nbCor = cors.size(); //in case of repro
    }
//traiter coral
    bool scaTooOld(false), corDestroy(false), scaBirth(false);
    for(int i(0); i<nbSca; i++) {
        scas[i].update(scaTooOld, corDestroy, scaBirth, *findCorById(scas[i].getTarget()));
        if(scaTooOld) {
            scas.erase(scas.begin()+i);
            nbSca--;
        }
        if(corDestroy) {
            int id=scas[i].getTarget();
            for(int j(0); j<nbCor; ++j) {
                if(cors[j].getId()==id) {
                    scas[i].setStatus(corDestroy);
                    killCoral(j);
                    j=nbCor;//pour quitter la boucle for
                }
            }
        }
        if(scaBirth) {
            scas.emplace_back(corLastSegmentById(scas[i].getTarget()));
            nbSca++;
            scaBirth=false;
        }
    }
}

void Simulation::updateNbSca () {

    nbSca=scas.size();
}

bool Simulation::readFile(char* fileName) {

    ifstream file(fileName);
    if(file.fail()) return false;

    istringstream line = nextLine(file);
    line>>nbAlg;
    for(int i=0; i<nbAlg; i++) {
        line = nextLine(file);
        Alg anAlg = readAlg(line);
        if(!anAlg.getInitSuccess()) {
            return false;
        }
        algs.push_back(anAlg);
    }

    line = nextLine(file);
    line>>nbCor;
    for(int i=0; i<nbCor; i++) {
        Cor aCor(readCor(file));
        if(!aCor.getInitSuccess()) {
            return false;
        }
        cors.push_back(aCor);
    }

    line = nextLine(file);
    line>>nbSca;
    for(int i=0; i<nbSca; i++) {
        line = nextLine(file);
        Sca aSca(readSca(line));
        if(!aSca.getInitSuccess()) {
            return false;
        }
        scas.push_back(aSca);
    }
    return true;
}

bool Simulation::corIdUnicityCheck() const {
    //for all cor check with the ones before
    for(int i = 1; i < nbCor; i++) {
        for(int k = 0; k < i; k++) {
            if(cors[k].getId()==cors[i].getId()) {
                std::cout << message::lifeform_duplicated_id(cors[i].getId());
                return false;
            }
        }
    }
    return true;
}

bool Simulation::corCollisionCheck() const {
    for(int i=0; i<nbCor; i++) {
        for(int k = 0; k<= i ;k++) {
            if(!cors[i].collisionCheck(cors[k])) {
                return false;
            }
        }
    }
    return true;
}

bool Simulation::scaTargetCheck() const {
    for(auto sca : scas) {
        unsigned target = sca.getTarget();
        if(sca.getStatus()==EATING) {
            bool targetExists(false);
            for(auto const & cor : cors) {
                if(cor.getId()==target) {
                    targetExists = true;
                }
            }
            if(!targetExists) {
                std::cout << message::lifeform_invalid_id(target);
                return false;
            }
        }
    }
    return true;
}

Cor* Simulation::findCorById(int id) {
    for(int i(0); i<nbCor; ++i) {
        if(cors[i].getId()==id) return(&cors[i]);
    }
}

Segment Simulation::corLastSegmentById(int id) {
    for(int i(0); i<nbCor; ++i) {
        if(cors[i].getId()==id) return(cors[i].getLastSegment());
    }
}

void Simulation::killCoral(int index) {
    cors[nbCor-1].swapCoral(cors[index]);
    cors.pop_back();
    nbCor--;
}

void Simulation::allocateTargetToScavenger(Cor deadCoral) {
    S2d segLastPoint=deadCoral.getLastSegmentSecPoint();
    double distance_ = 356; // > 256*sqrt(2) to be sure it's the largest
    int index=-1;
    for(int i(0); i<nbSca; i++) {
        double testDistance=distance(segLastPoint, scas[i].getPosition());
        if(testDistance<=distance_) {
            if(scas[i].getStatus()==FREE) {
                distance_=testDistance;
                index=i;
            }
            /*Si deux coraux meurent en meme temps : else {
                if(distance(scas[i].getPosition(),
                            corLastSegmentById(scas[i].getTarget()).getSecPoint())
                   < testDistance) {
                    distance_=testDistance;
                    index=i;
                }
            }*/
        }
    }
    if(distance_!=356) {
        scas[index].setTarget(deadCoral.getId());
        deadCoral.setAllocatedId(true);
    }
    //if no scavengers are free the fonction will try again on next update

}
