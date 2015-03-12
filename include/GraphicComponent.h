#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <string>

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#include "GfxProgram.h"

namespace argosClient {

  /**
   * The base class for every graphic component in the application
   * A graphic component is any renderable thing in the application,
   * from lines, rectangles and circles up to images and video
   */
  class GraphicComponent {
  public:
    /**
     * Constructs a new graphic component
     */
    GraphicComponent();

    /**
     * Destroys the graphic component
     */
    virtual ~GraphicComponent();

    /**
     * Loads the OpenGL program this graphic component use
     * @param vertexShader The Vertex Shader file to load
     * @param fragmentShader The Fragment Shader file to load
     */
    virtual void loadGLProgram(const std::string& vertexShader, const std::string& fragmentShader);

    /**
     * Sets the colour for this graphic component
     * @param r The red component
     * @param g The green component
     * @param b The blue component
     * @param a The alpha component
     */
    virtual void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * Rotates this graphic component relatively
     * @param angle The angle amount to rotate in degrees
     * @param axis The axis about which to rotate
     */
    virtual void rotate(GLfloat angle, glm::vec3 const & axis);

    /**
     * Scales this graphic component relatively
     * @param factors The 3 axis with the scale factors added
     */
    virtual void scale(glm::vec3 const & factors);

    /**
     * Translates this graphic component relatively
     * @param translation The movement amount to translate
     */
    virtual void translate(glm::vec3 const & translation);

    /**
     * Rotates this graphic component
     * @param angle The angle amount to rotate in degrees
     * @param axis The axis about which to rotate
     */
    virtual void setRotation(GLfloat angle, glm::vec3 const & axis);

    /**
     * Scales this graphic component
     * @param factors The 3 axis with the scale factors added
     */
    virtual void setScale(glm::vec3 const & factors);

    /**
     * Translates this graphic component
     * @param translation The movement amount to translate
     */
    virtual void setPosition(glm::vec3 const & position);

    /**
     * Sets the model matrix for this graphic component
     * @param modelMatrix The model matrix
     */
    virtual void setModelMatrix(const glm::mat4& modelMatrix);

    /**
     * Sets the model view matrix for this graphic component
     * @param modelViewMatrix The model view matrix
     */
    virtual void setModelViewMatrix(const glm::mat4& modelViewMatrix);

    /**
     * Sets the projection matrix for this graphic component
     * @param projectionMatrix The projection matrix
     */
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix);

    /**
     * States if this GC should be drawed or not
     * @param show Whether this GC should be drawed or not
     */
    virtual void show(bool show = true);

    /**
     * States if this GC should be updated or not
     * @param update Whether this GC should be updated or not
     */
    virtual void noUpdate(bool noUpdate = true);

    /**
     * Draws this graphic component
     */
    virtual void render();


  protected:
    /**
     * The specific logic used to draw this graphic component
     */
    virtual void specificRender() = 0;

    /**
     * Sets up the shader for this graphic component
     */
    virtual void setUpShader() = 0;

  protected:
    glm::mat4 _model; ///< The matrix holding the absolute transformation of this graphic component
    glm::mat4 _modelViewMatrix; ///< The model view matrix
    glm::mat4 _projectionMatrix; ///< The projection matrix
    GfxProgram _shader; ///< The shader program of this graphic component
    GLint _vertexHandler; ///< The vertex handler for the shader
    GLint _texHandler; ///< The texture handler for the shader
    GLint _samplerHandler; ///< The sampler handler for the shader
    GLint _colorHandler; ///< The colour handler for the shader
    GLint _mvpHandler; ///< The model view projection matrix handler for the shader
    bool _show; ///< Whether the GC should be drawed or not
    bool _noUpdate; ///< Whether the GC should be updated with any new model view matrix or not
  };

}

#endif
