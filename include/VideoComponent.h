#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <GLES2/gl2.h>
#include <opencv2/opencv.hpp>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A class representing a video
   * It allows to load a video from disk
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
     * Makes an OpenGL texture with the received frame
     * @param mat The OpenCV::Mat used as video frame
     */
    void makeVideoTexture(const cv::Mat& mat);

  private:
    /**
     * The specific logic used to draw this graphic component
     */
    void specificRender() override;

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
    cv::VideoCapture _videoReader; ///< The video file reader
    cv::Mat _videoFrame; ///< The current video frame read from the file
    bool _loop; ///< Whether loop the video or not
    std::string _fileName; ///< The file name of the video
  };

}

#endif
