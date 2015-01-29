#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <deque>
#include "Singleton.h"

namespace argosClient {

  class EventManager : public Singleton<EventManager> {
  public:
    enum EventType {
      NONE,
      TD_THREAD_READY,
      TD_THREAD_FINISHED
    };

  public:
    void addEvent(EventType ev);
    EventType popEvent();

    void visualizeQueue();

  private:
    std::deque<EventType> _eventsQueue;
    const unsigned int MAX_EVENTS = 10;
  };

}

#endif
