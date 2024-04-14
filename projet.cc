//
//projet.cc, Provenaz Clarence, version 1
//

#include <iostream>
#include "simulation.h"
#include "gui.h"

using namespace std;

int main(int argc, char* argv[]){
    //if(argc != 2) exit(0);
    //Simulation simulation(argv[1]); //initialise une simulation a partir du fichier d'entree
    auto app = Gtk::Application::create();
    app->make_window_and_run<MyEvent>(1, argv);
    return(0);
}
