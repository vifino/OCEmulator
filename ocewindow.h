#ifndef OCEWINDOW_H
#define OCEWINDOW_H
#include <gtkmm.h>
#include <boost/asio.hpp>

#include "screenswidget.h"

class OCEWindow : public Gtk::Window
{
public:
  OCEWindow();
  virtual ~OCEWindow();
  ScreensWidget &getScreenW();
private:
  Gtk::Box mbox;
  ScreensWidget screens;
  virtual bool on_key_press_event(GdkEventKey* event);
  virtual bool on_key_release_event(GdkEventKey* event);
};

#endif // OCEWINDOW_H
