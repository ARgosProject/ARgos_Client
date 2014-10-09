#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "VideoThread.h"
#include "GfxProgram.h"

/**
 * A class representing a video
 * It allows to load a video from disk
 * [Experimental]
 */
class VideoComponent : public GraphicComponent {
public:
  /**
   * Constructs a new empty video
   * @param width The width of this graphic component
   * @param height The height of this graphic component
   */
  VideoComponent(GLfloat width, GLfloat height);

  /**
   * Constructs a new video loaded from disk
   * @param fileName The path of the video file to load
   * @param width The width of this graphic component
   * @param height The height of this graphic component
   * @see loadVideoFromFile()
   */
	VideoComponent(const std::string& fileName, GLfloat width, GLfloat height);

  /**
   * Destroys the video
   */
  ~VideoComponent();

  /**
   * Sets whether the video should start again on finish or not
   * @param loop True whether the video should start again on finish. False otherwise
   */
	void setLoop(bool loop);

  /**
   * Loads a video from disk
   * @param fileName The path of the video file to load
   */
	void loadVideoFromFile(const std::string& fileName);

  /**
   * Draws this graphic component
   */
  void render() override;

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
  GLuint _textureId; ///< The OpenGL texture id used to render the video

	bool _loop; ///< Whether loop the video or not
	std::string _fileName; ///< The file name of the video
	void* _eglImage; ///< An EGL surface used to render the video
	VideoThread* _videoThread; ///< An auxiliar thread used to decode the video
};

#endif
