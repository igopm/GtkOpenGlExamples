//
// Created by Igor Maschikevich on 11/06/2017.
//
//=====================================================================================================
#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <utility>
#include <chrono>
#include <string>
//#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
//=====================================================================================================
#include "settings.h"
#include "SyncBuf.h"
#include "MarkerDataBlock.h"
//=====================================================================================================
class OtherThreads
{
public:
    OtherThreads(std::string udpHostE, std::string udpPortE);
    virtual ~OtherThreads();
private:
    std::thread receiveThread;
    std::thread decodeThread;

    std::string udpHost;
    std::string udpPort;
    std::atomic<bool> stopFlag;

    // The counters
    int countReceived;
    int countDecoded;
    int countShown;
    int countGL;
    using SupData = Dronestreamer::MarkerDataBlock;
    SyncBuf<std::pair<SupData, cv::Mat>> buf2;
};
