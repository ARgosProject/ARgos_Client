#ifndef GFXSHADER_H
#define GFXSHADER_H

#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

/**
 * A class representing a shader file
 */
class GfxShader {
public:
  /**
   * Constructs a new shader
   */
  GfxShader();

  /**
   * Destroys the shader
   */
  ~GfxShader();

  /**
   * Load a vertex shader file from disk
   * @param filename The path to the shader file
   * @return true whether no errors on loading and compiling
   */
  bool loadVertexShader(const std::string& filename);

  /**
   * Load a fragment shader file from disk
   * @param filename The path to the shader file
   * @return true whether no errors on loading and compiling
   */
  bool loadFragmentShader(const std::string& filename);

  /**
   * Retrieves the shader id
   * @return the shader id
   */
  GLuint getId() const;

private:
  GLchar* _src; ///< A buffer holding the shader file
  GLuint _id; ///< The shader id
  GLuint _glShaderType; ///< A control variable used to distinguish between VERTEX and FRAGMENT shader
};

#endif
