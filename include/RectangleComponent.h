#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A class representing a rectangle
   */
  class RectangleComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new rectangle
     * @param width The width of this graphic component
     * @param height The height of this graphic component
     */
    RectangleComponent(GLfloat width, GLfloat height);

    /**
     * Destroys the rectangle
     */
    ~RectangleComponent();

  private:
    /**
     * The specific logic used to draw this graphic component
     */
    void specificRender() override;

    /**
     * Sets up the shader for this graphic component
     */
    void setUpShader() override;

  private:
    GLushort* _indices; ///< Indices defining the shared vertex of the triangles
    GLfloat* _vertexData; ///< Positions of the vertex and their uv mapping
    GLfloat _width; ///< The width of this graphic component
    GLfloat _height; ///< The height of this graphic component
  };

}

#endif
