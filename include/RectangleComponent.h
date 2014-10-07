#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class RectangleComponent : public GraphicComponent {
public:
  RectangleComponent(GLfloat width, GLfloat height);
  ~RectangleComponent();

  void render() override;

private:
  void setUpShader() override;

private:
  GLushort* _indices;
  GLfloat* _vertexData;
  GLfloat _width, _height;
};

#endif
