//
// Created by Igor Maschikevich on 10/31/2017.
//
//=====================================================================================================
#pragma once
#include <iostream>
#include <gtkmm.h>
#include <opencv2/opencv.hpp>
#include "SyncBuf.h"

class  VideoThread
{
public:
    VideoThread();
    virtual ~VideoThread();
    void start();
    void threadfunc();

    Glib::Dispatcher    signal_increment;
    Glib::Thread*       thread;
    bool stopFlag;

    cv::VideoCapture cap{0};
    cv::Mat frame;
    SyncBuf<cv::Mat> buf1;
};

