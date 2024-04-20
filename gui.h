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
#include <gtkmm.h>
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
    //Simulation* simulation_;

private:
    S2d changeCoordinates (S2d newPoint, int width, int height);
    bool change;

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
    void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog);
    void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog);
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
    bool stop=false;
    void setCounters ();

};



#endif //MICRORECIF_GUI_H
