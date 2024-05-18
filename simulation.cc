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


template<typename LifeForm>
void kill(vector<LifeForm>& lifeForms, unsigned index);

template<typename LifeForm>
void kill(vector<LifeForm>& lifeForms, unsigned index){
    swap(lifeForms[index],lifeForms.back());
    lifeForms.pop_back();
}


//constructeurs
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


//méthodes pour le module gui
void Simulation::saveAs(std::string const & fileName) const {
    ofstream file(fileName);
    if(file.fail()) exit(EXIT_FAILURE);

    file<<to_string(nbAlg)<<endl;

    for(auto &anAlg : algs ) {
        anAlg.writeFile(file);
    }

    file<<to_string(nbCor)<<endl;

    for(auto &aCor : cors) {
        aCor.writeFile(file);
    }

    file<<to_string(nbSca)<<endl;

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

	//partie algue
    int i(0);
    while (i<nbAlg){
        isDead = false;
        algs[i].update(isDead);
        if(isDead){
            kill(algs,i);
            nbAlg--;
        }else i++;
    }

    if(algBirthOn) createRandomAlg();

	//partie corail
    vector <Cor*> freeDeadCor;
    for(int i=0; i<nbCor; i++) {
        vector<Cor> babyCor;
        int deadAlgIndex(-1);
        cors[i].update(cors, algs, babyCor, deadAlgIndex);
        if(deadAlgIndex != -1) kill(algs,deadAlgIndex);
        nbAlg = algs.size();

        if(cors[i].getStatus() == DEAD and !cors[i].isIdAllocated()) 
			freeDeadCor.push_back(&cors[i]);

        if(!babyCor.empty()) cors.push_back(babyCor[0]);
    }
    nbCor = cors.size(); //in case of repro (out of the loop to not update new cor)
    aCoralIsDead(freeDeadCor);
	
	//partie scavenger
	vector<Sca> newScas;
    for(int i(0); i<nbSca; i++) {
		bool scaTooOld(false), corDestroy(false), scaBirth(false);
		S2d newScaPos; //in case a reproduction take place
        scas[i].update(scaTooOld, corDestroy, scaBirth, newScaPos,
					   *findCorById(scas[i].getTarget()));
        if(corDestroy) scaIsDoneEatingCoral(scas[i].getTarget(), i);
        if(scaBirth) newScas.emplace_back(newScaPos);
        if(scaTooOld) {
            if(scas[i].getStatus() == EATING)
                findCorById(scas[i].getTarget())->setAllocatedId(false);
            kill(scas,i);
			i--;//sans cette ligne pas d'update pour l'anciennement dernier scavenger
		}
    }
    scas.insert(scas.end(),newScas.begin(),newScas.end());
    nbSca = scas.size();
}


//méthodes privates
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
        setAllocatedIdOpenningFiles();
    }
    return true;
}

void Simulation::createRandomAlg() {
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

void Simulation::setAllocatedIdOpenningFiles() {
	for(int i(0); i<nbSca; i++) {
		if(scas[i].getStatus() == EATING)
			findCorById(scas[i].getTarget())->setAllocatedId(true);
	}
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
    for(const auto& sca : scas) {
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

void Simulation::aCoralIsDead(const std::vector<Cor *> &freeDeadCor) {
    switch(freeDeadCor.size()) {
        case 0:
            return;
        case 1:
            allocateTargetToScavenger(true, freeDeadCor);
            return;
        default:
            allocateTargetToScavenger(false, freeDeadCor);
    }
}

void Simulation::allocateTargetToScavenger(bool oneDead,
                                           const vector<Cor*> &freeDeadCor) {
    double distanceMin = 365; //majore l'ensemble des distances du carré
    unsigned index; //stockage de la position du corail/scavenger le plus proche
    if(oneDead) {
        //if only one coral's dead he's eaten b the nearest scavenger
        for (int i(0); i < nbSca; i++) {
            double newDistance = distance(scas[i].getPosition(),
                                          scas[i].getPosition());
            if (distanceMin > newDistance and scas[i].getStatus() == FREE) {
                distanceMin = newDistance;
                index = i;
            }
        }
        if (distanceMin != 365) {
            scas[index].setTarget(freeDeadCor[0]->getId());
            freeDeadCor[0]->setAllocatedId(true);
        }
        return;
    }
    //otherwise the first one the vector is gonna eat the nearest coral
    for (int i(0); i < nbSca; i++) {
        if (scas[i].getStatus() == FREE) {
            for (int j(0); j < freeDeadCor.size(); j++) {
                if (!freeDeadCor[j]->isIdAllocated()) {
                    double distanceTest = distance(scas[i].getPosition(),
                                                   freeDeadCor[j]->getLastSegmentSecPoint());
                    if (distanceTest < distanceMin) {
                        distanceMin = distanceTest;
                        index = j;
                    }
                }
            }
            if (distanceMin != 365) {
                scas[i].setTarget(freeDeadCor[index]->getId());
                freeDeadCor[index]->setAllocatedId(true);
            }
        }
    }
}

void Simulation::scaIsDoneEatingCoral(const int corId, const unsigned indexSca) {
	for(int j(0); j<nbCor; ++j) {
		if(cors[j].getId()==corId) {
			scas[indexSca].setFree();
            swap(cors[j],cors.back());
            cors.pop_back();
            return;
		}
	}
}
