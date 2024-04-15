//
// Created by royer on 06.04.2024.
//

#include <cmath>
#include "graphic_gui.h"
#include "constantes.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static double heightFactor, widthFactor;

int convEntity(Entity ent);

//fonction qui dessine les entités
void drawEntity (Entity entity, int x, int y, int size, double angle=0.)
{
    switch (entity)
    {
        case gCircle :
            *ptcr->set_line_width(2.0);
            *ptcr->set_source_rgb(0, 0.7, 0);
            *ptcr->move_to(x+size, y);
            *ptcr->arc(x,y,size,0,6.5);
            *ptcr->stroke();
            break;

        case mSegment :
            *ptcr->set_line_width(2.0);
            *ptcr->set_source_rgb(0, 0, 0.7);
            *ptcr->move_to(x, y);
            *ptcr->line_to(x+size*cos(angle),y+size*sin(angle));

            break;

        case rCircle :
            *ptcr->set_line_width(2.0);
            *ptcr->set_source_rgb(0.7, 0, 0);
            *ptcr->move_to(x+size, y);
            *ptcr->arc(x,y,size,0,6.5);
            *ptcr->stroke();
            break;
    }

}
//y sont prédéfinies une table de couleur et des fonctions pour dessiner les entitées


void set_ptcr(const Cairo::RefPtr<Cairo::Context>& cr)
{
    ptcr = &cr;
}

int convEntity(Entity ent)
{
    if (ent==ALGUE) return 0;
    if (ent==CORAIL) return 1;
    if (ent==SCAVENGER) return 2;
}