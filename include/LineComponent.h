#ifndef LINE_H
#define LINE_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class LineComponent : public GraphicComponent {
public:
  LineComponent(glm::vec3 const & src, glm::vec3 const & dst);
  ~LineComponent();

  void render() override;

private:
  void setUpShader() override;

private:
  GLushort* _indices;
  GLfloat* _vertexData;
};

#endif
