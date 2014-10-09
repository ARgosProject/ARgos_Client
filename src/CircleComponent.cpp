#include "CircleComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace argosClient {

  CircleComponent::CircleComponent(GLfloat radius)
    : _indices(NULL), _vertexData(NULL), _radius(radius) {
    /**
     *    0__3
     *    |\ |
     *    | \|
     *    1__2
     */
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    GLfloat vertexData[] = {
      -radius,  radius, 0.0f, // Position 0
      0.0f,  0.0f,            // TexCoord 0
      -radius, -radius, 0.0f, // Position 1
      0.0f,  1.0f,            // TexCoord 1
      radius, -radius, 0.0f, // Position 2
      1.0f,  1.0f,            // TexCoord 2
      radius,  radius, 0.0f, // Position 3
      1.0f,  0.0f             // TexCoord 3
    };

    // Copy aux arrays to the class' members
    size_t length = 6*sizeof(GLushort);
    _indices = new GLushort[length];
    memcpy(_indices, indices, length);
    length = 20*sizeof(GLfloat);
    _vertexData = new GLfloat[length];
    memcpy(_vertexData, vertexData, length);

    // Set the shader
    this->loadGLProgram("shaders/circle.glvs", "shaders/circle.glfs");
  }

  CircleComponent::~CircleComponent() {
    delete [] _indices;
    delete [] _vertexData;
  }

  void CircleComponent::setUpShader() {
    GLuint id = _shader.getId();

    _vertexHandler = glGetAttribLocation(id, "a_position");
    _texHandler = glGetAttribLocation(id, "a_texCoord");
    _colorHandler = glGetUniformLocation(id, "u_color");
    _mvpHandler = glGetUniformLocation(id, "u_mvp");
  }

  void CircleComponent::render() {
    _shader.useProgram();

    glVertexAttribPointer(_vertexHandler, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _vertexData);
    glEnableVertexAttribArray(_vertexHandler);
    glVertexAttribPointer(_texHandler, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &_vertexData[3]);
    glEnableVertexAttribArray(_texHandler);
    glVertexAttribPointer(_colorHandler, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _vertexData);
    glEnableVertexAttribArray(_colorHandler);

    glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);

    glDisable(GL_BLEND);
  }

}
