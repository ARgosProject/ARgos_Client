#ifndef EGLCONFIG_H
#define EGLCONFIG_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <map>

class EGLconfig {
public:
  EGLconfig();
  ~EGLconfig();

  // Sets the attribute / value pair
  void setAttribute(EGLint attrib, EGLint value);

  // Sets the R,G,B,A bit size
  void setRGBA(EGLint r, EGLint g, EGLint b, EGLint a);

  // Sets the depth buffer size
  void setDepth(EGLint d);

  // Sets the surface type. Combination | of EGL_WINDOW_BIT,
  // EGL_PBUFFER_BIT, and EGL_PIXMAP_BIT
  void setSurface(EGLint s);

  // Sets the config for the current values of this class
  // this must be called for any changes to take place
  void chooseConfig(EGLDisplay display);

  // Get the config create with choose to the client
  EGLConfig getConfig() const;

  // Print the current config
  void print() const;

private:
  std::map<EGLint, EGLint> _attributes;
  EGLConfig _config;
};

#endif
