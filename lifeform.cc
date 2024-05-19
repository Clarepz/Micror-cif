//
// lifeform.cc, Provenaz Clarence 100%, Royer Yann 0%, verion 2
//

#include <iostream>
#include <fstream>
#include <cmath>
#include "message.h"
#include "lifeform.h"
#include "constantes.h"

using namespace std;

bool domainCheck(S2d center);
bool ageCheck(int age);
bool superposCheck(const vector<Segment>& segs, unsigned id);
bool segCheck(const vector<Segment>& segs, unsigned id);
bool scaRadiusCheck(int radius);
bool segCollisionCheck(const Segment& segment, const Segment& oldSegment, 
					   const Cor& otherCor);

bool domainCheck(S2d center) {
    if (center.x<=dmax-1 and center.y<=dmax-1 and center.x>=1 and center.y>=1) {
        return true;
    }
    else {
        cout << message::lifeform_center_outside(center.x, center.y);
        return false;
    }
}

bool ageCheck(int age) {
    if (age>0) return true;
    else {
        cout << message::lifeform_age(age);
        return false;
    }
}

bool superposCheck(const vector<Segment>& segs, unsigned id) {
    for(int i = 1; i < segs.size();i++){
        if(suppCommun(segs[i-1],segs[i])) {
            cout << message::segment_superposition(id,i-1,i);
            return false;
        }
    }
    return true;
}

bool segCheck(const vector<Segment>& segs, unsigned id) {
    for(auto seg:segs) {
        S2d point = seg.getSecPoint();
        if(!(point.x<dmax-epsil_zero and point.y<dmax-epsil_zero
             and point.x>epsil_zero and point.y>epsil_zero)) {
            cout << message::lifeform_computed_outside(id,point.x,point.y);
            return false;
        }
        unsigned length = seg.getlength();
        if(!(length >= l_repro-l_seg_interne and length < l_repro)) {
            cout << message::segment_length_outside(id,length);
            return false;
        }
        if(seg.getFail()==BADANGLE) {
            cout << message::segment_angle_outside(id, seg.getAngle());
            return false;
        }
    }
    return true;
}

bool scaRadiusCheck(int radius) {
    if(radius>=r_sca and radius<r_sca_repro) return true;
    else {
        cout << message::scavenger_radius_outside(radius);
        return false;
    }
}

bool segCollisionCheck(const Segment& segment, const Segment& oldSegment, 
					   const Cor& otherCor){
    for(Segment otherSegment : otherCor.getSegments()){
        if(segment.getPoint() == otherSegment.getSecPoint()) {
            double deltaAngleNew = deltaAngle(otherSegment,segment);
            double deltaAngleOld = deltaAngle(otherSegment,oldSegment);
            if(deltaAngleNew<=delta_rot and deltaAngleNew>=-delta_rot
               and deltaAngleNew*deltaAngleOld<=0 ) return false;
        }
        else {
            if (suppIndep(segment,otherSegment, epsil_zero)
                and segment.getPoint() != otherSegment.getPoint()) return false;
        }
    }
    return true;
}


//classe Lifeform
LifeForm::LifeForm(S2d position, int age) : 
	position_(position), 
	age_(age),
	initSuccess(true) {
    if(!(domainCheck(position_) and ageCheck(age))) initSuccess = false;
}

void LifeForm::writeFile(ofstream &file) const {
    file << '\t' << position_.x << ' ' << position_.y << ' ' <<to_string(age_)<< ' ' ;
}


//classe Alg
Alg::Alg(S2d position, int age)
    : LifeForm(position,age) {}

void Alg::writeFile(ofstream &file) const {
    LifeForm::writeFile(file);
    file << endl;
}

void Alg::display() const {
    drawEntity(CIRCLE, GREEN, position_, r_alg);
}

void Alg::update(bool &dead) {
    age_++;
    if(age_>=max_life_alg) dead = true;
}


//classe Cor
Cor::Cor(S2d position, int age, int id, int status, int dir, int statusDev, int nbSeg,
         const vector<Segment>& segs) :
    LifeForm(position,age), 
	id_(id),
	status_(static_cast<Status_cor>(status)),
	dir_(static_cast<Dir_rot_cor>(dir)),
	statusDev_(static_cast<Status_dev>(statusDev)),
	nbSeg_(nbSeg),
	segments_(segs),
	allocatedId(false) {
    if (!(superposCheck(segs,id_) and segCheck(segs,id_))) initSuccess = false;
}

bool Cor::collisionCheck(const Cor &otherCor) const {
    const vector<Segment>& otherSegs = otherCor.getSegments();
    unsigned otherId = otherCor.getId();
    for (int i(0);i<nbSeg_;i++) {
        for(int k(0); k<otherSegs.size();k++) {
            if(!(id_ == otherId and (i == k or i==k-1 or i == k+1))) {
                if (suppIndep(segments_[i], otherSegs[k])) {
                    //if not (same cor and same/touching segment)
                    cout << message::segment_collision(id_, i, otherId, k);
                    return false;
                }
            }
        }
    }
    return true;
}

void Cor::writeFile(ofstream &file) const {
    LifeForm::writeFile(file);
    file << id_ << ' ' << status_ << ' ' << dir_ << ' ' << statusDev_ << ' '
    << nbSeg_ << endl;
    for(auto aSeg : segments_) {
        file << "\t\t" << aSeg.getAngle() << ' ' << aSeg.getlength() << endl;
    }
}

void Cor::display() const {
    Color corColor = (status_ == ALIVE)? BLUE:BLACK;
    drawEntity(SQUARE, corColor, position_, d_cor);
    for(auto aSeg : segments_) {
        drawEntity(LINE, corColor, aSeg.getPoint(),
                   aSeg.getlength(), aSeg.getAngle());
    }
}

void Cor::update(const vector<Cor>& cors, const vector<Alg>& algs,
                 vector<Cor>& babyCor, int& deadAlgIndex) {
    if(status_==DEAD) return;
    age_++ ;
    if(age_ >= max_life_cor){
        status_ = DEAD;
        return;
    }
    Segment& lastSeg =  segments_[segments_.size()-1];
    if(lastSeg.getlength()>=l_repro){
        if(statusDev_==EXTEND){
            extend();
            statusDev_ = REPRO;
        }else{

            unsigned newId = cors[cors.size()-1].getId() + 1;
            bool IdAlreadyPicked;
            do{
                IdAlreadyPicked = false;
                for(Cor aCor : cors){
                    if(aCor.getId()==newId){
                        IdAlreadyPicked = true;
                        newId ++;
                        break;
                    }
                }
            }while (IdAlreadyPicked);
            babyCor.push_back(repro(newId));
            statusDev_ = EXTEND;
        }
        return;
    }
    Segment newLastSeg = lastSeg.addAngle((dir_ == TRIGO)? delta_rot : -delta_rot);
    int closestAlg = 0;
    double closestAlgAngle = delta_rot;
    bool mustEatAlg = false;
    for(int i=0; i<algs.size(); i++) {
        double angleToAlg ;
        if(shouldEat(algs[i], angleToAlg) && fabs(angleToAlg)<=fabs(closestAlgAngle)) {
            closestAlgAngle = angleToAlg;
            closestAlg = i;
            mustEatAlg =true;
        }
    }
    if(algs.size()>0 and shouldEat(algs[closestAlg],closestAlgAngle)) {
        newLastSeg = lastSeg.addAngle(closestAlgAngle);
        newLastSeg = newLastSeg.addLength(delta_l);
    }
    bool updateCheck = true;
    for(const Cor& aCor : cors) {
        if (!segCollisionCheck(newLastSeg, lastSeg, aCor)) {
            dir_ = (dir_ == TRIGO)? INVTRIGO : TRIGO;
            updateCheck = false;
            break;
        }
    }
    //check collision with domain border
    S2d endPoint = newLastSeg.getSecPoint();
    if(!(endPoint.x<dmax-epsil_zero and endPoint.y<dmax-epsil_zero
         and endPoint.x>epsil_zero and endPoint.y>epsil_zero)) {
        dir_ = (dir_ == TRIGO)? INVTRIGO : TRIGO;
        updateCheck = false;
    }
    if(updateCheck) {
        lastSeg = newLastSeg;
        if(mustEatAlg) deadAlgIndex = closestAlg;
        else deadAlgIndex = -1;
    }
}

void Cor::setAllocatedId(bool allocatedId_) {
    allocatedId=allocatedId_;
}

bool Cor::eaten(S2d &nextScaPos) {
    //si le segment restant est court, on le mange jusqu'au prochain segment
    if(segments_[nbSeg_-1].getlength()<=delta_l)
    {
        nextScaPos=segments_[nbSeg_-1].getPoint();
        nbSeg_--;
        //si nbSeg==0 on renvoie true pour informer que le corail est mangé en entier
        if(nbSeg_==0) return(true);
        else segments_.pop_back();
    }
    else {
        segments_[nbSeg_ - 1].changeLength(-int(delta_l));
        nextScaPos = segments_[nbSeg_ - 1].getSecPoint();
    }
    return(false);
}

bool Cor::shouldEat(const Alg &anAlg, double &angleToAlg)const {
    Segment lastSeg = segments_[nbSeg_-1];
    Segment algToCor(anAlg.getPosition(),lastSeg.getPoint());
    angleToAlg = deltaAngle(algToCor,lastSeg);
    if(algToCor.getlength() <= lastSeg.getlength()) {
        switch (dir_) {
            case TRIGO:
                return angleToAlg <= delta_rot and angleToAlg >=0;
            case INVTRIGO:
                return angleToAlg >= -delta_rot and angleToAlg <=0;
        }
    }
    return false;
}

void Cor::extend() {
    Segment& lastSeg = segments_[nbSeg_-1];
    lastSeg = lastSeg.addLength(-int(l_repro-l_seg_interne));
    segments_.emplace_back(lastSeg.getSecPoint(),lastSeg.getAngle(),
                           l_repro-l_seg_interne);
    nbSeg_++;
}

Cor Cor::repro(unsigned id) {
    Segment& lastSeg = segments_[nbSeg_-1];
    lastSeg = lastSeg.addLength(-int(l_repro/2));

    Segment constructionSeg(lastSeg.getPoint(), lastSeg.getAngle(), l_seg_interne);
    Segment newSeg(constructionSeg.getSecPoint(), lastSeg.getAngle(),
                   l_repro-l_seg_interne);

    vector<Segment> newSegs{newSeg};

    return Cor(newSeg.getPoint(), 1, id, ALIVE, TRIGO, EXTEND, 1, newSegs);
}


//classe sca
Sca::Sca(S2d position, int age, int radius, int status, int targetId) : 
	LifeForm(position,age),
	radius_(radius),
	status_(static_cast<Status_sca>(status)),
	targetId_(targetId),
	onTarget(false) {
    if(!scaRadiusCheck(radius)) initSuccess = false;
}

Sca::Sca(S2d newScaPos) : 
	LifeForm(newScaPos,1), 
    radius_(r_sca),
    status_(FREE),
    targetId_(0),
    onTarget(false) {}

void Sca::setFree() {
    onTarget=false;
    status_=FREE;
}

void Sca::setTarget(int coralId) {
    targetId_=coralId;
    status_=EATING;
}

void Sca::writeFile(ofstream &file) const {
    LifeForm::writeFile(file);
    file << radius_ << ' ' << status_ << ' ';
    if(status_ == EATING) file << targetId_ ;
    file << endl;
}

void Sca::display() const {
    drawEntity(CIRCLE, RED, position_, radius_);
}

void Sca::update(bool &scaTooOld, bool &corDestroy, bool &scaBirth, S2d &newScaPos, 
				 Cor &target) {
    //gestion de l'age et màj du booleen dead
    ++age_;
    if(age_ >= max_life_sca) {
        scaTooOld=true;
        return;
    }
    if(status_ == EATING) {
        //cas ou le scavenger n'a pas encore atteind le corail
        if(!onTarget) {
            double deltaX=target.getLastSegmentSecPoint().x-position_.x;
            double deltaY=target.getLastSegmentSecPoint().y-position_.y;
            //scavenger = près du corail, il se déplace directement sur lui
            if(sqrt(pow(deltaX,2)+ pow(deltaY, 2))<=delta_l) {
                position_=target.getLastSegmentSecPoint();
                onTarget=true;
            }
            else {
                double angle = atan2(deltaY, deltaX);
                position_.x += cos(angle) * delta_l;
                position_.y += sin(angle) * delta_l;
            }
        }
        else {
            /*mange un bout du corail via eaten, si il est fini, destroy <- true. Eaten
            modifie la position du scavenger pour la mettre à la fin du corail*/
			newScaPos=position_;
            if(target.eaten(position_)) corDestroy=true;
            if((radius_+=delta_r_sca) >= r_sca_repro) {
                scaBirth=true;
                radius_=r_sca;
            }
        }
    }
}


Alg readAlg(istringstream& line) {
    S2d pos;
    int age;
    line>>pos.x>>pos.y>>age;
    return Alg(pos,age);
}

Cor readCor(ifstream& file) {
    istringstream line = nextLine(file);
    S2d pos;
    int age, id, statut, dir, dev, nbSeg;
    line>>pos.x>>pos.y>>age>>id>>statut>>dir>>dev>>nbSeg;
    vector<Segment> segs;
    for(int k=0; k<nbSeg; k++) {
        line = nextLine(file);
        double angle;
        unsigned length;
        line >> angle >> length;
        if (k == 0) segs.emplace_back(pos, angle, length);
        else {
            S2d BasePoint = segs[k - 1].getSecPoint();
            segs.emplace_back(BasePoint, angle, length);
        }
    }
    return Cor(pos,age,id,statut,dir,dev,nbSeg,segs);
}

Sca readSca(istringstream& line) {
    S2d pos;
    int age,radius,statut,targetId;
    line>>pos.x>>pos.y>>age>>radius>>statut>>targetId;
    return Sca(pos, age, radius, statut, targetId);
}

istringstream nextLine(ifstream& file) {
    string line;
    do {
        getline(file>>ws, line);
    } while (line[0] == '#');
    istringstream lineStream(line);
    return lineStream;
}
