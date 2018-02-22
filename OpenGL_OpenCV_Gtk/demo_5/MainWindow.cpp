//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include "MainWindow.h"
using namespace std;
//=====================================================================================================
MainWindow::MainWindow():
    vBox(Gtk::ORIENTATION_VERTICAL, false),
    controlBox(Gtk::ORIENTATION_HORIZONTAL, false),
    button_close("_Close", /* mnemonic= */ true),
    button_one("_one", true),
    button_two("_two", true)

{
    //========================================================================================
    set_title("Main Window Demo 5");
    int const margin = 5;
    vBox.property_margin() = margin;
    vBox.set_spacing(margin);
    controlBox.set_spacing(margin);
    controlBox.set_hexpand(true);
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
    add(vBox);
    vBox.pack_start(ow, Gtk::PACK_EXPAND_WIDGET);
    vBox.add(controlBox);
    controlBox.add(button_one);
    controlBox.add(button_two);
    controlBox.add(button_close);
    //========================================================================================
    show_all_children();
}
//=====================================================================================================
MainWindow::~MainWindow()
{
}
//=====================================================================================================
void MainWindow::on_button_one(){
    cout << "one" << endl;
}
void MainWindow::on_button_two(){
    cout << "two" << endl;
}
void MainWindow::on_button_close(){
    hide(); //to close the application.
}
//=====================================================================================================
