#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <deque>
#include "Singleton.h"

namespace argosClient {

  class EventManager : public Singleton<EventManager> {
  public:
    enum EventType {
      NONE = -1,
      TD_THREAD_READY = 0,
      TD_THREAD_FINISHED = 1
    };

  public:
    void addEvent(EventType ev);
    EventType popEvent();
    void clearQueue();
    void visualizeQueue();

  private:
    std::deque<EventType> _eventsQueue;
    const unsigned int MAX_EVENTS = 10;
  };

}

#endif
