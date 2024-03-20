//
// Created by royer on 15.03.2024.
//

#ifndef MICRORECIF_SHAPE_H
#define MICRORECIF_SHAPE_H
constexpr double epsil_zero(0.5) ;

struct S2d {double x=0.; double y=0.;};

enum fail {NOFAIL, NEGLENGHT, BADANGLE};
enum posSeg {colineaire, antiTrigo, trigo};

class Segment{
public:
    Segment(S2d coor_,double angle_, unsigned longueur_);//constructor
    S2d getPoint() const;
    S2d getSecPoint() const;
    double getlength() const;
    double getAngle() const;
    fail getFail() const;
    Segment addAngle(double angle) const;
private:
    S2d point;
    double angle;
    double longueur;
    S2d secPoint;
};

//j' ai mis en fonction
double deltaAngle(Segment seg1, Segment seg2);
bool suppCommun(Segment seg1, Segment seg2, Segment newSeg, double delta_rot=0);//superposition de 2 segments ayant un point commun
bool suppIndep(Segment seg1, Segment seg2, bool sim=true);//intersection de 2 segments independants

#endif //MICRORECIF_SHAPE_H
