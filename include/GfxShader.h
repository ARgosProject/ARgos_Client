#ifndef GFXSHADER_H
#define GFXSHADER_H

#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

class GfxShader {
public:
  GfxShader();
  ~GfxShader();

  bool loadVertexShader(const std::string& filename);
  bool loadFragmentShader(const std::string& filename);
  GLuint getId() const;

private:
  GLchar* _src;
  GLuint _id;
  GLuint _glShaderType;
};

#endif
