//
// Created by Igor Maschikevich on 10/25/2017.
// Test
//=====================================================================================================
#pragma once
#include <iostream>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/aboutdialog.h>
//=====================================================================================================
#include "OpenglWindow.h"
//=====================================================================================================
class MainWindow : public Gtk::ApplicationWindow
{
public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& app);
    OpenglWindow ow;
    //protected:
    //========================================================================================
    //Signal handlers:
    void on_button_close();
    void on_button_about();
    //========================================================================================
    Gtk::Box vBox {Gtk::ORIENTATION_VERTICAL};
    Gtk::Box controlBox{Gtk::ORIENTATION_HORIZONTAL, false};
    Gtk::Button button_close{"_Close", /* mnemonic= */ true};
private:
    //Child widgets:
    Gtk::Box m_Box;

    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
    Glib::RefPtr<Gio::SimpleAction> m_refActionRain;

    //========================================================================================
};
