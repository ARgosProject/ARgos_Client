#include "EventManager.h"
#include <iostream>

namespace argosClient {

  void EventManager::addEvent(EventManager::EventType ev) {
    if(_eventsQueue.size() > MAX_EVENTS)
      _eventsQueue.pop_front();

    _eventsQueue.push_back(ev);
  }

  EventManager::EventType EventManager::popEvent() {
    EventType ev = EventType::NONE;

    if(!_eventsQueue.empty()) {
      ev = _eventsQueue.back();
      _eventsQueue.pop_back();
    }

    return ev;
  }

  void EventManager::visualizeQueue() {
    std::cout << "[";
    for(EventType ev : _eventsQueue) {
      switch(ev) {
      case EventType::TD_THREAD_READY:
        std::cout << "R|";
        break;
      case EventType::TD_THREAD_FINISHED:
        std::cout << "F|";
        break;
      case EventType::NONE:
        std::cout << "*|";
        break;
      }
    }
    std::cout << "]" << std::endl;
  }

}
