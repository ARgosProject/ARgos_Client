#include <cassert>
#include "GfxProgram.h"

GfxProgram::GfxProgram() {
  _vertexShader = new GfxShader();
  _fragmentShader = new GfxShader();
}

GfxProgram::~GfxProgram() {
  glDeleteProgram(_id);

  delete _vertexShader;
  delete _fragmentShader;
}

GLuint GfxProgram::loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {
  _vertexShader->loadVertexShader(vertex_file_path);
  _fragmentShader->loadFragmentShader(fragment_file_path);
  _id = glCreateProgram();

  glAttachShader(_id, _vertexShader->getId());
  glAttachShader(_id, _fragmentShader->getId());
  glLinkProgram(_id);

  glDetachShader(_id, _vertexShader->getId());
  glDetachShader(_id, _fragmentShader->getId());
  glDeleteShader(_vertexShader->getId());
  glDeleteShader(_fragmentShader->getId());

  assert(glGetError() == 0);

  return _id;
}

GLuint GfxProgram::getId() const {
  return _id;
}

void GfxProgram::useProgram() const {
  glUseProgram(_id);
  assert(glGetError() == 0);
}
