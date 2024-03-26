//
// shape.h, Provenaz Clarence, version 1
//

#ifndef SHAPE_H
#define SHAPE_H

enum Fail {NOFAIL, NEGLENGHT, BADANGLE};

constexpr double epsil_zero(0.5) ;

struct S2d {double x=0.; double y=0.;};

class Segment{
public:
    Segment(S2d coor_,double angle_, unsigned longueur_);//constructeur
    S2d getPoint() const;
    S2d getSecPoint() const;
    double getlength() const;
    double getAngle() const;
    Fail getFail() const;
    Segment addAngle(double angle) const; //sert pour la supperposition en simulation
private:
    S2d point;
    double angle;
    double longueur;
    S2d secPoint;
};

double deltaAngle(Segment seg1, Segment seg2);
bool suppCommun(const Segment &seg1, const Segment &seg2);
//renvoie true si les segements sont supperposés
bool suppIndep(Segment seg1, Segment seg2);//intersection de 2 segments independants

#endif
