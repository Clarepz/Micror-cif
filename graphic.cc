//
//graphic.cc, Provenaz Clarence 0%, Royer Yann 100%, verion 2
//conventions ok

#include <cmath>
#include "graphic_gui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void drawShape(Shape shape,Color color,double x, double y, double size, double angle) {
    (*ptcr)->set_line_width(1.0);
    switch (color) {
        case GREEN:
            (*ptcr)->set_source_rgb(0, 0.7, 0);
            break;
        case BLUE:
            (*ptcr)->set_source_rgb(0, 0, 0.7);
            break;
        case BLACK:
            (*ptcr)->set_source_rgb(0, 0, 0);
            break;
        case RED:
            (*ptcr)->set_source_rgb(0.7, 0, 0);
            break;
        case GREY:
            (*ptcr)->set_source_rgb(0.5,0.5,0.5);
    }
    switch (shape) {
        case CIRCLE :
            (*ptcr)->move_to(x+size, y);
            (*ptcr)->arc(x,y,size,0,6.5);
            (*ptcr)->stroke();
            break;

        case LINE :
            (*ptcr)->move_to(x, y);
            (*ptcr)->line_to(x+size*cos(angle),y+size*sin(angle));
            (*ptcr)->stroke();
            break;

        case SQUARE:
            (*ptcr)->move_to(x+size/2, y+size/2);
            (*ptcr)->line_to(x-size/2, y+size/2);
            (*ptcr)->line_to(x-size/2, y-size/2);
            (*ptcr)->line_to(x+size/2, y-size/2);
            (*ptcr)->line_to(x+size/2, y+size/2+(*ptcr)->get_line_width()/2);
            (*ptcr)->stroke();
    }
}

void set_ptcr(const Cairo::RefPtr<Cairo::Context>& cr)
{
    ptcr = &cr;
}

