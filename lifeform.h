//
// Created by Clarence Provenaz on 15.03.2024.
//

#ifndef MICRORECIF_LIFEFORM_H
#define MICRORECIF_LIFEFORM_H

#include "shape.h"
#include "constantes.h"
#include <vector>
#include <sstream>


class LifeForm{
public:
    LifeForm(S2d position, int age);
protected:
    S2d position_;
    unsigned age_;
};

class Alg{
public:
    Alg(std::istringstream& line); //constructor
private:
    S2d position_;
    unsigned age_;
};

class Cor{
public:
    Cor(S2d position, int age, unsigned id, int nbseg,const std::vector<Segment>& segs);
    unsigned getId() const;
    bool collisionCheck(const Cor& otherCor)const;
private:
    S2d position_;
    unsigned age_;
    unsigned id_;
    unsigned nbSeg_;
    std::vector<Segment> segments_;

    const std::vector<Segment>& getSegments()const;
};

class Sca{
public:
    Sca(S2d position, int age, int radius, int status, int targetId); //constructor

    unsigned getTarget() const;
    Statut_sca getStatus() const;
private:
    S2d position_;
    unsigned age_;
    unsigned radius_;
    Statut_sca status_;
    unsigned targetId_;

};

std::istringstream nextLine(std::ifstream& file);


#endif //MICRORECIF_LIFEFORM_H
