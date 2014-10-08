#ifndef EGLCONFIG_H
#define EGLCONFIG_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <map>

/**
 * A class used to config the EGL display
 */
class EGLconfig {
public:
  /**
   * Constructs a new config for the EGL window
   */
  EGLconfig();

  /**
   * Destroys the config
   */
  ~EGLconfig();

  /**
   * Sets the attribute / value pair
   * @param attrib The attribute we want to set
   * @param value The value we want to assign the prior attrib
   */
  void setAttribute(EGLint attrib, EGLint value);

  /**
   * Sets the depth colour size
   * @param r The red component
   * @param g The green component
   * @param b The blue component
   * @param a The alpha component
   */
  void setRGBA(EGLint r, EGLint g, EGLint b, EGLint a);

  /**
   * Sets the depth buffer size
   * @param d The depth buffer size
   */
  void setDepth(EGLint d);

  /**
   * Sets the surface type
   * @param s A combination (with ORs |) of EGL_WINDOW_BIG, EGL_PBUFFER_BIT, and EGL_PIXMAP_BIT
   */
  void setSurface(EGLint s);

  /**
   * Sets the config for the current values of this class
   * This must be called for any changes to take place
   * @param display The EGL display to apply the config
   */
  void chooseConfig(EGLDisplay display);

  /**
   * Gets the created config
   * @return the actual config
   */
  EGLConfig getConfig() const;

  /**
   * Prints the current config
   */
  void print() const;

private:
  std::map<EGLint, EGLint> _attributes; ///< A map of attributes
  EGLConfig _config; ///< The actual EGL Config
};

#endif
