#include "VideoArea.h"

VideoArea::VideoArea():
    thread(0),
    value(0),
    threadMustStop(false) {
    Glib::signal_idle().connect(sigc::bind_return(sigc::mem_fun(*this, &VideoArea::start), false));
}
void VideoArea::start() {
    std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";
    thread = Glib::Thread::create(sigc::mem_fun(*this, &VideoArea::threadfunc), true);
}
void VideoArea::threadfunc()
{
//    //    std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";
//    while (!stopFlag) {
//        int ind;

//        // Capture a frame
//        Mat &frame = buf1.startWriting(ind);  // Request bufffer slot
//        cap.read(frame);
//        buf1.endWriting(ind);     // End buffer slot
//        if (threadMustStop) break;
//    }
}
//    std::cerr << "Subthread stop\n";
////////////////////////////////////////////////////////////////////////////


// called by main thread to interogate state of subthread
int VideoArea::get_value()
{
    std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";
    int ret = 0;

    {
        Glib::Mutex::Lock lock (mutex);
        ret = value;
    }
    return ret;
}

// main thread tells substread to stop
VideoArea::~VideoArea(){
    {
        Glib::Mutex::Lock lock (mutex);
        threadMustStop = true;
    }

    if (thread)
        thread->join();
    std::cerr << "Subthread joined\n";
}
