//
// Created by royer on 06.04.2024.
//

#include <cmath>
#include "graphic_gui.h"
#include "constantes.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static double heightFactor, widthFactor;


//fonction qui dessine les entités
void drawEntity (Shape shape,Color color, int x, int y, int size, double angle)
{
    (*ptcr)->set_line_width(2.0);
    switch (color)
    {
        case Green:
            (*ptcr)->set_source_rgb(0, 0.7, 0);
            break;

        case Blue:
            (*ptcr)->set_source_rgb(0, 0, 0.7);
            break;

        case Black:
            (*ptcr)->set_source_rgb(0, 0, 0);
            break;

        case Red:
            (*ptcr)->set_source_rgb(0.7, 0, 0);
            break;

        case Grey:
            (*ptcr)->set_source_rgb(0.5,0.5,0.5);
            (*ptcr)->set_line_width(5.0);
    }
    switch (shape)
    {
        case Circle :
            (*ptcr)->move_to(x+size, y);
            (*ptcr)->arc(x,y,size,0,6.5);
            (*ptcr)->stroke();
            break;

        case Line :
            (*ptcr)->move_to(x, y);
            (*ptcr)->line_to(x+size*cos(angle),y+size*sin(angle));
            (*ptcr)->stroke();
            break;

        case Square:
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

