//
// Created by royer on 06.04.2024.
//

#ifndef MICRORECIF_GRAPHIC_H
#define MICRORECIF_GRAPHIC_H

enum Shape{CIRCLE, LINE, SQUARE};
enum Color{GREEN, BLUE, BLACK, RED, GREY};

void drawEntity(Shape shape,Color color, double x, double y, double size, double angle=0.);

#endif //MICRORECIF_GRAPHIC_H
