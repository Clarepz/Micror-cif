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
        S2d p1{1,2};
        S2d p2{3,4};
        std::cout<<(p1+p2).x<<"ET"<<(p1+p2).y;
        exit(0);
    auto app = Gtk::Application::create();
    app->make_window_and_run<MyEvent>(1, argv);
    return(0);
}
