#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>
#include <cstdlib>

template<typename T>
class Singleton {
public:
  static T& getInstance() {
    if (instance == NULL) instance = new T;
    return *instance;
  }

  static void destroy() {
    if(instance != NULL) delete instance;
  }

private:
  static T* instance;

  Singleton & operator=(const Singleton &);

};

template<typename T>
T* Singleton<T>::instance = NULL;

#endif
