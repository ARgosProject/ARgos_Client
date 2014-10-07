#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <string>
#include <boost/thread.hpp>

#include "bcm_host.h"
#include <ilclient/ilclient.h>

class VideoThread {
public:
	struct VideoProperties {
		bool setted;
		int width;
		int height;
		int fps;
	};

public:
	VideoThread(const std::string& fileName);
	~VideoThread();

	void start();
	bool isRunning() const;
	void join();
	void interrupt();
	void next(void* eglImage);

	const VideoProperties& getVideoProperties() const;

private:
	void videoDecode();
	void videoDecode1();
	static void myFillBufferDone(void* data, COMPONENT_T* comp);

private:
	std::string _fileName;

	static void* _eglImage;
	static OMX_BUFFERHEADERTYPE* _eglBuffer;
	static COMPONENT_T* _eglRender;

	VideoProperties _vp;

	bool _condition;
	bool _running;
	boost::condition_variable _condition_changed;
	boost::mutex _stopMutex;
	boost::thread _thread;
};

#endif
