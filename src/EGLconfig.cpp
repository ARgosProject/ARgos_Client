#include "EGLconfig.h"

#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "Log.h"

namespace argosClient {

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
    Log::info("Got numCofig = " + std::to_string(numConfig) + ".");
    if(result == EGL_FALSE) {
      Log::error("Setting config. Check your setting or if you have a valid display.");
      exit(EXIT_FAILURE);
    }

    Log::success("Config chosed successfully!");
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

}
