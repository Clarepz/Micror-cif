//
// lifeform.h, Provenaz Clarence, Royer Yann verion 2
// 

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <vector>
#include <sstream>
#include "shape.h"
#include "constantes.h"


class LifeForm {
public:
    LifeForm(S2d position, int age);//constructeur
    bool getInitSuccess() const;
    S2d getPosition() const {return position_;}
    //virtual void update();

protected:
    S2d position_;
    unsigned age_;
    bool initSuccess;
    virtual void writeFile(std::ofstream& file) const;
};

class Alg:public LifeForm {
public:
    Alg(S2d position, int age); //constructor
    void writeFile(std::ofstream& file) const;
    void display() const;
    void swapToKill(const Alg &livingAlg); //copy livingCor in actual cor
    void update(bool &dead);
};

class Cor:public LifeForm {
public:
    Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
        const std::vector<Segment>& segs);
    unsigned getId() const;
    Status_cor getStatus() {return status_;}
    unsigned getNbCor() const {return(nbSeg_);}
    S2d const getLastSegmentSecPoint() {return(segments_[nbSeg_-1].getSecPoint());}
    Segment getLastSegment() {return(segments_[nbSeg_-1]);}
    bool isIdAllocated () const {return allocatedId;}
    bool collisionCheck(const Cor& otherCor) const;
    void writeFile(std::ofstream &file) const;
    void display() const;
    void update(const std::vector<Cor>& cors, std::vector<Alg>& algs,
				std::vector<Cor>& babyCor);
    void setAllocatedId(bool allocatedId);
    void swapToKill(Cor &livingCor); //copy livingCor in actual cor
    const std::vector<Segment>& getSegments() const;
    bool eaten(S2d &nextScaPos); //this fonction is called when a coral is eaten
    //return true when the coral's fully eaten

private:
    unsigned id_;
    Status_cor status_;
    Dir_rot_cor dir_;
    Status_dev statusDev_;
    unsigned nbSeg_;
    std::vector<Segment> segments_;
    bool allocatedId;

    bool shouldEat(const Alg& anAlg, double &angleToAlg) const ;
    void extend();
    Cor repro(unsigned id);
};

class Sca:public LifeForm {
public:
	//constructeurs
    Sca(S2d position, int age, int radius, int status, int targetId);
    Sca(S2d newScaPos); //celui-ci sert à la reproduction
    
    //getteurs
    int getTarget() const {return targetId_;}
    Status_sca getStatus() const {return status_;}
    
    //sets
    void setFree();
    void setTarget(int coralId);
    
    //méthodes pour la simulation
    void writeFile(std::ofstream &file) const ;
    void display() const;
    void swapToKill(const Sca &livingSca); //copy livingSca in actual sca
    void update(bool &scaTooOld, bool &corDestroy, bool &scaBirth, S2d &newScaPos,
				Cor &target);
    //corDestroy let simulation know when to destroy the coral

private:
    unsigned radius_;
    Status_sca status_;
    int targetId_;
    bool onTarget;
};

Alg readAlg(std::istringstream& line);
Cor readCor(std::ifstream& file);
Sca readSca(std::istringstream& line);

std::istringstream nextLine(std::ifstream& file);


#endif
