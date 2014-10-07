#ifndef EGLWINDOW_H
#define EGLWINDOW_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <bcm_host.h>

#include "EGLconfig.h"

class EGLWindow {
public:
  EGLWindow(EGLconfig* config = NULL);
  virtual ~EGLWindow();

  // Must be implemented by the user. This will be
  // called after the ctor once we have a valid GL context
  virtual void render() = 0;

  // Tells EGL to re-draw the current buffer
  virtual void swapBuffers() const;

  // Resizes the screen with origin at 0,0
  virtual void resizeScreen(uint32_t w, uint32_t h);

  // Sets screen using x,y and w,h
  virtual void setScreen(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

  // Gets the width
  virtual uint32_t getWidth() const;

  // Gets the height
  virtual uint32_t getHeight() const;

  // Sets the flag to upscale the screen dst rectangle.
  // By default this is not set
  virtual void setUpscale(bool f);

  // This must be implemented by the user. It is called only
  // once when the window is created
  virtual void start() = 0;

	virtual EGLDisplay& getEGLDisplay();
	virtual EGLContext& getEGLContext();

protected:
  // Make our surface
  void makeSurface();

  uint32_t _width;
  uint32_t _height;
  EGLDisplay _display;
  EGLSurface _surface;
  EGLContext _context;
  EGLconfig* _config;

  DISPMANX_ELEMENT_HANDLE_T _dispmanElement;
  DISPMANX_DISPLAY_HANDLE_T _dispmanDisplay;
  DISPMANX_UPDATE_HANDLE_T _dispmanUpdate;

private:
  // Destroy the surface if it exists
  void destroySurface();

  // This is the main function to create our surface
  void makeSurface(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

  bool _activeSurface;
  bool _upscale;
};

#endif
