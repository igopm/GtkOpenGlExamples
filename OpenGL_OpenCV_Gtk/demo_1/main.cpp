//
// Created by Igor Maschikevich on 10/20/2017.
//
//=====================================================================================================
#include <iostream>
#include <gtkmm.h>
//=====================================================================================================
#include "openglWindow.h"
#include "Shader.h"
//=====================================================================================================
int main(int argc, char ** argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.demo_1");
    OpenglWindow mainWindow;
    return app->run(mainWindow);
}
//=====================================================================================================

