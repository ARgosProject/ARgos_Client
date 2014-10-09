#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

#include <GLES2/gl2.h>
#include <opencv2/opencv.hpp>

#include "GraphicComponent.h"
#include "VideoThread.h"
#include "GfxProgram.h"

using boost::asio::ip::udp;

namespace argosClient {

  /**
   * A class representing a networked video stream
   * It receives video from an endpoint and renders it
   * The video is actually a sequence of OpenCV::Mat retrieved from the server
   */
  class VideoStreamComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new empty video stream
     * @param width The width of this graphic component
     * @param height The height of this graphic component
     */
    VideoStreamComponent(GLfloat width, GLfloat height);

    /**
     * Destroys the video stream
     */
    ~VideoStreamComponent();

    /**
     * Launches an auxiliar thread used to receive the video
     * @param port The listening port
     * @see receiveVideo()
     */
    void startReceivingVideo(unsigned short port);

    /**
     * Makes an OpenGL texture with the received frame
     * @param mat The OpenCV::Mat used as video frame
     */
    void makeVideoTexture(const cv::Mat& mat);

    /**
     * Draws this graphic component
     */
    void render() override;

  private:
    /**
     * A method used as a thread which continuously waits for video frames
     * @param port The listening port
     */
    void receiveVideo(unsigned short port);

    /**
     * Sends video frames back to the sender
     * @param mat The OpenCV::Mat frame to send
     * @param udpSocket The UDP socket used to send the frames
     * @param udpSenderEndpoint The sender endpoint
     * @return the number of received bytes
     */
    size_t sendVideo(const cv::Mat& mat, udp::socket& udpSocket, const udp::endpoint& udpSenderEndpoint);

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

    bool _ready; ///< Whether the component is allowed to render or not
    bool _receive; ///< Whether the component has received a new frame or not
    cv::Mat _receivedFrame; ///< The received frame
    std::thread* _videoThread; ///< A thread object used to receive video concurrently

    /** @name Timeouts
     *  Timers used to simulate timeouts when no video is received for 1 second
     *  This way we can stop rendering the video stream component if no new video frame is received
     */
    ///@{
    std::chrono::time_point<std::chrono::high_resolution_clock> _begin;
    std::chrono::time_point<std::chrono::high_resolution_clock> _end;
    ///@}
  };

}

#endif
