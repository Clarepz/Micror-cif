//
// shape.h, Provenaz Clarence, Royer Yann
// conventions ok

#ifndef SHAPE_H
#define SHAPE_H

#include "graphic.h"

enum Fail {NOFAIL, NEGLENGHT, BADANGLE};

constexpr double epsil_zero(0.5) ;

struct S2d
{
    double x=0.;
    double y=0.;
};


class Segment{
public:
    Segment(S2d coor_,double angle_, double longueur_);//constructeur
    S2d getPoint() const;
    S2d getSecPoint() const;
    double getlength() const;
    double getAngle() const;
    Fail getFail() const;
    Segment addAngle(double angle_) const;//servira pour la supperposition en simulation
private:
    S2d point;
    double angle;
    double longueur;
    S2d secPoint;
};

double deltaAngle(Segment seg1, Segment seg2);
bool suppCommun(const Segment &seg1, const Segment &seg2, double simConstante=0);
//renvoie true si deux segements avec un point commun sont supperposés (delta_rot)
bool suppIndep(const Segment &seg1, const Segment &seg2, double simConstante=0);
//intersection de 2 segments independants                       (epsil_zero)
void drawEntity(Shape shape,Color color, S2d position, double size, double angle=0);

#endif
