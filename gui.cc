//
// Created by royer on 06.04.2024.
//

#include <cairomm/context.h>
#include "gui.h"
#include "graphic_gui.h"

constexpr int area_side(400);

MyArea::MyArea(): change(false)
{
    set_content_width(area_side);
    set_content_height(area_side);

    set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
    set_hexpand(true);
}

MyArea::~MyArea()
{
}


void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    set_ptcr(cr);
    double ratio=double(width)/height;
    double xMax(255), xMin(0), yMax(255), yMin(0), delta(255), middle(255./2);
    //prevent distortion
    if( ratio > 1)
    {
        xMax = middle + 0.5*ratio*delta ;
        xMin = middle - 0.5*ratio*delta ;
    }
    else
    {
        yMax = middle + 0.5/ratio*delta ;
        yMin = middle - 0.5/ratio*delta ;
    }
    cr->translate(width/2., height/2.);
    cr->scale(double(width/(xMax-xMin)), double(-height/(yMax - yMin)));
    cr->translate(-(xMin + xMax)/2., -(yMin + yMax)/2.);
    drawEntity(SQUARE, GREY, 127.5, 127.5, 255);
    drawEntity(LINE, RED, 0, 0, 200, 0.785);
    //if(change)
    //{
        //Simulation.display();
    //}

}

S2d changeCoordinates (S2d oldPoint, int width, int height)
{
}

void MyArea::exit()
{
    change = true;
    queue_draw();
}

void MyArea::open()
{
    change = false;
    queue_draw();
}

void MyArea::save()
{}

void MyArea::start()
{}

void MyArea::step()
{}

MyEvent::MyEvent():
        m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
        button(Gtk::Orientation::VERTICAL, 2),
        exit("Exit"),
        open("Open"),
        save("Save"),
        start("Start"),
        step("Step"),
        algue("Naissance d'algue"),
        titre1("Commandes"),
        titre2("Informations sur la simulation :"),
        miseAJour("Nombre de mises à jour : "),
        label_Algue("Nombre d'algues : "),
        corail("Nombre de coraux : "),
        charognards("Nombre de charognards : ")
{
    set_title("Microrecif");
    set_resizable(true);
    set_child(m_Main_Box);

    m_Main_Box.append(button);
    m_Main_Box.append(m_Area);

    button.append(exit);
    button.append(open);
    button.append(save);
    button.append(start);
    button.append(step);
    button.append(algue);

    exit.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::exitClicked));

    open.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::openClicked));

    save.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::saveClicked));

    start.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::startClicked));

    step.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::stepClicked));

    algue.signal_toggled().connect(sigc::mem_fun(*this,&MyEvent::algue_toggled));
}

void MyEvent::exitClicked()
{
    m_Area.exit();
}

void MyEvent::openClicked()
{
    m_Area.open();
}

void MyEvent::saveClicked()
{
    m_Area.save();
}

void MyEvent::startClicked()
{
    if (stop==false)
    {
        stop=true;
        start.set_label("Stop");
    }
    else
    {
        stop=false;
        start.set_label("Start");
    }
}

void MyEvent::stepClicked()
{
    m_Area.step();
}

void MyEvent::algue_toggled()
{

}
