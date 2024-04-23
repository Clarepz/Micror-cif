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
#include <gtkmm/filechooserdialog.h>
#include <map>
#include "simulation.h"

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea();
    virtual ~MyArea();



protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    //Simulation* simulation_;

private:

};

class MyEvent : public Gtk::Window
{
public:
    //MyEvent(Simulation& simulation);
    MyEvent();

    static void setSimulation(Simulation& simulation);
    static Simulation* getSimulation();

protected:
    //Button Signal handlers:
    void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    bool on_timeout();
    void exitClicked();
    void openClicked();
    void saveClicked();
    void startClicked();
    void stepClicked();
    void algue_toggled();

    MyArea m_Area;

    Gtk::Box m_Main_Box;
    Gtk::Box button;
    Gtk::Box cmd_et_infos;
    Gtk::Box infos;
    Gtk::Box sujetsInfos;
    Gtk::Box nbSujets;
    Gtk::Button exit;
    Gtk::Button open;
    Gtk::Button save;
    Gtk::Button start;
    Gtk::Button step;

    Gtk::Label titre1;
    Gtk::Label titre2;
    Gtk::Label miseAJour;
    Gtk::Label label_Algue;
    Gtk::Label corail;
    Gtk::Label charognards;
    Gtk::Label nbMiseAJour;
    Gtk::Label nbAlgue;
    Gtk::Label nbCorail;
    Gtk::Label nbCharognards;


    static Simulation simulation_;




private:
    Gtk::CheckButton algue;
    bool started=false;
    void setCounters();
    bool disconnect=false;
    bool saveMode;

};



#endif //MICRORECIF_GUI_H
