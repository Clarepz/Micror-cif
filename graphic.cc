//
// Created by royer on 06.04.2024.
//

#include <cmath>
#include "graphic_gui.h"
#include "constantes.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static double heightFactor, widthFactor;


//fonction qui dessine les entités
void drawShape (Shape shape,Color color, double x, double y, double size, double angle)
{
    (*ptcr)->set_line_width(2.0);
    switch (color)
    {
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
            (*ptcr)->set_line_width(5.0);
    }
    switch (shape)
    {
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
            (*ptcr)->line_to(x+size/2, y+size/2);
            (*ptcr)->stroke();
    }

}
//y sont prédéfinies une table de couleur et des fonctions pour dessiner les entitées


void set_ptcr(const Cairo::RefPtr<Cairo::Context>& cr)
{
    ptcr = &cr;
}

