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
    void update(bool &dead);
};

class Cor:public LifeForm {
public:
    Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
        const std::vector<Segment>& segs);
    unsigned getId() const;
    unsigned getNbCor() const {return(nbSeg_);}
    S2d const getLastSegmentSecPoint() {return(segments_[nbSeg_-1].getSecPoint());}
    Segment getLastSegment() {return(segments_[nbSeg_-1]);}
    bool collisionCheck(const Cor& otherCor) const;
    void writeFile(std::ofstream &file) const;
    void display() const;
    void update(std::vector<Cor>& cors, std::vector<Alg>& algs);
    void swapSegment(Cor &coral);
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

    bool shouldEat(const Alg& anAlg, double &angleToAlg) const ;
    void extend();
    Cor repro(unsigned id);
};

class Sca:public LifeForm {
public:
    Sca(S2d position, int age, int radius, int status, int targetId); //constructor
    Sca(Segment seg); //constructor for reproduction
    unsigned getTarget() const;
    Status_sca getStatus() const;
    void writeFile(std::ofstream &file) const ;
    void display() const;
    //corDestroy let simulation when to destroy the coral
    void update(bool &scaTooOld, bool &corDestroy, bool &scaBirth, Cor &target);
    void endEating(bool &corDestroy);

private:
    unsigned radius_;
    Status_sca status_;
    unsigned targetId_;
    bool onTarget;
};

Alg readAlg(std::istringstream& line);
Cor readCor(std::ifstream& file);
Sca readSca(std::istringstream& line);

std::istringstream nextLine(std::ifstream& file);


#endif
