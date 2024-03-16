//
// Created by royer on 15.03.2024.
//

#ifndef MICRORECIF_SHAPE_H
#define MICRORECIF_SHAPE_H
constexpr double epsil_zero(0.5) ;

struct S2d {double x=0.; double y=0.;};

class Segment{
public:
    Segment(S2d coor_,double angle_, double longueur_);//constructor
    S2d getSecPoint();
    double dAngle (Segment seg1, Segment seg2);
    bool suppCommun(Segment seg1, Segment seg2, bool sim=true);//superposition de 2 segments ayant un point commun
    bool suppIndep(Segment seg1, Segment seg2, bool sim=true);//intersection de 2 segments independants
private:
    S2d coor;
    double angle;
    double longueur;
    S2d secPoint;
};
#endif //MICRORECIF_SHAPE_H
