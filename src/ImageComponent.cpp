#include "ImageComponent.h"

#include <iostream>

#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

namespace argosClient {

  ImageComponent::ImageComponent(GLfloat width, GLfloat height)
    : _width(width), _height(height), _textureId(-1) {
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
    this->loadGLProgram("shaders/image.glvs", "shaders/image.glfs");
  }

  ImageComponent::ImageComponent(const std::string& file_name, GLfloat width, GLfloat height)
    : ImageComponent(width, height) {
    loadImageFromFile(file_name);
  }

  ImageComponent::~ImageComponent() {
    delete [] _indices;
    delete [] _vertexData;
    deleteTexture();
  }

  void ImageComponent::loadImageFromFile(const std::string& file_name) {
    int width, height, channels;

    unsigned char* buffer = SOIL_load_image(file_name.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    if(buffer == nullptr) {
      Log::error("Image '" + file_name + "' loaded incorrectly");
      Log::error(std::string(SOIL_last_result()));
      exit(1);
    }
    else {
      Log::success("Image '" + file_name + "' (" + std::to_string(width) + "x" + std::to_string(height) + ") successfully loaded");
    }

    GLenum format;
    switch(channels) {
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    }

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &_textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);

    // Free the image data
    SOIL_free_image_data(buffer);
  }

  void ImageComponent::loadImageFromMat(cv::Mat& mat) {
    // Byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);

    // Generate a texture object
    glGenTextures(1, &_textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set incoming texture format to:
    // GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,   ???????
    // GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
    GLenum inputColourFormat = GL_RGB;
    if(mat.channels() == 1) {
      inputColourFormat = GL_LUMINANCE;
    }

    // Create the texture
    glTexImage2D(GL_TEXTURE_2D, 0, inputColourFormat, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);

    assert(glGetError() == 0);
  }

  void ImageComponent::setUpShader() {
    GLint id = _shader.getId();

    _vertexHandler = glGetAttribLocation(id, "a_position");
    _texHandler = glGetAttribLocation(id, "a_texCoord");
    _mvpHandler = glGetUniformLocation(id, "u_mvp");
    _samplerHandler = glGetUniformLocation(id, "s_texture");
  }

  void ImageComponent::specificRender() {
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

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void ImageComponent::deleteTexture() {
    glDeleteTextures(1, &_textureId);
  }

}
