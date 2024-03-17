//
// Created by Clarence Provenaz on 15.03.2024.
//
#include <iostream>

#include "message.h"
#include "lifeform.h"
#include "constantes.h"

bool domainCheck(S2d center, double margin=1);
bool ageCheck(int age);
bool superposCheck(const std::vector<Segment>& segs,unsigned id);
bool segDomainAndLengthCheck(const std::vector<Segment>& segs,unsigned id);

Alg::Alg(S2d position, int age) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    //std::cout<<position_.x<<position_.y<<age<<std::endl;
}

Cor::Cor(S2d position, int age, unsigned id, int nbseg, const std::vector<Segment>& segs) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    id_=id;
    nbSeg_=nbseg;
    superposCheck(segs,id_);
    segDomainAndLengthCheck(segs,id_);
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


bool domainCheck(S2d center,double m){
    if (center.x<max-m and center.y<max-m and center.x>m and center.y>m){
        return true;
    }else{
        if(m==1) {
            std::cout << message::lifeform_center_outside(center.x, center.y);
            exit(EXIT_FAILURE);
        }
        return false;
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

bool superposCheck(const std::vector<Segment>& segs,unsigned id){
    for(int i = 1; i < segs.size();i++){
        if(suppCommun(segs[i],segs[i-1],false)){
            std::cout << message::segment_superposition(id,i-1,i);
            exit(EXIT_FAILURE);
            return false;
        }
    }
    return true;
}

bool segDomainAndLengthCheck(const std::vector<Segment>& segs,unsigned id){
    for(auto seg:segs){
        S2d point = seg.getSecPoint();
        if(!domainCheck(point,epsil_zero)){
            std::cout << message::lifeform_computed_outside(id,point.x,point.y);
            exit(EXIT_FAILURE);
            return false;
        }
        unsigned length = seg.getlength();
        if(!(length >= l_repro-l_seg_interne and length < l_repro)){
            std::cout << message::segment_length_outside(id,length);
            exit(EXIT_FAILURE);
            return false;
        }
    }
    return true;
}

unsigned Cor:: getId() const{
    return id_;
}

