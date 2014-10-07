#ifndef CIRCLECOMPONENT_H
#define CIRCLECOMPONENT_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class CircleComponent : public GraphicComponent {
public:
  CircleComponent(GLfloat radius);
  ~CircleComponent();

  void render() override;

private:
  void setUpShader() override;

private:
  GLushort* _indices;
  GLfloat* _vertexData;
  GLfloat _radius;
};

#endif
