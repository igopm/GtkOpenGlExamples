//
// Created by Igor Maschikevich on 10/31/2017.
//
//=====================================================================================================
#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <opencv2/opencv.hpp>
//=====================================================
#include <gtkmm.h>
#include "SyncBuf.h"
class VideoThread
{
public:
    virtual ~VideoThread();
    void threadfunc();
    std::atomic<bool> stopFlag{false};
    cv::VideoCapture cap{0};
    SyncBuf<cv::Mat> buf1;
    std::thread t{[this]()->void{
            this->threadfunc();
        }};
};

