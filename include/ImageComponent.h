#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <opencv2/opencv.hpp>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

/**
 * A class representing an image
 * It allows to load an image from disk or from an incoming OpenCV::Mat
 */
class ImageComponent : public GraphicComponent {
public:
  /**
   * Constructs a new empty image
   * @param width The width of this graphic component
   * @param height The height of this graphic component
   */
  ImageComponent(GLfloat width, GLfloat height);

  /**
   * Constructs a new image loaded from disk
   * @param file_name The path of the image file to load
   * @param width The width of this graphic component
   * @param height The height of this graphic component
   * @see loadImageFromFile()
   */
  ImageComponent(const std::string& file_name, GLfloat width, GLfloat height);

  /**
   * Destroys the image
   */
  ~ImageComponent();

  /**
   * Loads an image from disk
   * @param file_name The path of the image file to load
   */
  void loadImageFromFile(const std::string& file_name);

  /**
   * Loads an image from an incoming OpenCV::Mat
   * @param mat A reference to a OpenCV::Mat to load
   */
  void loadImageFromMat(cv::Mat& mat);

  /**
   * Draws this graphic component
   */
  void render() override;

  /**
   * Deletes the OpenGL texture used for this image
   */
  void deleteTexture();

private:
  /**
   * Sets up the shader for this graphic component
   */
  void setUpShader() override;

private:
  GLushort* _indices; ///< Indices defining the shared vertex of the triangles
  GLfloat* _vertexData; ///< Positions of the vertex and their uv mapping
  GLfloat _width; ///< The width of this graphic component
  GLfloat _height; ///< The height of this graphic component
  GLuint _textureId; ///< The OpenGL texture id used to render the image
};

#endif
