#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

#include <GLES2/gl2.h>
#include <opencv2/opencv.hpp>

#include "GraphicComponent.h"
#include "VideoThread.h"
#include "GfxProgram.h"

using boost::asio::ip::udp;

class VideoStreamComponent : public GraphicComponent {
public:
  VideoStreamComponent(GLfloat width, GLfloat height);
  ~VideoStreamComponent();

  void startReceivingVideo(unsigned short port);
  void makeVideoTexture(const cv::Mat& mat);
  void render() override;

private:
  void receiveVideo(unsigned short port);
  size_t sendVideo(const cv::Mat& mat, udp::socket& udpSocket, const udp::endpoint& udpSenderEndpoint);
  void setUpShader() override;

private:
  GLushort* _indices;
  GLfloat* _vertexData;
  GLfloat _width, _height;
	GLuint _textureId;

  bool _ready, _receive;
  cv::Mat _receivedFrame;
  cv::Mat* _currentFrame;
  std::thread* _videoThread;

  std::chrono::time_point<std::chrono::high_resolution_clock> _begin, _end;
};

#endif
