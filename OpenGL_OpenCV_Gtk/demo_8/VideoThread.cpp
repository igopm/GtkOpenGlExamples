//
// Created by Igor Maschikevich on 10/31/2017.
//
//=====================================================================================================
#include "VideoThread.h"
void VideoThread::threadfunc()
{
    while (!stopFlag) {
        int ind;
        // Capture a frame
        cv::Mat &frame = buf1.startWriting(ind);  // Request bufffer slot
        cap.read(frame);
        buf1.endWriting(ind);     // End buffer slot
    }
}

VideoThread::~VideoThread(){
    stopFlag = true;
    // Join all threads
    t.join();
}
