//
// graphic.h, royer yann, verion 1
// conventions ok

#ifndef MICRORECIF_GRAPHIC_H
#define MICRORECIF_GRAPHIC_H

enum Shape{CIRCLE, LINE, SQUARE};
enum Color{GREEN, BLUE, BLACK, RED, GREY};

//fonction qui dessine les entités
void drawShape(Shape shape,Color color, double x, double y, double size, double angle);

#endif //MICRORECIF_GRAPHIC_H
