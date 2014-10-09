#ifndef LINE_H
#define LINE_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A class representing a single straight line
   */
  class LineComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new straight line
     * @param src The starting point of the line
     * @param dst The end point of the line
     */
    LineComponent(glm::vec3 const & src, glm::vec3 const & dst);

    /**
     * Destroys the line
     */
    ~LineComponent();

    /**
     * Draws this graphic component
     */
    void render() override;

  private:
    /**
     * Sets up the shader for this graphic component
     */
    void setUpShader() override;

  private:
    GLushort* _indices; ///< Indices defining the shared vertex of the triangles
    GLfloat* _vertexData; ///< Positions of the vertex and their uv mapping
  };

}

#endif
