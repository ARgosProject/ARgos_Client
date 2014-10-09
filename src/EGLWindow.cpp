#include "EGLWindow.h"
#include "EGLconfig.h"

#include <GLES2/gl2.h>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

namespace argosClient {

  EGLWindow::EGLWindow(EGLconfig* config) {
    _activeSurface = false;
    _upscale = false;

    _display = NULL;
    _context = NULL;
    _surface = NULL;

    int32_t success = 0;
    success = graphics_get_display_size(0, &_width, &_height);
    assert(success >= 0);
    std::cout << "Resolution: " << _width << "x" << _height << std::endl;

    if(!config) {
      std::cout << "Making new config" << std::endl;
      _config = new EGLconfig();
    }
    else {
      _config = config;
    }
  }

  EGLWindow::~EGLWindow() {
    glClear(GL_COLOR_BUFFER_BIT);
    destroySurface();
  }

  void EGLWindow::makeSurface() {
    makeSurface(0, 0, _width, _height);
  }

  void EGLWindow::setScreen(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    destroySurface();
    makeSurface(x, y, w, h);
  }

  void EGLWindow::makeSurface(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    static EGL_DISPMANX_WINDOW_T nativeWindow;
    EGLBoolean result;
    VC_RECT_T dstRect;
    VC_RECT_T srcRect;

    static const EGLint contextAttributes[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(_display == EGL_NO_DISPLAY) {
      std::cerr << "Error getting display" << std::endl;
      exit(EXIT_FAILURE);
    }

    int major, minor;
    result = eglInitialize(_display, &major, &minor);
    std::cout << "EGL init version " << major << "." << minor << std::endl;
    if(result == EGL_FALSE) {
      std::cerr << "Error initialising display" << std::endl;
      exit(EXIT_FAILURE);
    }

    _config->chooseConfig(_display);
    EGLConfig config = _config->getConfig();
    result = eglBindAPI(EGL_OPENGL_ES_API);
    if(result == EGL_FALSE) {
      std::cerr << "Error binding OpenGL ES API to EGL" << std::endl;
      exit(EXIT_FAILURE);
    }

    _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttributes);
    if(_context == EGL_NO_CONTEXT) {
      std::cerr << "Couldn't get a valid context" << std::endl;
      exit(EXIT_FAILURE);
    }

    // Create an EGL window surface the way this works is we set the dimensions of the srec
    // and destination rectangles.
    // If these are the same size there is no scaling, else the window will auto scale

    dstRect.x = x;
    dstRect.y = y;
    if(!_upscale) {
      dstRect.width = w;
      dstRect.height = h;
    }
    else {
      dstRect.width = _width;
      dstRect.height = _height;
    }

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.width = w << 16;
    srcRect.height = h << 16;

    _dispmanDisplay = vc_dispmanx_display_open(0);
    _dispmanUpdate = vc_dispmanx_update_start(0);
    _dispmanElement = vc_dispmanx_element_add(_dispmanUpdate, _dispmanDisplay,
                                              0, &dstRect, 0, &srcRect,
                                              DISPMANX_PROTECTION_NONE, 0,
                                              0, DISPMANX_NO_ROTATE);

    nativeWindow.element = _dispmanElement;
    nativeWindow.width = w;
    nativeWindow.height = h;

    vc_dispmanx_update_submit_sync(_dispmanUpdate);

    _surface = eglCreateWindowSurface(_display, config, &nativeWindow, NULL);
    assert(_surface != EGL_NO_SURFACE);

    result = eglMakeCurrent(_display, _surface, _surface, _context);
    assert(EGL_FALSE != result);

    _activeSurface = true;

    std::cout << "Surface created" << std::endl;
  }

  void EGLWindow::destroySurface() {
    if(_activeSurface) {
      eglSwapBuffers(_display, _surface);
      eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
      eglDestroySurface(_display, _surface);
      eglDestroyContext(_display, _context);
      eglTerminate(_display);
      _activeSurface = false;
    }
  }

  void EGLWindow::swapBuffers() const {
    eglSwapBuffers(_display, _surface);
  }

  void EGLWindow::resizeScreen(uint32_t w, uint32_t h) {
    destroySurface();
    makeSurface(0, 0, w, h);
  }

  uint32_t EGLWindow::getWidth() const {
    return _width;
  }

  uint32_t EGLWindow::getHeight() const {
    return _height;
  }

  void EGLWindow::setUpscale(bool f) {
    _upscale = f;
  }

  EGLDisplay& EGLWindow::getEGLDisplay() {
    return _display;
  }

  EGLContext& EGLWindow::getEGLContext() {
    return _context;
  }

}
