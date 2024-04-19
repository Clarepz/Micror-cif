//
// Created by royer on 06.04.2024.
//

#ifndef MICRORECIF_GUI_H
#define MICRORECIF_GUI_H

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/checkbutton.h>
#include "simulation.h"

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea();
    virtual ~MyArea();

    void exit();
    void open();
    void save();
    void start();
    void step();


protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

private:
    S2d changeCoordinates (S2d newPoint, int width, int height);
    bool change;
};

class MyEvent : public Gtk::Window
{
public:
    MyEvent();

protected:
    //Button Signal handlers:
    void exitClicked();
    void openClicked();
    void saveClicked();
    void startClicked();
    void stepClicked();
    void algue_toggled();

    MyArea m_Area;

    Gtk::Box m_Main_Box;
    Gtk::Box button;
    Gtk::Button exit;
    Gtk::Button open;
    Gtk::Button save;
    Gtk::Button start;
    Gtk::Button step;

private:
    Gtk::CheckButton algue;
    bool stop=false;
};

#endif //MICRORECIF_GUI_H
