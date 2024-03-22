//
// Created by Clarence Provenaz on 14.03.2024.
//

#include <cmath>
#include "shape.h"

int signe(double val);
int orientation(S2d p, S2d q, S2d r);
bool onSegment(S2d p, S2d q, S2d r);

int signe(double val)
{
    if(val>0) return 1;

    if(val<0) return -1;

    else return 0;
}

int orientation(S2d p, S2d q, S2d r)
{
    double val = (q.y - p.y) * (r.x - q.x) -
                 (q.x - p.x) * (r.y - q.y);
    double distance=val/sqrt(pow(p.y-q.y,2)+pow(p.x-q.x,2));
    if (distance <= epsil_zero and distance >=-epsil_zero) return 0;  // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool onSegment(S2d p, S2d q, S2d r)
{
    double s=(q.x-p.x)*(r.x-p.x)+(q.y-p.y)*(r.y-p.y);
    double pr=sqrt(pow(r.x-p.x,2)+pow(r.y-p.y,2));
    return (s/pr<=-epsil_zero and s/pr>=pr+epsil_zero);
}

Segment::Segment(S2d point_, double angle_, unsigned longueur_):
    point(point_),
    angle(angle_),
    longueur(longueur_)
{
    secPoint.x = point.x+longueur* cos(angle);
    secPoint.y = point.y+longueur* sin(angle);
}

S2d Segment::getPoint() const {
    return point;
}

S2d Segment::getSecPoint() const {
    return secPoint;
}

double Segment::getlength() const{
    return longueur;
}

double Segment::getAngle() const
{
    return(angle);
}

Fail Segment::getFail () const
{
    if(longueur<=0) return(NEGLENGHT);

    if(angle<-M_PI or angle>M_PI) return(BADANGLE);

    else return(NOFAIL);
}

Segment Segment::addAngle(double angle) const
{
    double newAngle=angle+this->angle;
    return(Segment(point, newAngle, longueur));
}

double deltaAngle(Segment seg1, Segment seg2)
{
    return(seg1.getAngle()-seg2.getAngle()+M_PI);
}

bool suppCommun(const Segment &seg1, const Segment &seg2, const Segment &newSeg,
                double inter)
{
    double delta1 = deltaAngle(seg1,seg2);
    double delta2 = deltaAngle(seg1,newSeg);
    if (delta1==delta2)
        return(delta1==0);
    else
    {
        bool bornInf=seg2.getAngle()<=seg1.getAngle()+inter;
        bool bornSup=seg2.getAngle()>=seg1.getAngle()-inter;
        if(bornInf and bornSup)
        {
            return(signe(delta1) != signe(delta2));
        }
        return false;
    }
}

bool suppIndep(Segment seg1, Segment seg2)
{
    S2d p1=seg1.getPoint();
    S2d q1=seg1.getSecPoint();
    S2d p2=seg2.getPoint();
    S2d q2=seg2.getSecPoint();
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}