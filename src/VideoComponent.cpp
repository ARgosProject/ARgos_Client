#include "VideoComponent.h"
#include "GLContext.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Log.h"

namespace argosClient {

  VideoComponent::VideoComponent(GLfloat width, GLfloat height)
    : _width(width), _height(height), _textureId(-1), _loop(false) {
    /**
     *    0__1
     *    | /|
     *    |/ |
     *    3__2
     */

    _indices = new GLushort[6] { 0, 1, 2, 0, 2, 3 };
    _vertexData = new GLfloat[20] {
    // X        Y        Z     U     V
      -_width,  _height, 0.0f, 0.0f, 0.0f, // Top-left
       _width,  _height, 0.0f, 1.0f, 0.0f, // Top-right
       _width, -_height, 0.0f, 1.0f, 1.0f, // Bottom-right
      -_width, -_height, 0.0f, 0.0f, 1.0f  // Bottom-left
    };

    // Set the shader
    this->loadGLProgram("shaders/video.glvs", "shaders/video.glfs");
  }

  VideoComponent::VideoComponent(const std::string& fileName, GLfloat width, GLfloat height)
    : VideoComponent(width, height) {
    loadVideoFromFile(fileName);
  }

  VideoComponent::~VideoComponent() {
    delete [] _indices;
    delete [] _vertexData;
    glDeleteTextures(1, &_textureId);
  }

  void VideoComponent::setLoop(bool loop) {
    _loop = loop;
  }

  void VideoComponent::loadVideoFromFile(const std::string& fileName) {
    // Load the video file
    _fileName = fileName;
    _videoReader.open(_fileName);
    if(!_videoReader.isOpened()) {
      Log::error("Could not open video file '" + _fileName + "'.");
      exit(1);
    }

    cv::Size info = cv::Size((int) _videoReader.get(CV_CAP_PROP_FRAME_WIDTH),
                             (int) _videoReader.get(CV_CAP_PROP_FRAME_HEIGHT));

    Log::success("Video file '" + _fileName + "' loaded.");
    Log::success("Video information: " + std::to_string(info.width) + "x" + std::to_string(info.height) + ". " +
                 std::to_string(_videoReader.get(CV_CAP_PROP_FRAME_COUNT)) + " frames.");

    readVideoFrame(_videoFrame);
  }

  void VideoComponent::readVideoFrame(cv::Mat& videoFrame) {
    _videoReader >> videoFrame;
    cvtColor(videoFrame, videoFrame, CV_BGR2RGB);
  }

  void VideoComponent::makeVideoTexture(const cv::Mat& mat) {
    // Byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);

    // Generate a texture object
    if(!glIsTexture(_textureId)) {
      glGenTextures(1, &_textureId);
    }

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create a gl texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
  }

  void VideoComponent::setUpShader() {
    GLint id = _shader.getId();

    _vertexHandler = glGetAttribLocation(id, "a_position");
    _texHandler = glGetAttribLocation(id, "a_texCoord");
    _mvpHandler = glGetUniformLocation(id, "u_mvp");
    _samplerHandler = glGetUniformLocation(id, "s_texture");
  }

  void VideoComponent::specificRender() {
    if(_loop) {
      if(_videoFrame.empty())
        _videoReader.set(CV_CAP_PROP_POS_FRAMES, 0);
    }
    else {
      if(_videoFrame.empty())
        return;
    }

    _shader.useProgram();

    glVertexAttribPointer(_vertexHandler, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _vertexData);
    glEnableVertexAttribArray(_vertexHandler);
    glVertexAttribPointer(_texHandler, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &_vertexData[3]);
    glEnableVertexAttribArray(_texHandler);

    glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glUniform1i(_samplerHandler, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);

    readVideoFrame(_videoFrame);
    makeVideoTexture(_videoFrame);
  }

}
