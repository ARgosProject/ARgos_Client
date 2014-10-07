#include "GfxShader.h"
#include <iostream>
#include <cstdio>
#include <cassert>

GfxShader::GfxShader()
  : _src(NULL), _id(0), _glShaderType(0) {

}

GfxShader::~GfxShader() {
  if(_src) {
    delete [] _src;
  }

  glDeleteShader(_id);
}

bool GfxShader::loadVertexShader(const std::string& filename) {
  assert(!_src);

  // Read the whole file into memory
  FILE* f = fopen(filename.c_str(), "rb");
  assert(f);

  fseek(f, 0, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0, SEEK_SET);
  _src = new GLchar[sz+1];
  fread(_src, 1, sz, f);
  _src[sz] = 0;
  fclose(f);

  // Create and compile the shader
  int compile_ok, length;
  _glShaderType = GL_VERTEX_SHADER;
  _id = glCreateShader(_glShaderType);
  length = strlen(_src);
  glShaderSource(_id, 1, (const GLchar**)&_src, &length);
  glCompileShader(_id);
  glGetShaderiv(_id, GL_COMPILE_STATUS, &compile_ok);
  if(compile_ok == GL_FALSE) {
    std::cerr << "vert: " << std::endl;
    glDeleteShader(_id);
    return 0;
  }

  // Any error?
  assert(glGetError() == 0);

  return true;
}

bool GfxShader::loadFragmentShader(const std::string& filename) {
  assert(!_src);

  // Read the whole file into memory
  FILE* f = fopen(filename.c_str(), "rb");
  assert(f);

  fseek(f, 0, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0, SEEK_SET);
  _src = new GLchar[sz+1];
  fread(_src, 1, sz, f);
  _src[sz] = 0;
  fclose(f);

  // Create and compile the shader
  int compile_ok, length;
  _glShaderType = GL_FRAGMENT_SHADER;
  _id = glCreateShader(_glShaderType);
  length = strlen(_src);
  glShaderSource(_id, 1, (const GLchar**)&_src, &length);
  glCompileShader(_id);
  glGetShaderiv(_id, GL_COMPILE_STATUS, &compile_ok);
  if(compile_ok == GL_FALSE) {
    std::cerr << "frag: " << std::endl;
    glDeleteShader(_id);
    return 0;
  }

  // Any error?
  assert(glGetError() == 0);

  return true;
}

GLuint GfxShader::getId() const {
  return _id;
}
