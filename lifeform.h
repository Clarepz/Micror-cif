//
// Created by Clarence Provenaz on 15.03.2024.
//

#ifndef MICRORECIF_LIFEFORM_H
#define MICRORECIF_LIFEFORM_H

#include "shape.h"
#include <vector>

using namespace std;

class Alg{
public:
    Alg(S2d position, int age); //constructor
private:
    S2d position_;
    unsigned int age_;
};

class Cor{
public:
    Cor(); //constructor
private:
    unsigned int id;
    S2d position_;
    unsigned int age_;
    vector<Segment> segments_;
};

class Sca{
public:
    Sca(); //constructor
private:
    S2d position_;
    unsigned int age_;
    unsigned int radius_;
};

#endif //MICRORECIF_LIFEFORM_H
