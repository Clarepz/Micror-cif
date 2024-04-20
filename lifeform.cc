//
// lifeform.cc, Provenaz Clarence 100%, Royer Yann 0%
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
    if (center.x<=dmax-1 and center.y<=dmax-1 and center.x>=1 and center.y>=1){
        return true;
    }else{
        std::cout << message::lifeform_center_outside(center.x, center.y);
        return false;
    }
}

bool ageCheck(int age){
    if (age>0){
        return true;
    }else{
        std::cout << message::lifeform_age(age);
        return false;
    }
}

bool superposCheck(const std::vector<Segment>& segs,unsigned id){
    for(int i = 1; i < segs.size();i++){

        if(suppCommun(segs[i-1],segs[i])){
            std::cout << message::segment_superposition(id,i-1,i);
            return false;
        }
    }
    return true;
}

bool segCheck(const std::vector<Segment>& segs,unsigned id){
    for(auto seg:segs){
        S2d point = seg.getSecPoint();
        if(!(point.x<dmax-epsil_zero and point.y<dmax-epsil_zero
        and point.x>epsil_zero and point.y>epsil_zero)){
            std::cout << message::lifeform_computed_outside(id,point.x,point.y);
            return false;
        }
        unsigned length = seg.getlength();
        if(!(length >= l_repro-l_seg_interne and length < l_repro)){
            std::cout << message::segment_length_outside(id,length);
            return false;
        }
        if(seg.getFail()==BADANGLE){
            std::cout << message::segment_angle_outside(id, seg.getAngle());
            return false;
        }
    }
    return true;
}

bool scaRadiusCheck(int radius){
    if(radius>=r_sca and radius<r_sca_repro){
        return true;
    }else{
        std::cout << message::scavenger_radius_outside(radius);
        return false;
    }
}


LifeForm::LifeForm(S2d position, int age)
    : position_(position), initSuccess(true){
    if ( !(domainCheck(position_) and ageCheck(age)) ){
        initSuccess = false;
    }
    age_=age;
}

bool LifeForm::getInitSuccess() const {
    return initSuccess;
}

void LifeForm::update() {
    age_++;
}

void LifeForm::writeFile(std::ofstream &file) const {
    file << '\t' << position_.x << ' ' << position_.y << ' ' << age_ << ' ' ;
}

Alg::Alg(S2d position, int age)
    : LifeForm(position,age){}

void Alg::writeFile(std::ofstream &file) const {
    LifeForm::writeFile(file);
    file << std::endl;
}

void Alg::display() const {
    drawEntity(CIRCLE, GREEN, position_, r_alg);
}

bool Alg::isTooOld() const {
    return age_>=max_life_alg;
}

Cor::Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
         const std::vector<Segment>& segs): LifeForm(position,age){
    id_ = id;
    status_ = static_cast<Status_cor>(status);
    dir_ = static_cast<Dir_rot_cor>(dir);
    statusDev_ = static_cast<Status_dev>(statusDev);

    nbSeg_ = nbSeg;
    if ( !(superposCheck(segs,id_) and segCheck(segs,id_)) ){
        initSuccess = false;
    }

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
                    return false;
                }
            }
        }
    }
    return true;
}

void Cor::writeFile(std::ofstream &file) const{
    LifeForm::writeFile(file);
    file << id_ << ' ' << status_ << ' ' << dir_ << ' ' << statusDev_ << ' '
    << nbSeg_ << std::endl;
    for(auto aSeg : segments_){
        file << "\t\t" << aSeg.getAngle() << ' ' << aSeg.getlength() << std::endl;
    }
}

void Cor::display() const {
    Color corColor = (status_ == ALIVE)? BLUE:BLACK;
    drawEntity(SQUARE, corColor, position_, d_cor);
    for(auto aSeg : segments_){
        drawEntity(LINE, corColor, aSeg.getPoint(),
                   aSeg.getlength(), aSeg.getAngle());
    }
}

const std::vector<Segment>& Cor::getSegments()const{
    return segments_;
}


Sca::Sca(S2d position, int age, int radius, int status, int targetId)
    : LifeForm(position,age){

    if(!scaRadiusCheck(radius)){
        initSuccess = false;
    }
    radius_ = radius;
    status_ = static_cast<Status_sca>(status);
    targetId_ = targetId;
}

unsigned int Sca::getTarget() const {
    return targetId_;
}

Status_sca Sca::getStatus() const {
    return status_;
}

void Sca::writeFile(std::ofstream &file) const {
    LifeForm::writeFile(file);
    file << radius_ << ' ' << status_ << ' ';
    if(status_ == EATING) file << targetId_ ;
    file << std::endl;
}

void Sca::display() const {
    drawEntity(CIRCLE, RED, position_, radius_);
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
    return Cor(pos,age,id,statut,dir,dev,nbSeg,segs);
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
