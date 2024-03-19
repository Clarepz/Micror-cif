//
// Created by Clarence Provenaz on 14.03.2024.
//
// faire type def
#include "shape.h"
#include <cmath>
#include "constantes.h"

Segment::Segment(S2d coor_, double angle_, unsigned longueur_):
    coor(coor_),
    angle(angle_),
    longueur(longueur_)
{
    secPoint.x = coor.x+longueur* cos(angle);
    secPoint.y = coor.y+longueur* sin(angle);
}

fail Segment::getFail (Segment seg) const
{
    if(longueur<=0) return(negLenght);

    if(angle<-M_PI or angle>M_PI) return(badAngle);

    else return(noFail);
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

bool suppCommun(Segment seg1, Segment seg2,bool sensTrigo, bool sim)
{
    double delta= deltaAngle(seg1,seg2);
    if (delta>=-delta_rot*sim and delta<=delta_rot*sim)
    {
        //faire le mode simulation
        return true;
    }
    else return false;
}

bool suppIndep(Segment seg1, Segment seg2, bool sim)
{
    return true;
    return false;
}
