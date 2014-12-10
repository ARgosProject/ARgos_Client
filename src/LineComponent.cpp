#include "LineComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace argosClient {

  LineComponent::LineComponent(glm::vec3 const & src, glm::vec3 const & dst, GLfloat width)
    : _width(width) {
    /**
     *    0
     *    |
     *    |
     *    1
     */

    _indices = new GLushort[2] { 0, 1 };
    _vertexData = new GLfloat[6] {
    // X      Y      Z
       src.x, src.y, src.z,
       dst.x, dst.y, dst.z,
    };

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

  void LineComponent::specificRender() {
    _shader.useProgram();

    glVertexAttribPointer(_colorHandler, 3, GL_FLOAT, GL_FALSE, 0, _vertexData);
    glEnableVertexAttribArray(_colorHandler);

    glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

    glLineWidth(_width);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, _indices);
  }

}
