//
// Created by Clarence Provenaz on 15.03.2024.
//
#include <iostream>

#include "message.h"
#include "lifeform.h"
#include "constantes.h"
#include <fstream>
#include <sstream>



bool domainCheck(S2d center, double margin=1);
bool ageCheck(int age);
bool superposCheck(const std::vector<Segment>& segs,unsigned id);
bool segCheck(const std::vector<Segment>& segs,unsigned id);
bool scaRadiusCheck(int radius);

Alg::Alg(std::istringstream& line) {
    line>>position_.x>>position_.y>>age_;
    domainCheck(position_);
    ageCheck(age_);
}

Cor::Cor(S2d position, int age, unsigned id, int nbseg, const std::vector<Segment>& segs) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    id_=id;
    nbSeg_=nbseg;
    superposCheck(segs,id_);
    segCheck(segs,id_);
    segments_=segs;
}

Sca::Sca(S2d position, int age, int radius,int status, int targetId) {
    domainCheck(position);
    position_=position;
    ageCheck(age);
    age_=age;
    scaRadiusCheck(radius);
    radius_ = radius;
    status_ = static_cast<Statut_sca>(status);
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
        if(suppCommun(segs[i],segs[i-1],segs[i-1])){
            std::cout << message::segment_superposition(id,i-1,i);
            exit(EXIT_FAILURE);
            return false;
        }
    }
    return true;
}

bool segCheck(const std::vector<Segment>& segs,unsigned id){
    for(auto seg:segs){
        S2d point = seg.getSecPoint();
        if(!domainCheck(point,epsil_zero)){
            std::cout << message::lifeform_computed_outside(id,point.x,point.y);
            exit(EXIT_FAILURE);
            //return false;
        }
        unsigned length = seg.getlength();
        if(!(length >= l_repro-l_seg_interne and length < l_repro)){
            std::cout << message::segment_length_outside(id,length);
            exit(EXIT_FAILURE);
            //return false;
        }
        if(seg.getFail()==BADANGLE){
            std::cout << message::segment_angle_outside(id, seg.getAngle());
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

unsigned Cor:: getId() const{
    return id_;
}

const std::vector<Segment>& Cor::getSegments()const{
    return segments_;
}

bool Cor::collisionCheck(const Cor &otherCor) const{
    const std::vector<Segment>& otherSegs = otherCor.getSegments();
    unsigned otherId = otherCor.getId();
    for (int i(0);i<nbSeg_;i++){
        for(int k(0); k<otherSegs.size();k++){
            if(!(id_ == otherId and (i == k or i==k-1 or i == k+1))) {
                if (suppIndep(segments_[i], otherSegs[k], false)) {
                    //if not (same cor same segment/consecutif)
                    std::cout << message::segment_collision(id_, i, otherId, k);;
                    exit(EXIT_FAILURE);
                    return false;
                }
            }
        }
    }
    return true;
}


bool scaRadiusCheck(int radius){
    if(radius>r_sca and radius<r_sca_repro){
        return true;
    }else{
        std::cout << message::scavenger_radius_outside(radius);
        exit(EXIT_FAILURE);
    }
}

unsigned int Sca::getTarget() const {
    return targetId_;
}

Statut_sca Sca::getStatus() const {
    return status_;
}

std::istringstream nextLine(std::ifstream& file) {
    std::string line;
    do {
        std::getline(file>>std::ws, line);
    } while (line[0] == '#');
    std::istringstream lineStream(line);
    return lineStream;
}