#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <string>
#include <boost/thread.hpp>

#include "bcm_host.h"
#include <ilclient/ilclient.h>

namespace argosClient {

  /**
   * A class used for threading video decoding
   * [Experimental]
   */
  class VideoThread {
  public:
    /**
     * The video properties
     */
    struct VideoProperties {
      bool setted; ///< If the struct properties have been filled
      int width; ///< The width of the video
      int height; ///< The height of the video
      int fps; ///< The rate (frames per second) of the video
    };

  public:
    /**
     * Constructs a new video thread
     * @param fileName The file name used in the future to load the video
     */
    VideoThread(const std::string& fileName);

    /**
     * Destroys the video thread
     */
    ~VideoThread();

    /**
     * Starts the video thread
     * @see videoDecode()
     */
    void start();

    /**
     * Checks whether the thread is still running or not
     * @return True if the thread is running. False otherwise
     */
    bool isRunning() const;

    /**
     * Waits for the thread to finish
     */
    void join();

    /**
     * Stops the thread
     */
    void interrupt();

    /**
     * Notifies the thread to continue once the EGL image has been created
     * @param eglImage The EGL image used to render the video
     */
    void next(void* eglImage);

    /**
     * Retrieves the video properties
     * @return the video properties
     */
    const VideoProperties& getVideoProperties() const;

  private:
    /**
     * A thread method used to decode video
     * It uses OpenMAX to decode
     */
    void videoDecode();

    /**
     * A thread method used to decode video
     * It uses libAV to decode
     */
    void videoDecode1();

    /**
     * Callback triggered when the buffer fill has done
     */
    static void myFillBufferDone(void* data, COMPONENT_T* comp);

  private:
    std::string _fileName; ///< The file name of the video

    static void* _eglImage; ///< The EGL image to render the video frames
    static OMX_BUFFERHEADERTYPE* _eglBuffer; ///< The EGL buffer
    static COMPONENT_T* _eglRender; ///< The EGL render

    VideoProperties _vp; ///< The video properties

    bool _running; ///< Control variable stating whether the thread is running or not
    boost::thread _thread; ///< The thread object used to hold the video decoding

    /** @name Thread control
     *  Objects used to control concurrency of the thread
     */
    ///@{
    bool _condition;
    boost::condition_variable _condition_changed;
    boost::mutex _stopMutex;
    ///@}
  };

}

#endif
