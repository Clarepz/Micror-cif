//
// Created by Clarence Provenaz on 14.03.2024.
//

#ifndef MICRORECIF_SHAPE_H
#define MICRORECIF_SHAPE_H
constexpr double epsil_zero(0.5) ;

struct S2d {double x=0.; double y=0.;};

class Segment
{
    S2d coor;
    double angle;

};
#endif //MICRORECIF_SHAPE_H
