#include "MainWindow.h"

MainWindow::MainWindow()
{
    win.add(pb);
    pb.show();
    win.show();

    pb.set_fraction(0.0);
    ping.signal_increment.connect(sigc::mem_fun(*this, &MainWindow::pong));
}
void MainWindow::pong()
{
    int val = ping.get_value();
    std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << " value = " << val << "\n";
    pb.set_fraction(float(val) / float(100.0));
    if (val >= 99)
        Gtk::Main::quit();
}
