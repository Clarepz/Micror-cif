//
// gui.cc, Provenaz Clarence 10%, Royer Yann 90%, version 1
//

#include <cairomm/context.h>
#include <iostream>
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm/main.h>
#include <map>
#include "gui.h"
#include "graphic_gui.h"

constexpr int area_side(400);
constexpr int timeoutValue(25);

MyArea::MyArea()
{
    set_content_width(area_side);
    set_content_height(area_side);
    set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
    set_hexpand(true);
}

MyArea::~MyArea() {}


void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    set_ptcr(cr);
    double ratio=double(width)/height;
    double xMax(dmax), xMin(0), yMax(dmax), yMin(0), delta(dmax), middle(dmax/2);
    //prevent distortion
    if(ratio > 1)
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
    cr->scale(width/(xMax-xMin), -height/(yMax - yMin));
    cr->translate(-(xMin + xMax)/2., -(yMin + yMax)/2.);
    drawEntity(SQUARE, GREY, {middle, middle}, 256); //bord du monde

    UserInterface::getSimulation()->display();

}


UserInterface::UserInterface():
        m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
        cmd_et_infos(Gtk::Orientation::VERTICAL,5),
        button(Gtk::Orientation::VERTICAL, 2),
        infos(Gtk::Orientation::HORIZONTAL,5),
        sujetsInfos(Gtk::Orientation::VERTICAL,5),
        nbSujets(Gtk::Orientation::VERTICAL, 5),
        exit("Exit"),
        open("Open"),
        save("Save"),
        start("Start"),
        step("Step"),
        algue("Naissance d'algue"),
        titre1("Commandes"),
        titre2("Informations sur la simulation :     "),
        miseAJour("Nombre de mises à jour : "),
        label_Algue("Nombre d'algues : "),
        corail("Nombre de coraux : "),
        charognards("Nombre de charognards :  "),
        nbMiseAJour(std::to_string(simulation_.getNbSim())),
        nbAlgue(std::to_string(simulation_.getNbAlg())),
        nbCorail(std::to_string(simulation_.getNbCor())),
        nbCharognards(std::to_string(simulation_.getNbSca())),
        disconnect(false) // to handle a single timer
        //simulation_(simulation)
{
    set_title("Microrecif");
    set_resizable(true);
    set_child(m_Main_Box);

    m_Main_Box.append(cmd_et_infos);
    m_Main_Box.append(m_Area);
    cmd_et_infos.append(titre1);
    cmd_et_infos.append(button);
    cmd_et_infos.append(titre2);
    cmd_et_infos.append(infos);
    infos.append(sujetsInfos);
    infos.append(nbSujets);

    button.append(exit);
    button.append(open);
    button.append(save);
    button.append(start);
    button.append(step);
    button.append(algue);

    sujetsInfos.append(miseAJour);
    sujetsInfos.append(label_Algue);
    sujetsInfos.append(corail);
    sujetsInfos.append(charognards);

    nbSujets.append(nbMiseAJour);
    nbSujets.append(nbAlgue);
    nbSujets.append(nbCorail);
    nbSujets.append(nbCharognards);

    exit.signal_clicked().connect(sigc::mem_fun(*this, &UserInterface::exitClicked));

    open.signal_clicked().connect(sigc::mem_fun(*this, &UserInterface::openClicked));

    save.signal_clicked().connect(sigc::mem_fun(*this, &UserInterface::saveClicked));

    start.signal_clicked().connect(sigc::mem_fun(*this, &UserInterface::startClicked));

    step.signal_clicked().connect(sigc::mem_fun(*this, &UserInterface::stepClicked));

    algue.signal_toggled().connect(sigc::mem_fun(*this,&UserInterface::algue_toggled));

    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
            sigc::mem_fun(*this, &UserInterface::on_window_key_pressed), false);
    add_controller(controller);

}

void UserInterface::setSimulation(Simulation(& simulation)) {
    simulation_=simulation;
}

Simulation* UserInterface::getSimulation() {
    return &simulation_;
}

void UserInterface::on_file_dialog_response(int response_id,
                                            Gtk::FileChooserDialog* dialog)
{
    //Handle the response:
    switch (response_id)
    {
        case Gtk::ResponseType::OK:
        {
            std::cout << "Open or Save clicked." << std::endl;
            auto filename = dialog->get_file()->get_path();
            std::cout << "File selected: " <<  filename << std::endl;
            char* fileName = &filename[0];
            if(saveMode)
            {
                simulation_.saveAs(fileName);
                break;
            }
            else //open mode
            {
                Simulation newSimulation(fileName);
                setSimulation(newSimulation);
                setCounters();
                m_Area.queue_draw();
            }
        }
        case Gtk::ResponseType::CANCEL:
        {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default:
        {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
    delete dialog;
}

bool UserInterface::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
    switch(gdk_keyval_to_unicode(keyval))
    {
        case 's':
            startClicked();
            return true;
            break;

        case '1':
            stepClicked();
            return true;
            break;
    }
    return false;//in case of another key
}

bool UserInterface::on_timeout()
{
    if(disconnect)
    {
        disconnect = false;
        return false;
    }
    simulation_.update(algue.get_active());
    m_Area.queue_draw();
    setCounters();
    return true;
}

void UserInterface::exitClicked()
{
    hide();
}

void UserInterface::openClicked()
{
    saveMode=false;
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &UserInterface::on_file_dialog_response), dialog));

    //Add response buttons to the dialog:
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    //Show the dialog and wait for a user response:
    dialog->show();

}

void UserInterface::saveClicked()
{
    saveMode=true;
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &UserInterface::on_file_dialog_response), dialog));

    //Add response buttons to the dialog:
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    //Show the dialog and wait for a user response:
    dialog->show();
}

void UserInterface::startClicked()
{
    if (started)
    {
        start.set_label("Start");
        disconnect  = true;
    }
    else
    {
        start.set_label("Stop");
        sigc::slot<bool()> my_slot = sigc::bind(
                sigc::mem_fun(*this,&UserInterface::on_timeout));

        auto conn = Glib::signal_timeout().connect(my_slot,timeoutValue);
    }
    started=not started;
}

void UserInterface::stepClicked()
{
    if (not started)
    {
        simulation_.update(algue.get_active());
        m_Area.queue_draw();
        setCounters();
    }
}

void UserInterface::algue_toggled()
{

}

Simulation UserInterface::simulation_ = Simulation();

void UserInterface::setCounters ()
{
    nbMiseAJour.set_text(std::to_string(simulation_.getNbSim()));
    nbAlgue.set_text(std::to_string(simulation_.getNbAlg()));
    nbCorail.set_text(std::to_string(simulation_.getNbCor()));
    nbCharognards.set_text(std::to_string(simulation_.getNbSca()));
}


