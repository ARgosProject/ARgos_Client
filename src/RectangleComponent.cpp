#include "RectangleComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

RectangleComponent::RectangleComponent(GLfloat width, GLfloat height)
  : _indices(NULL), _vertexData(NULL), _width(width), _height(height) {
  /**
   *    0__3
   *    |\ |
   *    | \|
   *    1__2
   */
  GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
  GLfloat vertexData[] = {
    -width,  height, 0.0f,  // Position 0
    -width, -height, 0.0f,  // Position 1
     width, -height, 0.0f,  // Position 2
     width,  height, 0.0f,  // Position 3
  };

  // Copy aux arrays to the class' members
	size_t length = 6*sizeof(GLushort);
	_indices = new GLushort[length];
	memcpy(_indices, indices, length);
	length = 12*sizeof(GLfloat);
	_vertexData = new GLfloat[length];
	memcpy(_vertexData, vertexData, length);

	// Set the shader
	this->loadGLProgram("shaders/rectangle.glvs", "shaders/rectangle.glfs");
}

RectangleComponent::~RectangleComponent() {
  delete [] _indices;
  delete [] _vertexData;
}

void RectangleComponent::setUpShader() {
  GLuint id = _shader.getId();

  _colorHandler = glGetUniformLocation(id, "u_color");
  _mvpHandler = glGetUniformLocation(id, "u_mvp");
}

void RectangleComponent::render() {
  _shader.useProgram();

  glVertexAttribPointer(_colorHandler, 3, GL_FLOAT, GL_FALSE, 0, _vertexData);
  glEnableVertexAttribArray(_colorHandler);

  glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix));

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);
}
