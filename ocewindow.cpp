#include "ocewindow.h"

OCEWindow::OCEWindow() : mbox(Gtk::ORIENTATION_VERTICAL)
{
  add(mbox);
  
  Glib::RefPtr<Gtk::ActionGroup> actions = Gtk::ActionGroup::create();
  actions->add(Gtk::Action::create("MenuFile", "_File"));
  actions->add(Gtk::Action::create("New", "_New"));
  
  Glib::RefPtr<Gtk::UIManager> umanager = Gtk::UIManager::create();
  umanager->insert_action_group(actions);
  add_accel_group(umanager->get_accel_group());
  
  Glib::ustring ui_info = 
    "<ui>"
    "	<menubar name='MenuBar'>"
    "		<menu action='MenuFile'>"
    "			<menuitem action='New'/>"
    "		</menu>"
    "	</menubar>"
    "</ui>";
  
  umanager->add_ui_from_string(ui_info);
  
  Gtk::Widget *menuBar = umanager->get_widget("/MenuBar");
  mbox.pack_start(*menuBar, Gtk::PACK_SHRINK);
  mbox.pack_start(screens);
  
  show_all_children();
}

ScreensWidget &OCEWindow::getScreenW()
{
  return screens;
}

bool OCEWindow::on_key_press_event(GdkEventKey* event)
{
  if (!screens.screens.empty())
  {
    if (!screens.screens[0]->keyboards.empty())
    {
      screens.screens[0]->keyboards[0]->keyEvent(event);
    }
  }
  return Gtk::Widget::on_key_press_event(event);
}

bool OCEWindow::on_key_release_event(GdkEventKey* event)
{
  if (!screens.screens.empty())
  {
    if (!screens.screens[0]->keyboards.empty())
    {
      screens.screens[0]->keyboards[0]->keyEvent(event);
    }
  }
  return Gtk::Widget::on_key_release_event(event);
}


OCEWindow::~OCEWindow()
{

}
