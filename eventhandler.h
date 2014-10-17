#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <gtkmm.h>
#include <iostream>
#include <queue>
#include <vector>
#include <lua.hpp>

struct eventParameter
{
  union
  {
    double number;
    bool boolean;
  };
  int type;
  std::string string; // This cannot go in the union
};

#define eevent std::vector<eventParameter>
#define eventIterator std::queue<eevent>::iterator
#define eventParamIterator std::vector<eventParameter>::iterator

class EventHandler
{
public:
  EventHandler();
  ~EventHandler();
  void start(lua_State *state);
  void queueEvent(eevent ev);
  void resumeThread(lua_State *state);
private:
  mutable Glib::Threads::Mutex m_Mutex;
  std::queue<eevent> eventQueue;
  bool stopHandler;
  double timeout;
};

#endif // EVENTHANDLER_H
