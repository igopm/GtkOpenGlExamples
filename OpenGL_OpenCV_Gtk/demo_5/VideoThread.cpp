//
// Created by Igor Maschikevich on 10/31/2017.
//
//=====================================================================================================
#include "VideoThread.h"

VideoThread::VideoThread():
    thread(0),
    stopFlag(false){
    Glib::signal_idle().connect(sigc::bind_return(sigc::mem_fun(*this, &VideoThread::start), false));
}
void VideoThread::start() {
    thread = Glib::Thread::create(sigc::mem_fun(*this, &VideoThread::threadfunc), true);
}
void VideoThread::threadfunc()
{
    while (!stopFlag) {
        int ind;
        // Capture a frame
        frame = buf1.startWriting(ind);  // Request bufffer slot
        cap.read(frame);
        buf1.endWriting(ind);     // End buffer slot
    }
}
// main thread tells substread to stop
VideoThread::~VideoThread(){
    {
        stopFlag = true;
    }

    if (thread)
        thread->join();
    std::cerr << "Subthread joined\n";
}
