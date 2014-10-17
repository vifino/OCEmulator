#ifndef SCREENSWIDGET_H
#define SCREENSWIDGET_H

#include <gtkmm.h>
#include <memory>
#include <vector>
#include "componentscreen.h"

#define  screenIterator std::vector<std::shared_ptr<ComponentScreen> >::iterator

class ScreensWidget :  public Gtk::DrawingArea
{
public:
  ScreensWidget();
  ~ScreensWidget();
  void addScreen(std::shared_ptr<ComponentScreen> screen);
  std::vector<std::shared_ptr<ComponentScreen> > screens;
protected:
  virtual bool on_draw(const Cairo::RefPtr< Cairo::Context >& cr);
private:
  Pango::FontDescription font;
  Glib::RefPtr<Pango::Layout> fontLayout;
};

#endif // SCREENSWIDGET_H
