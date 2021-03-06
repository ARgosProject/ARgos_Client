#ifndef CIRCLECOMPONENT_H
#define CIRCLECOMPONENT_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A circle graphic component defined by a radius
   */
  class CircleComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new circle
     * @param radius The radius defining the size of the circle
     */
    CircleComponent(GLfloat radius);

    /**
     * Destroys the circle
     */
    ~CircleComponent();

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
    GLfloat _radius; ///< The radius of the circle
  };

}

#endif
