//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include "MainWindow.h"
//=====================================================================================================
MainWindow::MainWindow():
    VBox(Gtk::ORIENTATION_VERTICAL, false),
    ControlBox(Gtk::ORIENTATION_HORIZONTAL, false),
    button_close("_Close", /* mnemonic= */ true),
    button_one("_one", true),
    button_two("_two", true)
{
    //========================================================================================
    set_title("Main Window. Demo 2");
    int const margin = 5;
    VBox.property_margin() = margin;
    VBox.set_spacing(margin);
    ControlBox.set_spacing(margin);
    ControlBox.set_hexpand(true);
    button_close.set_hexpand(true);
    button_close.set_halign(Gtk::ALIGN_END);
    button_close.set_valign(Gtk::ALIGN_CENTER);
    //========================================================================================
    button_one.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &MainWindow::on_button_one));
    button_two.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &MainWindow::on_button_two));
    button_close.signal_clicked().connect( sigc::mem_fun(*this,
                                                         &MainWindow::on_button_close));
    //========================================================================================
    add(VBox);
    VBox.pack_start(ow, Gtk::PACK_EXPAND_WIDGET);
    VBox.add(ControlBox);
    ControlBox.add(button_one);
    ControlBox.add(button_two);
    ControlBox.add(button_close);
    //========================================================================================
    show_all_children();
}
//=====================================================================================================
void MainWindow::on_button_one(){
    std::cout << "one" << std::endl;
}
void MainWindow::on_button_two(){
    std::cout << "two" << std::endl;
}
void MainWindow::on_button_close(){
    hide(); //to close the application.
}
//=====================================================================================================
