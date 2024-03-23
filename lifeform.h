//
// Created by Clarence Provenaz on 15.03.2024.
//

#ifndef MICRORECIF_LIFEFORM_H
#define MICRORECIF_LIFEFORM_H

#include <vector>
#include <sstream>
#include "shape.h"
#include "constantes.h"


class LifeForm{
public:
    LifeForm(S2d position, int age);
protected:
    S2d position_;
    unsigned age_;
};

class Alg:public LifeForm{
public:
    Alg(S2d position, int age); //constructor
};

class Cor:public LifeForm{
public:
    Cor(S2d position, int age, unsigned id, int nbseg,const std::vector<Segment>& segs);
    unsigned getId() const;
    bool collisionCheck(const Cor& otherCor)const;
private:
    unsigned id_;
    unsigned nbSeg_;
    std::vector<Segment> segments_;

    const std::vector<Segment>& getSegments()const;
};

class Sca:public LifeForm{
public:
    Sca(S2d position, int age, int radius, int status, int targetId); //constructor

    unsigned getTarget() const;
    Statut_sca getStatus() const;
private:
    unsigned radius_;
    Statut_sca status_;
    unsigned targetId_;

};

Alg readAlg(std::istringstream& line);
Cor readCor(std::ifstream& file);
Sca readSca(std::istringstream& line);

std::istringstream nextLine(std::ifstream& file);


#endif //MICRORECIF_LIFEFORM_H
