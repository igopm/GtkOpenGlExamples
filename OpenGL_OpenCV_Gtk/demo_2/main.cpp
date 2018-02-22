//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <gtkmm/application.h>
//=====================================================================================================
#include "MainWindow.h"
//=====================================================================================================
int main(int argc, char ** argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
                                                                  "org.gtkmm.demo_2");
    MainWindow mainWindow;

    return app->run(mainWindow);
}
//=====================================================================================================