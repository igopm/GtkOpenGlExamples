//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include "MainWindow.h"
using namespace std;

//=====================================================================================================
MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app){
    //========================================================================================
    set_title("Main Window 8");
    set_resizable(false);
    set_icon_from_file("common_textures/photo.jpg");
    set_default_size(640, 480);
    int const margin = 5;
    controlBox.set_hexpand(true);
    button_close.set_hexpand(true);
    button_close.set_halign(Gtk::ALIGN_END);
    button_close.set_valign(Gtk::ALIGN_CENTER);

    ow.button_one.property_margin() = margin;
    ow.button_two.property_margin() = margin;
    ow.button_three.property_margin() = margin;
    button_close.property_margin() = margin;
    //========================================================================================
    button_close.signal_clicked().connect( sigc::mem_fun(*this,
                                                         &MainWindow::on_button_close));
    //========================================================================================
    add(vBox);
    //Define the actions:
    m_refActionGroup = Gio::SimpleActionGroup::create();
    //========================================================================================
    m_refActionGroup->add_action("one",
                                 sigc::mem_fun(ow, &OpenglWindow::on_button_one));
    m_refActionGroup->add_action("two",
                                 sigc::mem_fun(ow, &OpenglWindow::on_button_two));
    m_refActionGroup->add_action("three",
                                 sigc::mem_fun(ow, &OpenglWindow::on_button_three));
    //========================================================================================
    m_refActionGroup->add_action("close", sigc::mem_fun(*this,
                                                        &MainWindow::on_button_close));
    m_refActionGroup->add_action("open_help", sigc::mem_fun(*this,
                                                            &MainWindow::on_button_close));
    m_refActionGroup->add_action("about", sigc::mem_fun(*this,
                                                        &MainWindow::on_button_about));
    //========================================================================================
    insert_action_group("example", m_refActionGroup);

    //Define how the actions are presented in the menus and toolbars:
    m_refBuilder = Gtk::Builder::create();
    //========================================================================================
    //Layout the actions in a menubar and toolbar:
    const char* ui_info =
            "<interface>"
            "  <menu id='menubar'>"
            "    <submenu>"
            "      <attribute name='label' translatable='yes'>_Textures</attribute>"
            "      <section>"
            "      <item>"
            "        <attribute name='label' translatable='yes'>One</attribute>"
            "        <attribute name='action'>example.one</attribute>"
            "        <attribute name='accel'>&lt;Primary&gt;1</attribute>"
            "      </item>"
            "      <item>"
            "        <attribute name='label' translatable='yes'>_Two</attribute>"
            "        <attribute name='action'>example.two</attribute>"
            "        <attribute name='accel'>&lt;Primary&gt;2</attribute>"
            "      </item>"
            "      <item>"
            "        <attribute name='label' translatable='yes'>_Three</attribute>"
            "        <attribute name='action'>example.three</attribute>"
            "        <attribute name='accel'>&lt;Primary&gt;3</attribute>"
            "      </item>"
            "      </section>"
            "      <section>"
            "        <item>"
            "          <attribute name='label' translatable='yes'>_Close</attribute>"
            "          <attribute name='action'>example.close</attribute>"
            "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
            "        </item>"
            "      </section>"
            "    </submenu>"

            "    <submenu>"
            "      <attribute name='label' translatable='yes'>_Certificate</attribute>"
            "      <section>"
            "        <item>"
            "          <attribute name='label' translatable='yes'>_Open help</attribute>"
            "          <attribute name='action'>example.open_help</attribute>"
            //"        <attribute name='accel'>&lt;Primary&gt;</attribute>"
            "        </item>"
            "      </section>"
            "      <section>"
            "        <item>"
            "          <attribute name='label' translatable='yes'>_About</attribute>"
            "          <attribute name='action'>example.about</attribute>"
            //            "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
            "        </item>"
            "      </section>"
            "    </submenu>"
            "  </menu>"
            "</interface>";
    //========================================================================================
    app->set_accel_for_action("example.one", "<Primary>1");
    app->set_accel_for_action("example.two", "<Primary>2");
    app->set_accel_for_action("example.three", "<Primary>3");
    app->set_accel_for_action("example.close", "<Primary>q");

    try{
        m_refBuilder->add_from_string(ui_info);
        //        m_refBuilder->add_from_resource("/toolbar/toolbar.glade");
    }
    catch(const Glib::Error& ex){
        std::cerr << "Building menus and toolbar failed: " <<  ex.what();
    }

    //Get the menubar:
    auto object = m_refBuilder->get_object("menubar");
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    if (!gmenu)
        g_warning("GMenu not found");
    else{
        auto pMenuBar = Gtk::manage(new Gtk::MenuBar(gmenu));

        //Add the MenuBar to the window:
        vBox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
    }
    //=====================================================================================================
    vBox.pack_start(ow, Gtk::PACK_EXPAND_WIDGET);
    vBox.add(controlBox);
    controlBox.add(ow.button_one);
    controlBox.add(ow.button_two);
    controlBox.add(ow.button_three);
    controlBox.add(button_close);

    show_all_children();
}
////=====================================================================================================
void MainWindow::on_button_close(){
    hide(); //to close the application.
}
////=====================================================================================================
void MainWindow::on_button_about(){
    Gtk::AboutDialog m_Dialog;
    m_Dialog.set_icon_from_file("common_textures/photo.jpg");
    m_Dialog.set_version("1.0.0");
    m_Dialog.set_website("https://www.google.com.ua");
    m_Dialog.set_website_label("Kaa Solutions website");
    m_Dialog.set_comments("This is GTK program using OpenGL and OpenCV.");
    m_Dialog.set_copyright("ⓒ 2017 The Kaa Solutions Team");
    m_Dialog.run();
}
