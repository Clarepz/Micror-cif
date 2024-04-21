//
// gui.cc, Provenaz Clarence 10%, Royer Yann 90%
//

#include <cairomm/context.h>
#include <iostream>
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
    double xMax(dmax), xMin(0), yMax(dmax), yMin(0), delta(dmax), middle(dmax/2);
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
    drawEntity(SQUARE, GREY, {middle, middle}, 256);


    MyEvent::getSimulation()->display();


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
{
    queue_draw();
}

MyEvent::MyEvent():
        m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
        button(Gtk::Orientation::VERTICAL, 2),
        cmd_et_infos(Gtk::Orientation::VERTICAL,5),
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
        nbCharognards(std::to_string(simulation_.getNbSca()))
        //simulation_(simulation)
{
    set_title("Microrecif");
    set_resizable(true);
    set_child(m_Main_Box);

    m_Main_Box.append(cmd_et_infos);
    m_Main_Box.append(m_Area);
    cmd_et_infos.append(button);
    cmd_et_infos.append(titre2);
    cmd_et_infos.append(infos);
    infos.append(sujetsInfos);
    infos.append(nbSujets);

    button.append(titre1);
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

    exit.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::exitClicked));

    open.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::openClicked));

    save.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::saveClicked));

    start.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::startClicked));

    step.signal_clicked().connect(sigc::mem_fun(*this, &MyEvent::stepClicked));

    algue.signal_toggled().connect(sigc::mem_fun(*this,&MyEvent::algue_toggled));

}

void MyEvent::on_file_dialog_response_save(int response_id,
                                            Gtk::FileChooserDialog* dialog)
{
    //Handle the response:
    switch (response_id)
    {
        case Gtk::ResponseType::OK:
        {
            std::cout << "Open or Save clicked." << std::endl;

            //Notice that this is a std::string, not a Glib::ustring.
            auto filename = dialog->get_file()->get_path();
            std::cout << "File selected: " <<  filename << std::endl;
            char* fileName = &filename[0];
            simulation_.saveAs(fileName);
            break;
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

void MyEvent::on_file_dialog_response_open(int response_id,
                                           Gtk::FileChooserDialog* dialog)
{
    //Handle the response:
    switch (response_id)
    {
        case Gtk::ResponseType::OK:
        {
            std::cout << "Open or Save clicked." << std::endl;

            //Notice that this is a std::string, not a Glib::ustring.
            auto filename = dialog->get_file()->get_path();
            std::cout << "File selected: " <<  filename << std::endl;
            char* fileName = &filename[0];
            Simulation newSimulation(fileName);
            setSimulation(newSimulation);
            setCounters();
            m_Area.step();
            //newSimulation.display();
            break;
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

void MyEvent::setSimulation(Simulation(& simulation)) {
    simulation_=simulation;
    //&MyEvent::setCounters;
}

Simulation* MyEvent::getSimulation() {
    return &simulation_;
}

void MyEvent::exitClicked()
{
    m_Area.exit();
}

void MyEvent::openClicked()
{
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &MyEvent::on_file_dialog_response_open), dialog));

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

void MyEvent::saveClicked()
{
    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &MyEvent::on_file_dialog_response_save), dialog));

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

void MyEvent::startClicked()
{
    if (!stop)
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
    simulation_.update(algue.get_active());
    m_Area.step();
    setCounters();
}

void MyEvent::algue_toggled()
{

}

Simulation MyEvent::simulation_ = Simulation();

void MyEvent::setCounters ()
{
    nbMiseAJour.set_text(std::to_string(simulation_.getNbSim()));
    nbAlgue.set_text(std::to_string(simulation_.getNbAlg()));
    nbCorail.set_text(std::to_string(simulation_.getNbCor()));
    nbCharognards.set_text(std::to_string(simulation_.getNbSca()));
    //nbCharognards.set_text("123");
}


