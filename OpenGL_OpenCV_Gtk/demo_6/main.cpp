#include "MainWindow.h"
#include "VideoArea.h"
#include <iostream>
#include <gtkmm.h>

//// an object that lives in a subthread (mostly)
//class pinger
//{
//public:
//    pinger() : thread(0), value(0), threadMustStop(false) {
//        // deferr creation of the thread until main loop has some time
//        // It is not entirely clear why this is needed, but the documentation says so
//        Glib::signal_idle().connect(sigc::bind_return(sigc::mem_fun(*this, &pinger::start), false));
//    }

//    // called by main thread to create subthread
//    void start() {
//        std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";
//        thread = Glib::Thread::create(sigc::mem_fun(*this, &pinger::threadfunc), true);
//    }

//    // main subthread function - called on the stack of the subthread (obviously)
//    void threadfunc()
//    {
//        std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";

//        for(;;)
//        {
//            Glib::usleep(50000);

//            {
//                Glib::Mutex::Lock lock (mutex);
//                value++;
//                value = value % 100;
//            }

//            std::cerr << "signal_increment called by " << Glib::Thread::self() << "\n";
//            signal_increment();

//            // less than ideal: thread only stops when it notices flag has changed
//            {
//                Glib::Mutex::Lock lock (mutex);
//                if (threadMustStop) break;
//            }
//        }
//        std::cerr << "Subthread stop\n";
//    }

//    // called by main thread to interogate state of subthread
//    int get_value()
//    {
//        std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << "\n";
//        int ret = 0;

//        {
//            Glib::Mutex::Lock lock (mutex);
//            ret = value;
//        }
//        return ret;
//    }

//    // main thread tells substread to stop
//    ~pinger()
//    {
//        {
//            Glib::Mutex::Lock lock (mutex);
//            threadMustStop = true;
//        }

//        if (thread)
//            thread->join();
//        std::cerr << "Subthread joined\n";
//    }

//    Glib::Dispatcher    signal_increment;
//    Glib::Thread*       thread;
//    Glib::Mutex     mutex; // protects both the following variables
//    int value;
//    bool threadMustStop;
//};

//// a progress bar that lives in the main thread
//class pbar
//{
//public:
//    pbar()
//    {
//        win.add(pb);
//        pb.show();
//        win.show();

//        pb.set_fraction(0.0);
//        ping.signal_increment.connect(sigc::mem_fun(*this, &pbar::pong));
//    }

//    // invoked on stack of *main thread* by signal from subthread (magic)
//    // so *can* call normal gtkmm GUI functions (like set_fraction)
//    void pong()
//    {
//        int val = ping.get_value();
//        std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << " value = " << val << "\n";
//        pb.set_fraction(float(val) / float(100.0));
//        //        if (val >= 99)
//        //            Gtk::Main::quit();
//    }

//    pinger ping;
//    Gtk::Window win;
//    Gtk::ProgressBar pb;
//};

//int main(int argc, char *argv[])
//{
//    if(!Glib::thread_supported()) Glib::thread_init();

//    Gtk::Main app(argc, argv);

//    pbar pb;

//    app.run();
//}
int main(int argc, char *argv[])
{
    if(!Glib::thread_supported()) Glib::thread_init();

    Gtk::Main app(argc, argv);
    MainWindow mw;

    app.run();
}
