#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "exampleworker.h"

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();

  // Called from the worker thread.
  void notify();

private:
  // Signal handlers.
  void on_start_button_clicked();
  void on_stop_button_clicked();
  void on_quit_button_clicked();

  void update_start_stop_buttons();
  void update_widgets();

  // Dispatcher handler.
  void on_notification_from_worker_thread();

  // Member data.
  Gtk::Box m_VBox;
  Gtk::ButtonBox m_ButtonBox;
  Gtk::Button m_ButtonStart;
  Gtk::Button m_ButtonStop;
  Gtk::Button m_ButtonQuit;
  Gtk::ProgressBar m_ProgressBar;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TextView m_TextView;

  Glib::Dispatcher m_Dispatcher;
  ExampleWorker m_Worker;
  std::thread* m_WorkerThread;
};

#endif // GTKMM_EXAMPLEWINDOW_H