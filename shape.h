//
// Created by royer on 15.03.2024.
//

#ifndef MICRORECIF_SHAPE_H
#define MICRORECIF_SHAPE_H

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
    Segment addAngle(double angle) const;
private:
    S2d point;
    double angle;
    double longueur;
    S2d secPoint;
};

double deltaAngle(Segment seg1, Segment seg2);
bool suppCommun(const Segment &seg1, const Segment &seg2, const Segment &newSeg,
                double inter=0);//renvoie true si les segs sont supperposés
bool suppIndep(Segment seg1, Segment seg2);//intersection de 2 segments independants

#endif //MICRORECIF_SHAPE_H
