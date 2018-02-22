//
// Created by Igor Maschikevich on 10/21/2017.
//
//=====================================================================================================
#include <iostream>
#include <gtkmm.h>
//=====================================================================================================
#include "OpenglWindow.h"
//=====================================================================================================
int main(int argc, char ** argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
                                                                  "org.gtkmm.demo_3");
    OpenglWindow mainWindow;
    return app->run(mainWindow);
}
//=====================================================================================================

