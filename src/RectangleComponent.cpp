#include "RectangleComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace argosClient {

  RectangleComponent::RectangleComponent(GLfloat width, GLfloat height)
    : _width(width), _height(height) {
    /**
     *    0__1
     *    | /|
     *    |/ |
     *    3__2
     */

    _indices = new GLushort[6] { 0, 1, 2, 0, 2, 3 };
    _vertexData = new GLfloat[20] {
    // X        Y        Z
      -_width,  _height, 0.0f, // Top-left
       _width,  _height, 0.0f, // Top-right
       _width, -_height, 0.0f, // Bottom-right
      -_width, -_height, 0.0f, // Bottom-left
    };

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

  void RectangleComponent::specificRender() {
    _shader.useProgram();

    glVertexAttribPointer(_colorHandler, 3, GL_FLOAT, GL_FALSE, 0, _vertexData);
    glEnableVertexAttribArray(_colorHandler);

    glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);
  }

}
