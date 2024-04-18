//
//projet.cc, Provenaz Clarence, version 1
//

#include <iostream>
#include "simulation.h"
#include "gui.h"

using namespace std;

int main(int argc, char* argv[]){

    if(argc != 2) exit(EXIT_FAILURE);
    Simulation simulation(argv[1]); //initialise une simulation a partir du fichier d'entree
    simulation.saveAs("save.txt");

    auto app = Gtk::Application::create();
    app->make_window_and_run<MyEvent>(1, argv);
    return(0);
}
