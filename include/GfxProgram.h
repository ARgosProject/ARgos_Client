#ifndef GFXPROGRAM_H
#define GFXPROGRAM_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

#include "GfxShader.h"

class GfxProgram {
public:
  GfxProgram();
  ~GfxProgram();

  GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
  GLuint getId() const;
  void useProgram() const;

private:
  GfxShader* _vertexShader;
  GfxShader* _fragmentShader;
  GLuint _id;
};

#endif
