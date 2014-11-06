#include "VideoStreamComponent.h"
#include "Log.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using boost::asio::ip::udp;

namespace argosClient {

  VideoStreamComponent::VideoStreamComponent(GLfloat width, GLfloat height)
    : _vertexData(NULL), _width(width), _height(height),
      _textureId(-1), _ready(false), _receive(false), _videoThread(NULL) {
    /**
     *    0__1
     *    | /|
     *    |/ |
     *    3__2
     */

    _indices = new GLushort[6] { 0, 1, 2, 0, 2, 3 };
    _vertexData = new GLfloat[20] {
    // X        Y        Z     U     V
      -_width,  _height, 0.0f, 0.0f, 0.0f, // Top-left
       _width,  _height, 0.0f, 1.0f, 0.0f, // Top-right
       _width, -_height, 0.0f, 1.0f, 1.0f, // Bottom-right
      -_width, -_height, 0.0f, 0.0f, 1.0f  // Bottom-left
    };

    // Set the shader
    this->loadGLProgram("shaders/video.glvs", "shaders/video.glfs");
  }

  VideoStreamComponent::~VideoStreamComponent() {
    delete [] _indices;
    delete [] _vertexData;

    glDeleteTextures(1, &_textureId);
  }

  void VideoStreamComponent::startReceivingVideo(unsigned short port) {
    _videoThread = new std::thread(&VideoStreamComponent::receiveVideo, this, port);
    _videoThread->detach();
  }

  void VideoStreamComponent::receiveVideo(unsigned short port) {
    boost::asio::io_service ioService;
    udp::endpoint endpoint(udp::v4(), port);
    udp::socket udpSocket(ioService, endpoint);

    while(1) {
      udp::endpoint udpSenderEndpoint;

      size_t bytes = 0;
      unsigned char type_buf[sizeof(int)];
      unsigned char size_buf[sizeof(int)];
      unsigned char* data_buf;
      int size, type;

      Log::video("Esperando nuevo fotograma de vídeo.");

      _begin = std::chrono::high_resolution_clock::now();

      // Type.
      udpSocket.receive_from(boost::asio::buffer(&type_buf, sizeof(int)), udpSenderEndpoint);
      memcpy(&type, &type_buf, sizeof(int));

      // Size.
      udpSocket.receive_from(boost::asio::buffer(&size_buf, sizeof(int)), udpSenderEndpoint);
      memcpy(&size, &size_buf, sizeof(int));

      // Mat data
      data_buf = new unsigned char[size];
      bytes = udpSocket.receive_from(boost::asio::buffer(data_buf, size), udpSenderEndpoint);
      std::vector<unsigned char> transformed(&data_buf[0], &data_buf[size]);
      delete [] data_buf;

      _receivedFrame = cv::imdecode(transformed, CV_LOAD_IMAGE_UNCHANGED);

      Log::video(std::to_string(bytes) + " bytes de video recibidos.");

      _ready = true;
      _receive = true;
    }
  }

  size_t VideoStreamComponent::sendVideo(const cv::Mat& mat, udp::socket& udpSocket, const udp::endpoint& udpSenderEndpoint) {
    size_t bytes = 0;
    std::vector<unsigned char> output;
    std::vector<unsigned char> mat_buff;
    std::vector<int> params;
    int type = 2;

    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(80);
    cv::imencode(".jpg", mat, mat_buff, params);

    unsigned char packet_type[sizeof(int)];
    memcpy(packet_type, &type, sizeof(int));

    int length = mat_buff.size();
    unsigned char packet_size[sizeof(int)];
    memcpy(packet_size, &length, sizeof(int));

    // Tipo
    output.insert(output.begin(), &packet_type[0], &packet_type[sizeof(int)]);
    bytes += udpSocket.send_to(boost::asio::buffer(output, output.size()), udpSenderEndpoint);
    output.clear();

    // Tamaño
    output.insert(output.begin(), &packet_size[0], &packet_size[sizeof(int)]);
    bytes += udpSocket.send_to(boost::asio::buffer(output, output.size()), udpSenderEndpoint);
    output.clear();

    // Datos
    bytes += udpSocket.send_to(boost::asio::buffer(mat_buff, mat_buff.size()), udpSenderEndpoint);

    return bytes;
  }

  void VideoStreamComponent::makeVideoTexture(const cv::Mat& mat) {
    // Byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);

    // Generate a texture object
    if(!glIsTexture(_textureId)) {
      glGenTextures(1, &_textureId);
    }

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create a gl texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
  }

  void VideoStreamComponent::setUpShader() {
    GLint id = _shader.getId();

    _vertexHandler = glGetAttribLocation(id, "a_position");
    _texHandler = glGetAttribLocation(id, "a_texCoord");
    _mvpHandler = glGetUniformLocation(id, "u_mvp");
    _samplerHandler = glGetUniformLocation(id, "s_texture");
  }

  void VideoStreamComponent::specificRender() {
    _end = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::seconds>(_end - _begin).count() > 1) {
      _ready = false;
    }

    if(_ready) {
      _shader.useProgram();

      glVertexAttribPointer(_vertexHandler, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _vertexData);
      glEnableVertexAttribArray(_vertexHandler);
      glVertexAttribPointer(_texHandler, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &_vertexData[3]);
      glEnableVertexAttribArray(_texHandler);

      glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix * _model));

      if(_receive) {
        makeVideoTexture(_receivedFrame);
      }

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _textureId);
      glUniform1i(_samplerHandler, 0);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, _indices);

      _receive = false;
    }
  }

}
