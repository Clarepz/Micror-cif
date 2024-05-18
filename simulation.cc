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

	//partie algues
    int i=0;
    while (i<nbAlg){
        isDead = false;
        algs[i].update(isDead);
        if(isDead){
            algs.erase(algs.begin()+i);
            nbAlg--;
        }else i++;
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

	//partie coraux
    vector <Cor*> freeDeadCor;
    for(int i=0; i<nbCor; i++) {
        vector<Cor> babyCor;
        cors[i].update(cors, algs, babyCor);
        if(cors[i].getStatus() == DEAD and !cors[i].isIdAllocated()) 
			freeDeadCor.push_back(&cors[i]);
        nbAlg = algs.size(); //in case some algs died
        if(!babyCor.empty()) cors.push_back(babyCor[0]);
    }
    nbCor = cors.size(); //in case of repro (out of the loop to not update new cor)
	
	//partie scavengers
	allocateTargetToScavenger(freeDeadCor);
	vector<Sca> newScas;
    for(int i(0); i<nbSca; i++) {
		bool scaTooOld(false), corDestroy(false), scaBirth(false);
		S2d newScaPos; //in case a reproduction take place
        scas[i].update(scaTooOld, corDestroy, scaBirth, newScaPos,
					   *findCorById(scas[i].getTarget()));
        if(corDestroy) scaIsDoneEatingCoral(scas[i].getTarget(), i);
        if(scaBirth) newScas.emplace_back(newScaPos);
        if(scaTooOld) {
			killScavenger(i, scas);
			i--;//sans cette ligne pas d'update pour l'anciennement dernier scavenger
		}
    }
    addToScas(newScas);
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
        setAllocatedIdOpenningFiles();
    }
    return true;
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

void Simulation::allocateTargetToScavenger(const std::vector<Cor*> &freeDeadCor) {
	if(freeDeadCor.empty()) return;
    //if only one coral's dead he's eaten b the nearest scavenger
    if(freeDeadCor.size()==1) {
        double distanceMin=365;
        int index(0);
        for (int i(0); i<nbSca; i++) {
            double newDistance=distance(scas[i].getPosition(),
                                        scas[i].getPosition());
            if(distanceMin>newDistance and scas[i].getStatus()==FREE) {
                distanceMin=newDistance;
                index=i;
            }
        }
        if(distanceMin!=365) {
            scas[index].setTarget(freeDeadCor[0]->getId());
            return;
        }
    }
    //otherwise the first one the vector is gonna eat the nearest coral
	for(int i(0); i<nbSca; i++) {
		if(scas[i].getStatus() == FREE) {
			double distanceMin=365; //majore l'ensemble des distances du carré
			unsigned index; //stockage de la position du corail le plus proche
			for(int j(0); j<freeDeadCor.size(); j++) {
				if(!freeDeadCor[j]->isIdAllocated()) {
					double distanceTest=distance(scas[i].getPosition(),
												 freeDeadCor[j]->getLastSegmentSecPoint());
					if(distanceTest<distanceMin) {
						distanceMin=distanceTest;
						index=j;
					}
				}
			}
			if(distanceMin!=365) {
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
            killCoral(j);
            return;
		}
	}
}

void Simulation::addToScas(const std::vector<Sca> newScas) {
	for(int i(0); i<newScas.size(); i++) {
		scas.push_back(newScas[i]);
		nbSca++;
	}
}

void Simulation::killCoral(int index) {
    cors[index].swapToKill(cors[nbCor-1]);
    cors.pop_back();
    nbCor--;
}

void Simulation::killScavenger(int index, std::vector<Sca> &scavengers) {
	if(scavengers[index].getStatus() == EATING) 
		findCorById(scavengers[index].getTarget())->setAllocatedId(false);
    scavengers[index].swapToKill(scavengers[nbSca-1]);
    scavengers.pop_back();
    nbSca--;
}

void Simulation::killAlg(int index, std::vector<Alg> &algs) {
    algs[index].swapToKill(algs[nbAlg-1]);
    algs.pop_back();
    nbAlg--;
}
