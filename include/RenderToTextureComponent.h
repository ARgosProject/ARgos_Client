#ifndef TEXTURERENDER_H
#define TEXTURERENDER_H

#include <vector>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A class representing a compound of graphic components
   * to be drawn as a single one
   */
  class RenderToTextureComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new empty render to texture
     * @param width The width of this graphic component
     * @param height The height of this graphic component
     */
    RenderToTextureComponent(GLfloat width, GLfloat height);

    /**
     * Destroys the render to texture
     */
    ~RenderToTextureComponent();

    /**
     * Retrieves a graphic component from this render to texture by its index
     * @param index The position of the graphic component in the list
     * @return the specified graphic component
     */
    GraphicComponent* getGraphicComponent(int index);

    /**
     * Adds a new graphic component to be rendered to a texture
     * @param graphicComponent The graphic component to be added
     */
    void addGraphicComponent(GraphicComponent* graphicComponent);

    /**
     * Draws this graphic component
     * @see renderToTexture()
     * @see drawTexture()
     */
    void render() override;

  private:
    /**
     * Sets up the shader for this graphic component
     */
    void setUpShader() override;

    /**
     * Generates a new frame buffer object
     * @return 0 if everything was right
     */
    int genFrameBufferObject();

    /**
     * Sets the framebuffer of this render to texture and draws all the graphic components
     */
    void renderToTexture();

    /**
     * Draw the whole texture to the main frame buffer
     */
    void drawTexture();

  private:
    std::vector<GraphicComponent*> _graphicComponents; ///< The list of graphic components

    GLushort* _indices; ///< Indices defining the shared vertex of the triangles
    GLfloat* _vertexData; ///< Positions of the vertex and their uv mapping
    GLuint _framebufferObject; ///< The frame buffer object id
    GLuint _depthRenderbuffer; ///< The render buffer object id
    GLuint _texture; ///< The texture object id
    GLfloat _texWidth; ///< The texture width
    GLfloat _texHeight; ///< The texture height
    uint32_t _screenWidth; ///< The screen width
    uint32_t _screenHeight; ///< The screen height
  };

}

#endif
