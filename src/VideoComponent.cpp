#include "VideoComponent.h"
#include "GLContext.h"
#include "VideoThread.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace argosClient {

  VideoComponent::VideoComponent(GLfloat width, GLfloat height)
    : _width(width), _height(height),
      _textureId(-1), _loop(false), _eglImage(NULL), _videoThread(NULL) {
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

    if(_videoThread) {
      _videoThread->join();
      delete _videoThread;
    }

    glDeleteTextures(1, &_textureId);

    if(_eglImage != 0) {
      eglDestroyImageKHR(GLContext::getInstance().getEGLDisplay(), _eglImage);
    }
  }

  void VideoComponent::setLoop(bool loop) {
    _loop = loop;
  }

  void VideoComponent::loadVideoFromFile(const std::string& fileName) {
    // Generate a texture object
    glGenTextures(1, &_textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    _fileName = fileName;

    // Start the video decoding thread
    _videoThread = new VideoThread(fileName);
    _videoThread->start();

    // Get video properties
    VideoThread::VideoProperties vp = _videoThread->getVideoProperties();
    while(!vp.setted) {
      vp = _videoThread->getVideoProperties();
    }

    // Create a gl texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vp.width, vp.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // Create an EGL Image
    _eglImage = eglCreateImageKHR(GLContext::getInstance().getEGLDisplay(),
                                  GLContext::getInstance().getEGLContext(),
                                  EGL_GL_TEXTURE_2D_KHR,
                                  (EGLClientBuffer)_textureId,
                                  0);

    assert(_eglImage != EGL_NO_IMAGE_KHR);

    // Texture is correctly created; thread can start decoding the video
    _videoThread->next(_eglImage);
  }

  void VideoComponent::setUpShader() {
    GLint id = _shader.getId();

    _vertexHandler = glGetAttribLocation(id, "a_position");
    _texHandler = glGetAttribLocation(id, "a_texCoord");
    _mvpHandler = glGetUniformLocation(id, "u_mvp");
    _samplerHandler = glGetUniformLocation(id, "s_texture");
  }

  void VideoComponent::specificRender() {
    _shader.useProgram();

    if(_loop) {
      if(_videoThread && !_videoThread->isRunning()) {
        _videoThread->interrupt();
        _videoThread->start();
      }
    }

    glVertexAttribPointer(_vertexHandler, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _vertexData);
    glEnableVertexAttribArray(_vertexHandler);
    glVertexAttribPointer(_texHandler, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &_vertexData[3]);
    glEnableVertexAttribArray(_texHandler);

    glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glUniform1i(_samplerHandler, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);
  }

}
