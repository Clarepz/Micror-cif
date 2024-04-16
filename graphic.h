//
// Created by royer on 06.04.2024.
//

#ifndef MICRORECIF_GRAPHIC_H
#define MICRORECIF_GRAPHIC_H

enum Shape{Circle, Segment, Square};
enum Color{Green, Blue, Black, Red};

void drawEntity (Shape shape,Color color, int x, int y, int size, double angle=0.);

#endif //MICRORECIF_GRAPHIC_H
