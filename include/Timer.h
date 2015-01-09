#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

namespace argosClient {

  /**
   * A timer class used to time measurements
   */
  class Timer {
  public:
    /**
     * Starts the timer count
     */
    void start() {
      epoch = high_resolution_clock::now();
    }

    /**
     * Retrieves the elapsed time at the moment of the invocation
     */
    high_resolution_clock::duration elapsed() const {
      return high_resolution_clock::now() - epoch;
    }

    /**
     * Retrieves the elapsed time as hours
     */
    hours::rep getHours() const {
      return duration_cast<hours>(elapsed()).count();
    }

    /**
     * Retrieves the elapsed time as minutes
     */
    minutes::rep getMinutes() const {
      return duration_cast<minutes>(elapsed()).count();
    }

    /**
     * Retrieves the elapsed time as seconds
     */
    seconds::rep getSeconds() const {
      return duration_cast<seconds>(elapsed()).count();
    }

    /**
     * Retrieves the elapsed time as milliseconds
     */
    milliseconds::rep getMilliseconds() const {
      return duration_cast<milliseconds>(elapsed()).count();
    }

    /**
     * Retrieves the elapsed time as microseconds
     */
    microseconds::rep getMicroseconds() const {
      return duration_cast<microseconds>(elapsed()).count();
    }

  private:
    high_resolution_clock::time_point epoch; ///< The epoch used by std::chrono
  };

}

#endif
