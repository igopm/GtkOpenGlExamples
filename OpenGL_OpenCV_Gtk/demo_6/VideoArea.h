#pragma once
#include <iostream>
#include <gtkmm.h>
class VideoArea
{
public:
    VideoArea();
    virtual ~VideoArea();
    void start();
    void threadfunc();
    int get_value();
    Glib::Dispatcher    signal_increment;
    Glib::Thread*       thread;
    Glib::Mutex     mutex; // protects both the following variables
    int value;
    bool threadMustStop;
    bool stopFlag = false;
};
