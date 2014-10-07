#include "EGLconfig.h"

#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>

EGLconfig::EGLconfig() {
  _attributes[EGL_RED_SIZE] = 8;
  _attributes[EGL_GREEN_SIZE] = 8;
  _attributes[EGL_BLUE_SIZE] = 8;
  _attributes[EGL_ALPHA_SIZE] = 8;
  _attributes[EGL_DEPTH_SIZE] = 16;
  _attributes[EGL_SURFACE_TYPE] = EGL_WINDOW_BIT;
}

EGLconfig::~EGLconfig() {

}

void EGLconfig::setAttribute(EGLint attrib, EGLint value) {
  assert(attrib >= 0x3020 && attrib <= 0x3042);
  _attributes[attrib] = value;
}

void EGLconfig::setRGBA(EGLint r, EGLint g, EGLint b, EGLint a) {
  _attributes[EGL_RED_SIZE] = r;
  _attributes[EGL_GREEN_SIZE] = g;
  _attributes[EGL_BLUE_SIZE] = b;
  _attributes[EGL_ALPHA_SIZE] = a;
}
void EGLconfig::setDepth(EGLint d) {
  _attributes[EGL_DEPTH_SIZE] = d;
}

void EGLconfig::setSurface(EGLint s) {
  _attributes[EGL_SURFACE_TYPE] = s;
}

void EGLconfig::chooseConfig(EGLDisplay display) {
  std::vector<EGLint> attribs;

  for(auto it = _attributes.begin(); it != _attributes.end(); ++it) {
    attribs.push_back(it->first);
    attribs.push_back(it->second);
  }
  attribs.push_back(EGL_NONE);

  EGLBoolean result;
  EGLint numConfig;

  result = eglChooseConfig(display, &attribs[0], &_config, 1, &numConfig);
  std::cout << "Got numCofig = " << numConfig << std::endl;
  if(result == EGL_FALSE) {
    std::cerr << "Error setting config check your setting or if you have a valid display" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Config chosed successfully!" << std::endl;
}

EGLConfig EGLconfig::getConfig() const {
  return _config;
}

void EGLconfig::print() const {
  std::map<EGLint, EGLint>::const_iterator it;

  for(it = _attributes.begin(); it != _attributes.end(); it++) {
    std::cout << it->first << " => " << it->second << std::endl;
  }
}
