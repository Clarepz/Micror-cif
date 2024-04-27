//
// gui.h, Provenaz Clarence, Royer Yann
//

#ifndef MICRORECIF_GUI_H
#define MICRORECIF_GUI_H


#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/application.h>
#include <gtkmm/checkbutton.h>
#include "simulation.h"

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea();

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

class UserInterface : public Gtk::Window
{
public:
    UserInterface();
    static void setSimulation(char* inputFilename);
    static const Simulation& getSimulation();
protected:
    void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    bool on_timeout();

    //Button Signal handlers:
    void exitClicked();
    void openClicked();
    void saveClicked();
    void startClicked();
    void stepClicked();

    MyArea m_Area;

    Gtk::Box m_Main_Box;
    Gtk::Box cmd_et_infos; //contient une boite pour les boutons, et une pour les infos
    Gtk::Box button;
    Gtk::Box infos;
    Gtk::Box sujetsInfos; //contient les labels du style mise a jour, algue, etc ...
    Gtk::Box nbSujets; //dans cette boite le nombre de mise a jour, etc ...

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

    static Simulation simulation_ ;

private:
    Gtk::CheckButton algue;
    bool started=false;
    void setCounters();
    bool disconnect=false;
    bool saveMode;
};

#endif //MICRORECIF_GUI_H
