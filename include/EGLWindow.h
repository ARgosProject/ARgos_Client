#ifndef EGLWINDOW_H
#define EGLWINDOW_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <bcm_host.h>

#include "EGLconfig.h"

/**
 * A virtual EGL Window class used to initialize the OpenGL ES 2.0 context
 */
class EGLWindow {
public:
  /**
   * Constructs a new OpenGL ES 2.0 context
   * @param config The config we want for the context. If NULL, a default
   * config is built
   */
  EGLWindow(EGLconfig* config = NULL);

  /**
   * Destroys the EGL window and its surface
   */
  virtual ~EGLWindow();

  /**
   * It must be implemented by the user. This will be called
   * after the constructor once we have a valid GL context
   */
  virtual void render() = 0;

  /**
   * Tells EGL to re-draw the current buffer
   */
  virtual void swapBuffers() const;

  /**
   * Resizes the screen with origin at 0,0
   * @param w The width of the screen
   * @param h The height of the screen
   */
  virtual void resizeScreen(uint32_t w, uint32_t h);

  /**
   * (Re)Initializes the screen using X, Y coordinates and W, H size
   * @param x The X coordinate
   * @param y The Y coordinate
   * @param w The width of the screen
   * @param h The height of the screen
   */
  virtual void setScreen(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

  /**
   * Gets the width of the screen
   * @return the width of the screen
   */
  virtual uint32_t getWidth() const;

  /**
   * Gets the height of the screen
   * @return the height of the screen
   */
  virtual uint32_t getHeight() const;

  /**
   * Sets the flag to upscale the screen destination rectangle
   * It's not set by default
   * @param f Whether set the flag or not
   */
  virtual void setUpscale(bool f);

  /**
   * Sets up specific properties for the context
   */
  virtual void start() = 0;

  /**
   * Gets the EGL display
   * @return the EGL display
   */
	virtual EGLDisplay& getEGLDisplay();

  /**
   * Gets the EGL context
   * @return the EGL context
   */
	virtual EGLContext& getEGLContext();

protected:
  /**
   * Make a new surface
   */
  void makeSurface();

  uint32_t _width; ///< The width of the screen
  uint32_t _height; ///< The height of the screen
  EGLDisplay _display; ///< The current EGL display
  EGLSurface _surface; ///< The current EGL surface
  EGLContext _context; ///< The current EGL context
  EGLconfig* _config; ///< The EGL config for this window

  DISPMANX_ELEMENT_HANDLE_T _dispmanElement; ///< The dispmanx representation of a visual object
  DISPMANX_DISPLAY_HANDLE_T _dispmanDisplay; ///< The dispmanx 'handle' on the display
  DISPMANX_UPDATE_HANDLE_T _dispmanUpdate; ///< The dispmanx transaction object

private:
  /**
   * Destroys the surface if it exists
   */
  void destroySurface();

  /**
   * Actually, this is the main function to create our surface
   */
  void makeSurface(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

  bool _activeSurface; ///< Whether we have an active surface or not
  bool _upscale; ///< Whether to upscale the screen or not
};

#endif
