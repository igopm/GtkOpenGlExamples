//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#pragma once
#include <iostream>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
//=====================================================================================================
#include "OpenglWindow.h"
//=====================================================================================================
class MainWindow : public Gtk::Window{
public:
    MainWindow();
protected:
    //========================================================================================
    //Signal handlers:
    void on_button_one();
    void on_button_two();
    void on_button_close();
    //========================================================================================
    Gtk::Box VBox;

    OpenglWindow ow;

    Gtk::Box ControlBox;
    Gtk::Button button_close;
    Gtk::Button button_one;
    Gtk::Button button_two;
    //========================================================================================
};
