#pragma once
#include "VideoArea.h"

//// a progress bar that lives in the main thread
class MainWindow
{
public:
    MainWindow();
    void pong();

    VideoArea ping;
    Gtk::Window win;
    Gtk::ProgressBar pb;
};
