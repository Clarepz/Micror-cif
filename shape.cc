//
// Created by Clarence Provenaz on 14.03.2024.
//
// faire type def
#include "shape.h"
#include <cmath>

Segment::Segment(S2d coor_, double angle_, double longueur_):
    coor(coor_),
    angle(angle_),
    longueur(longueur_){
    secPoint.x = coor.x+longueur* cos(angle);
    secPoint.y = coor.y+longueur* sin(angle);
}
//rajouter:
//tester l'erreur : L’angle des segments est compris dans [-π, π]
S2d Segment::getSecPoint() {
    return secPoint;
}

bool suppCommun(Segment seg1, Segment seg2, bool sim){
    return true;
}
