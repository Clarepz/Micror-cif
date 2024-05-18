//
// shape.cc, Provenaz Clarence 0%, Royer Yann 100%, verion 2
// conventions rendu 3 ok

#include "shape.h"


int orientation(S2d p, S2d q, S2d r, double simConstante);
bool onSegment(S2d p, S2d q, S2d r, double simConstante);

int orientation(S2d p, S2d q, S2d r, double simConstante) {
    double val = (q.y - p.y) * (r.x - q.x)-(q.x - p.x) * (r.y - q.y);
    double distance=val/sqrt(pow(p.y-q.y,2)+pow(p.x-q.x,2));
    if (distance <= simConstante and distance >= simConstante) return 0;
    //si simConstante n'est pas initialisée (lecture) la condition <=> distance==0
    return (val > 0)? 1 : 2; // sens horaire ou trigo
}

bool onSegment(S2d p, S2d q, S2d r, double simConstante) {
    double s=(q.x-p.x)*(r.x-p.x)+(q.y-p.y)*(r.y-p.y);
    double pr=sqrt(pow(r.x-p.x,2)+pow(r.y-p.y,2));
    return ((s/pr) >= (-simConstante) and (s/pr) <= (pr+simConstante));
}

bool S2d::operator==(S2d point) const {
    return(point.x==x and point.y==y);
}

bool S2d::operator!=(S2d point) const {
	return !operator==(point);
}


Segment::Segment(S2d point, double angle, double longueur):
    point_(point),
    angle_(angle),
    longueur_(longueur) {}

Segment::Segment(S2d basePoint, S2d secPoint):
    point_(basePoint) {
    longueur_ = distance(basePoint,secPoint);
    angle_ = atan2((secPoint.y-point_.y),(secPoint.x-point_.x));
}

Fail Segment::getFail () const {
    if(longueur_<=0) return(NEGLENGHT);

    if(angle_<-M_PI or angle_>M_PI) return(BADANGLE);

    else return(NOFAIL);
}

Segment Segment::addAngle(double angle) const {
    double newAngle = (angle+angle_<=-M_PI) ? angle+angle_+2*M_PI :
                      (angle+angle_>M_PI) ? angle+angle_-2*M_PI : angle+angle_;

    return(Segment(point_, newAngle, longueur_));

}

Segment Segment::addLength(double length) const {
    return Segment(point_, angle_, longueur_+length);
}

void Segment::changeLength(double change) {
    longueur_+=change;
}

double deltaAngle(const Segment &seg1, const Segment &seg2) {
    double delta=seg1.getAngle()-seg2.getAngle()+M_PI;
    if (delta>M_PI) return (delta-2*M_PI);
    else return(delta);
}

bool suppCommun(const Segment &seg1, const Segment &seg2) {
    return(deltaAngle(seg1,seg2) == 0);
}

bool suppIndep(const Segment &seg1, const Segment &seg2, double simConstante) {
    S2d p1=seg1.getPoint();
    S2d q1=seg1.getSecPoint();
    S2d p2=seg2.getPoint();
    S2d q2=seg2.getSecPoint();
    int o1 = orientation(p1, q1, p2, simConstante);
    int o2 = orientation(p1, q1, q2, simConstante);
    int o3 = orientation(p2, q2, p1, simConstante);
    int o4 = orientation(p2, q2, q1, simConstante);
    

    if (o1 and o2 and o3 and o4 and o1 != o2 and o3 != o4) return true;

    if (o1 == 0 && onSegment(p1, p2, q1, simConstante)) return true;

    if (o2 == 0 && onSegment(p1, q2, q1, simConstante)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2, simConstante)) return true;

    if (o4 == 0 && onSegment(p2, q1, q2, simConstante)) return true;

    return false;
}

void drawEntity(Shape shape,Color color, S2d position, double size, double angle) {
    drawShape(shape, color, position.x, position.y, size, angle);
}

double distance(const S2d &p1, const S2d &p2) {
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}
