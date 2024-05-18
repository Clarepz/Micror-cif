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
    Segment(S2d coor,double angle, double longueur);
    Segment(S2d basePoint, S2d secPoint);
    
    //getters
    S2d getPoint() const {return point_;}
    S2d getSecPoint() const {return S2d {point_.x+longueur_* cos(angle_),
										 point_.y+longueur_* sin(angle_)};}
    double getlength() const {return longueur_;}
    double getAngle() const {return(angle_);}
    
    //méthodes pour la simulation
    Fail getFail() const;
    Segment addAngle(double angle) const;
    Segment addLength (double length) const;
    void changeLength(double change); //ajoute change a l'attribut longueur_
    
private:
    S2d point_;
    double angle_;
    double longueur_;
};

double deltaAngle(const Segment &seg1, const Segment &seg2);
bool suppCommun(const Segment &seg1, const Segment &seg2);
//renvoie true si deux segements avec un point commun sont supperposés
bool suppIndep(const Segment &seg1, const Segment &seg2, double simConstante=0);
//intersection de 2 segments independants                       (epsil_zero)
void drawEntity(Shape shape,Color color, S2d position, double size, double angle=0);
double distance(const S2d &p1, const S2d &p2);
#endif
