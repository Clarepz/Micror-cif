//
// shape.cc, Provenaz Clarence 0%, Royer Yann 100%, verion 2
// conventions rendu 3 ok

#include "shape.h"


int orientation(S2d p, S2d q, S2d r);
bool onSegment(S2d p, S2d q, S2d r, double simConstante);

int orientation(S2d p, S2d q, S2d r)
{
    double val = (q.y - p.y) * (r.x - q.x)-(q.x - p.x) * (r.y - q.y);
    double distance=val/sqrt(pow(p.y-q.y,2)+pow(p.x-q.x,2));
    if (distance == 0) return 0;
    //si simConstante n'est pas initialisée (lecture) la condition <=> distance==0
    return (val > 0)? 1 : 2; // sens horaire ou trigo
}

bool onSegment(S2d p, S2d q, S2d r, double simConstante) 
{	
    double s=(q.x-p.x)*(r.x-p.x)+(q.y-p.y)*(r.y-p.y);
    double pr=sqrt(pow(r.x-p.x,2)+pow(r.y-p.y,2));
    return ((s/pr) >= (-simConstante) and (s/pr) <= (pr+simConstante));
}

bool S2d::operator==(S2d point) const 
{	
    return(point.x==x and point.y==y);
}

bool S2d::operator!=(S2d point) const 
{
	return !operator==(point);
}


Segment::Segment(S2d point_, double angle_, double longueur_):
    point(point_),
    angle(angle_),
    longueur(longueur_)
{}

Segment::Segment(S2d basePoint, S2d secPoint_):
    point(basePoint)
{
    longueur = distance(basePoint,secPoint_);
    angle = atan2((secPoint_.y-point.y),(secPoint_.x-point.x));
}

Fail Segment::getFail () const
{
    if(longueur<=0) return(NEGLENGHT);

    if(angle<-M_PI or angle>M_PI) return(BADANGLE);

    else return(NOFAIL);
}

Segment Segment::addAngle(double angle_) const
{
    double newAngle = (angle+angle_<=-M_PI) ? angle+angle_+2*M_PI :
                      (angle+angle_>M_PI) ? angle+angle_-2*M_PI : angle+angle_;

    return(Segment(point, newAngle, longueur));

}

Segment Segment::addLength(double length)
{
    return Segment(point, angle, longueur+length);
}

void Segment::changeLength(double change) 
{
    longueur+=change;
}

double deltaAngle(Segment seg1, Segment seg2)
{
    double delta=seg1.getAngle()-seg2.getAngle()+M_PI;
    if (delta>M_PI) return (delta-2*M_PI);
    else return(delta);
}

bool suppCommun(const Segment &seg1, const Segment &seg2, double simConstante)
{
    return(deltaAngle(seg1,seg2) <= simConstante and 
		   deltaAngle(seg1,seg2) >= -simConstante);
}

bool suppIndep(const Segment &seg1, const Segment &seg2, double simConstante)
{
    S2d p1=seg1.getPoint();
    S2d q1=seg1.getSecPoint();
    S2d p2=seg2.getPoint();
    S2d q2=seg2.getSecPoint();
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && onSegment(p1, p2, q1, simConstante)) return true;

    if (o2 == 0 && onSegment(p1, q2, q1, simConstante)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2, simConstante)) return true;

    if (o4 == 0 && onSegment(p2, q1, q2, simConstante)) return true;

    return false;
}

void drawEntity(Shape shape,Color color, S2d position, double size, double angle)
{
    drawShape(shape, color, position.x, position.y, size, angle);
}

double distance(S2d p1, S2d p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}
