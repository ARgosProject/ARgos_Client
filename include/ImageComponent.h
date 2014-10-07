#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <opencv2/opencv.hpp>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class ImageComponent : public GraphicComponent {
 public:
  ImageComponent(GLfloat width, GLfloat height);
  ImageComponent(const std::string& file_name, GLfloat width, GLfloat height);
  ~ImageComponent();

  void loadImageFromFile(const std::string& file_name);
  void loadImageFromMat(cv::Mat& mat);
  void render() override;
  void deleteTexture();

 private:
  void setUpShader() override;

 private:
  GLushort* _indices;
  GLfloat* _vertexData;
  GLfloat _width, _height;
  GLuint _textureId;
};

#endif
