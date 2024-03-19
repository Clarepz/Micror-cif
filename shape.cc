//
// Created by Clarence Provenaz on 14.03.2024.
//
// faire type def
#include "shape.h"
#include <cmath>

Segment::Segment(S2d coor_, double angle_, unsigned longueur_):
    coor(coor_),
    angle(angle_),
    longueur(longueur_)
{
    secPoint.x = coor.x+longueur* cos(angle);
    secPoint.y = coor.y+longueur* sin(angle);
}

fail Segment::getFail (Segment seg)
{
    if(longueur<=0) return(negLenght);

    if(angle<-M_PI or angle>M_PI) return(badAngle);

    else return(noFail);
}

S2d Segment::getSecPoint() {
    return secPoint;
}

unsigned Segment::getlength() {
    return longueur;
}

double Segment::getAngle()
{
    return(angle);
}

double deltaAngle(Segment seg1, Segment seg2)
{
    return(seg1.getAngle()-seg2.getAngle()+M_PI);
}

bool suppCommun(Segment seg1, Segment seg2, bool sim)
{
    if (sim)
    {
        //if (deltaAngle(seg1, seg2) != 0) return true;
        //return false;
        //test123
    }

    else
    {
        if (deltaAngle(seg1, seg2) != 0) return true;
        else return false;
    }
}

bool suppIndep(Segment seg1, Segment seg2, bool sim){
    return true;
    return false;
}
