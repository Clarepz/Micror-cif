//
// Created by Clarence Provenaz on 15.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include "message.h"
#include "lifeform.h"
#include "constantes.h"



bool domainCheck(S2d center);
bool ageCheck(int age);
bool superposCheck(const std::vector<Segment>& segs,unsigned id);
bool segCheck(const std::vector<Segment>& segs,unsigned id);
bool scaRadiusCheck(int radius);


bool domainCheck(S2d center){
    if (center.x<=max-1 and center.y<=max-1 and center.x>=1 and center.y>=1){
        return true;
    }else{
        std::cout << message::lifeform_center_outside(center.x, center.y);
        exit(EXIT_FAILURE);
        //return false
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

        if(suppCommun(segs[i-1],segs[i],segs[i])){
            std::cout << message::segment_superposition(id,i-1,i);
            exit(EXIT_FAILURE);
            //return false;
        }
    }
    return true;
}

bool segCheck(const std::vector<Segment>& segs,unsigned id){
    for(auto seg:segs){
        S2d point = seg.getSecPoint();
        if(!(point.x<max-epsil_zero and point.y<max-epsil_zero
        and point.x>epsil_zero and point.y>epsil_zero)){
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

bool scaRadiusCheck(int radius){
    if(radius>r_sca and radius<r_sca_repro){
        return true;
    }else{
        std::cout << message::scavenger_radius_outside(radius);
        exit(EXIT_FAILURE);
    }
}


LifeForm::LifeForm(S2d position, int age)
    : position_(position){
    domainCheck(position_);
    ageCheck(age);
    age_=age;
}


Alg::Alg(S2d position, int age)
    : LifeForm(position,age){}


Cor::Cor(S2d position, int age, unsigned id, int nbSeg, const std::vector<Segment>& segs)
    : LifeForm(position,age){
    id_=id;
    nbSeg_=nbSeg;
    superposCheck(segs,id_);
    segCheck(segs,id_);
    segments_=segs;
}

unsigned Cor::getId() const{
    return id_;
}

bool Cor::collisionCheck(const Cor &otherCor) const{
    const std::vector<Segment>& otherSegs = otherCor.getSegments();
    unsigned otherId = otherCor.getId();
    for (int i(0);i<nbSeg_;i++){
        for(int k(0); k<otherSegs.size();k++){
            if(!(id_ == otherId and (i == k or i==k-1 or i == k+1))) {
                if (suppIndep(segments_[i], otherSegs[k])) {
                    //if not (same cor and same/touching segment)
                    std::cout << message::segment_collision(id_, i, otherId, k);;
                    exit(EXIT_FAILURE);
                    //return false;
                }
            }
        }
    }
    return true;
}

const std::vector<Segment>& Cor::getSegments()const{
    return segments_;
}


Sca::Sca(S2d position, int age, int radius, int status, int targetId)
    : LifeForm(position,age){
    scaRadiusCheck(radius);
    radius_ = radius;
    status_ = static_cast<Statut_sca>(status);
    targetId_=targetId;
}

unsigned int Sca::getTarget() const {
    return targetId_;
}

Statut_sca Sca::getStatus() const {
    return status_;
}


Alg readAlg(std::istringstream& line){
    S2d pos;
    int age;

    line>>pos.x>>pos.y>>age;
    return Alg(pos,age);
}

Cor readCor(std::ifstream& file){
    std::istringstream line = nextLine(file);
    S2d pos;
    int age, id, statut, dir, dev, nbSeg;
    line>>pos.x>>pos.y>>age>>id>>statut>>dir>>dev>>nbSeg;
    std::vector<Segment> segs;
    for(int k=0;k<nbSeg;k++) {
        line = nextLine(file);
        double angle;
        unsigned length;
        line >> angle >> length;
        if (k == 0) {
            segs.emplace_back(pos, angle, length);
        } else {
            S2d BasePoint = segs[k - 1].getSecPoint();
            segs.emplace_back(BasePoint, angle, length);
        }
    }
    return Cor(pos,age,id,nbSeg,segs);
}

Sca readSca(std::istringstream& line){
    S2d pos;
    int age,radius,statut,targetId;
    line>>pos.x>>pos.y>>age>>radius>>statut>>targetId;
    return Sca(pos, age, radius, statut, targetId);
}

std::istringstream nextLine(std::ifstream& file) {
    std::string line;
    do {
        std::getline(file>>std::ws, line);
    } while (line[0] == '#');
    std::istringstream lineStream(line);
    return lineStream;
}
