//
// lifeform.h, Provenaz Clarence, Royer Yann
//

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <vector>
#include <sstream>
#include "shape.h"
#include "constantes.h"


class LifeForm{
public:
    LifeForm(S2d position, int age);
    bool getInitSuccess() const;
    void update();

protected:
    S2d position_;
    unsigned age_;
    bool initSuccess;

    virtual void writeFile(std::ofstream& file) const ;
};

class Alg:public LifeForm{
public:
    Alg(S2d position, int age); //constructor
    void writeFile(std::ofstream& file) const ;
    void display() const;
    bool isTooOld() const;
};

class Cor:public LifeForm{
public:
    Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
        const std::vector<Segment>& segs);

    unsigned getId() const;
    bool collisionCheck(const Cor& otherCor)const;
    void writeFile(std::ofstream &file) const ;
    void display() const;

private:
    unsigned id_;
    Status_cor status_;
    Dir_rot_cor dir_;
    Status_dev statusDev_;
    unsigned nbSeg_;

    std::vector<Segment> segments_;

    const std::vector<Segment>& getSegments()const;
};

class Sca:public LifeForm{
public:
    Sca(S2d position, int age, int radius, int status, int targetId); //constructor

    unsigned getTarget() const;
    Status_sca getStatus() const;
    void writeFile(std::ofstream &file) const ;
    void display() const;
private:
    unsigned radius_;
    Status_sca status_;
    unsigned targetId_;
};

Alg readAlg(std::istringstream& line);
Cor readCor(std::ifstream& file);
Sca readSca(std::istringstream& line);

std::istringstream nextLine(std::ifstream& file);


#endif
