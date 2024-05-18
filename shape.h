//
// shape.h, Provenaz Clarence, Royer Yann
// conventions rendu 3 ok

#ifndef SHAPE_H
#define SHAPE_H

#include <cmath>
#include "graphic.h"

enum Fail {NOFAIL, NEGLENGHT, BADANGLE};

constexpr double epsil_zero(0.5) ;

struct S2d {
    double x=0.;
    double y=0.;
    bool operator==(S2d point) const;
    bool operator!=(S2d point) const;
};


class Segment {
public:
	//constructeurs
    Segment(S2d coor_,double angle_, double longueur_);
    Segment(S2d basePoint, S2d secPoint_);
    
    //getteurs
    S2d getPoint() const {return point;}
    S2d getSecPoint() const {return S2d {point.x+longueur* cos(angle),
										 point.y+longueur* sin(angle)};}
    double getlength() const {return longueur;}
    double getAngle() const {return(angle);}
    
    //méthode pour la simulation
    Fail getFail() const;
    Segment addAngle(double angle_) const;
    Segment addLength(double length) const; //renvoie un segment plus long de "lenght"
    void changeLength(double change); //change la taille du segment en rajoutant change
    
private:
    S2d point;
    double angle;
    double longueur;
};

double deltaAngle(Segment seg1, Segment seg2);
bool suppCommun(const Segment &seg1, const Segment &seg2);
//renvoie true si deux segements avec un point commun sont supperposés
bool suppIndep(const Segment &seg1, const Segment &seg2, double simConstante=0);
//intersection de 2 segments independants                       (epsil_zero)
void drawEntity(Shape shape,Color color, S2d position, double size, double angle=0);
double distance(S2d p1, S2d p2);
#endif
