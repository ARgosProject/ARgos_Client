#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <string>

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#include "GfxProgram.h"

class GraphicComponent {
public:
  GraphicComponent();
  virtual ~GraphicComponent();

  virtual void loadGLProgram(const std::string& vertexShader, const std::string& fragmentShader);
  virtual void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

  virtual void rotate(GLfloat angle, glm::vec3 const & axis);
	virtual void scale(glm::vec3 const & factors);
  virtual void translate(glm::vec3 const & translation);

  virtual void setModelViewMatrix(glm::mat4 modelViewMatrix);
  virtual void setProjectionMatrix(glm::mat4 projectionMatrix);

  virtual void render() = 0;

protected:
  virtual void setUpShader() = 0;

protected:
  glm::mat4 _modelViewMatrix;
  glm::mat4 _projectionMatrix;
  GfxProgram _shader;
  GLint _vertexHandler, _texHandler, _samplerHandler, _colorHandler, _mvpHandler;
};

#endif
