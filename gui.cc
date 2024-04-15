//
// Created by royer on 06.04.2024.
//

#include <cairomm/context.h>
#include "gui.h"
#include "graphic_gui.h"

constexpr int area_side(200);

MyArea::MyArea(): empty(false)
{
    set_content_width(area_side);
    set_content_height(area_side);

    set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
}

MyArea::~MyArea()
{
}


void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    set_ptcr(cr);
    if(not empty)
    {
        // coordinates for the center of the window
        int xc, yc;
        xc = width / 2;
        yc = height / 2;

        cr->set_line_width(10.0);

        // draw red lines out from the center of the window
        cr->set_source_rgb(0.8, 0.0, 0.0);
        cr->move_to(0, 0);
        cr->line_to(xc, yc);
        cr->line_to(0, height);
        cr->move_to(xc, yc);
        cr->line_to(width, yc);
        cr->stroke();
    }

}



MyEvent::MyEvent():
        m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
        button(Gtk::Orientation::VERTICAL, 2),
        exit("Exit"),
        open("Open"),
        save("Save"),
        start("Start"),
        step("Step")
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

    exit.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::exitClicked));

    open.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::openClicked));

    save.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::saveClicked));

    start.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::startClicked));

    step.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::stepClicked));
}

void MyEvent::exitClicked()
{

}

void MyEvent::openClicked()
{
    
}

void MyEvent::saveClicked()
{}

void MyEvent::startClicked()
{}

void MyEvent::stepClicked()
{}
