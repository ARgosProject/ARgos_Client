#include "LineComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

LineComponent::LineComponent(glm::vec3 const & src, glm::vec3 const & dst)
  : _indices(NULL), _vertexData(NULL) {
  /**
   *    0
   *    |
   *    |
   *    1
   */
  GLushort indices[] = { 0, 1 };
  GLfloat vertexData[] = {
    src.x, src.y, src.z,  // Position 0
    dst.x, dst.y, dst.z,  // Position 1
  };

  // Copy aux arrays to the class' members
  size_t length = 2*sizeof(GLushort);
  _indices = new GLushort[length];
  memcpy(_indices, indices, length);
  length = 6*sizeof(GLfloat);
  _vertexData = new GLfloat[length];
  memcpy(_vertexData, vertexData, length);

  // Set the shader
  this->loadGLProgram("shaders/rectangle.glvs", "shaders/rectangle.glfs");
}

LineComponent::~LineComponent() {
  delete [] _indices;
  delete [] _vertexData;
}

void LineComponent::setUpShader() {
  GLuint id = _shader.getId();

  _colorHandler = glGetUniformLocation(id, "u_color");
  _mvpHandler = glGetUniformLocation(id, "u_mvp");
}

void LineComponent::render() {
  _shader.useProgram();

  glVertexAttribPointer(_colorHandler, 3, GL_FLOAT, GL_FALSE, 0, _vertexData);
  glEnableVertexAttribArray(_colorHandler);

  glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

  glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, _indices);
}
