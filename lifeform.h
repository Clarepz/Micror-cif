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
    //constructeur
    LifeForm(S2d position, int age);
    //getters
    bool getInitSuccess() const {return initSuccess;}
    S2d getPosition() const {return position_;}

protected:
    S2d position_;
    unsigned age_;
    bool initSuccess;
    virtual void writeFile(std::ofstream& file) const;
};

class Alg:public LifeForm {
public:
    Alg(S2d position, int age);
    void writeFile(std::ofstream& file) const;
    void display() const;
    void update(bool &dead);
};

class Cor:public LifeForm {
public:
    //constructeur
    Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
        const std::vector<Segment>& segs);

    //getters
    unsigned getId() const {return id_;}
    Status_cor getStatus() {return status_;}
    S2d getLastSegmentSecPoint() const {return(segments_[nbSeg_-1].getSecPoint());}
    bool isIdAllocated () const {return allocatedId;}
    const std::vector<Segment>& getSegments() const {return segments_;}

    //méthodes publiques
    bool collisionCheck(const Cor& otherCor) const;
    void writeFile(std::ofstream &file) const;
    void display() const;
    void update(const std::vector<Cor>& cors, const std::vector<Alg>& algs,
				std::vector<Cor>& babyCor, int& deadAlgIndex);
    void setAllocatedId(bool allocatedId);
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
    void update(bool &scaTooOld, bool &corDestroy, bool &scaBirth, S2d &newScaPos,
				Cor &target);

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
