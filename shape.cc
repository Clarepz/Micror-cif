//
// Created by Clarence Provenaz on 14.03.2024.
//
// faire type def
#include "shape.h"
#include <cmath>
#include <algorithm>

int signe(double val);

Segment::Segment(S2d coor_, double angle_, unsigned longueur_):
    coor(coor_),
    angle(angle_),
    longueur(longueur_)
{
    secPoint.x = coor.x+longueur* cos(angle);
    secPoint.y = coor.y+longueur* sin(angle);
}

fail Segment::getFail () const
{
    if(longueur<=0) return(NEGLENGHT);

    if(angle<-M_PI or angle>M_PI) return(BADANGLE);

    else return(NOFAIL);
}

S2d Segment::getSecPoint() const {
    return secPoint;
}

unsigned Segment::getlength() const{
    return longueur;
}

double Segment::getAngle() const
{
    return(angle);
}

double deltaAngle(Segment seg1, Segment seg2)
{
    return(seg1.getAngle()-seg2.getAngle()+M_PI);
}

bool suppCommun(Segment seg1, Segment seg2, Segment newSeg, double delta_rot=0)
{
    double delta1 = deltaAngle(seg1,seg2);
    double delta2 = deltaAngle(seg1,newSeg);
    if (delta1==delta2)
        return(delta1==0);
    else
    {
        bool bornInf=seg2.getAngle()<=seg1.getAngle()+delta_rot;
        bool bornSup=seg2.getAngle()>=seg1.getAngle()-delta_rot;
        if(bornInf and bornSup)
        {
            return(signe(delta1) != signe(delta2));
        }
        return false;
    }
}

bool suppIndep(Segment seg1, Segment seg2, bool sim)
{
    return true;
    return false;
}

int signe(double val)
{
    if(val>0) return 1;

    if(val<0) return -1;

    if(val==0) return 0;
}







bool onSegment(S2d p, S2d q, S2d r)
{
    //if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
      //  q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        //return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
posSeg orientation(S2d p, S2d q, S2d r)
{
    double val = (q.y - p.y) * (r.x - q.x)-(q.x - p.x) * (r.y - q.y);

    if (val<-epsil_zero and val>epsil_zero) return colineaire;  // collinear

    return (val > 0)? antiTrigo: trigo; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(S2d p1, S2d q1, S2d p2, S2d q2)
{
    // Find the four orientations needed for general and
    // special cases
    posSeg o1 = orientation(p1, q1, p2);
    posSeg o2 = orientation(p1, q1, q2);
    posSeg o3 = orientation(p2, q2, p1);
    posSeg o4 = orientation(p2, q2, q1);

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

    return false; // Doesn't fall in any of the above cases
}
