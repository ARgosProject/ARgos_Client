#ifndef GFXPROGRAM_H
#define GFXPROGRAM_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

#include "GfxShader.h"

namespace argosClient {

  /**
   * A class representing a shader program
   */
  class GfxProgram {
  public:
    /**
     * Constructs a new shader program
     */
    GfxProgram();

    /**
     * Destroys the shader program
     */
    ~GfxProgram();

    /**
     * Creates, attaches and links a shader
     * @param vertex_file_path The Vertex Shader file
     * @param fragment_file_path The Fragment Shader file
     * @return the program id
     */
    GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);

    /**
     * Gets the program id
     * @return the program id
     */
    GLuint getId() const;

    /**
     * Activates this program
     */
    void useProgram() const;

  private:
    GfxShader* _vertexShader; ///< A pointer to the vertex shader of this program
    GfxShader* _fragmentShader; ///< A pointer to the fragment shader of this program
    GLuint _id; ///< The id of this program
  };

}

#endif
