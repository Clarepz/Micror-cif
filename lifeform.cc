//
// Created by Clarence Provenaz on 15.03.2024.
//
#include <iostream>

#include "message.h"
#include "lifeform.h"
#include "constantes.h"

bool domainCheck(S2d center);
bool ageCheck(int age);

Alg::Alg(S2d position, int age) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    //std::cout<<position_.x<<position_.y<<age<<std::endl;
}

Cor::Cor(S2d position, int age, int id, int nbseg, const std::vector<Segment>& segs) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    id_=id;

    for(int i = 1; i < segs.size();i++){
        if(suppCommun(segs[i],segs[i-1]),false){
            message::segment_superposition(id_,i-1,i);
            exit(EXIT_FAILURE);
        }
    }

    nbSeg_=nbseg;
    segments_=segs;
}

Sca::Sca(S2d position, int age, int radius, int targetId) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;

    if(radius>r_sca and radius<r_sca_repro){
        radius_ = radius;
    }else{
        std::cout << message::scavenger_radius_outside(radius);
        exit(EXIT_FAILURE);
    }
    targetId_=targetId;
}


bool domainCheck(S2d center){
    if (center.x<max-1 and center.y<max-1 and center.x>1 and center.y>1){
        return true;
    }else{
        std::cout << message::lifeform_center_outside(center.x,center.y);
        exit(EXIT_FAILURE);
    }
}

bool ageCheck(int age){
    if (age>0){
        return true;
    }else{
        std::cout << message::lifeform_age(age);
        exit(EXIT_FAILURE);
    }
}