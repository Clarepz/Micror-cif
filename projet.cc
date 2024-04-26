//
//projet.cc, Provenaz Clarence 50%, Royer Yann 50%, verion 2
// conventions ok

#include <iostream>
#include "simulation.h"
#include "gui.h"

using namespace std;

int main(int argc, char* argv[]) {
    UserInterface::setSimulation(argv[1]);
    auto app = Gtk::Application::create();
    app->make_window_and_run<UserInterface>(1, argv);
    return(0);
}
