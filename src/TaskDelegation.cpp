#include "TaskDelegation.h"
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "Log.h"

namespace argosClient {

  TaskDelegation::TaskDelegation() : _ip("-1"), _port("-1"), _error(0), _offset(0) {
    _tcpSocket = new tcp::socket(_ioService);
    _tcpResolver = new tcp::resolver(_ioService);
  }

  TaskDelegation::~TaskDelegation() {
    if(_tcpResolver)
      delete _tcpResolver;

    if(_tcpSocket) {
      _tcpSocket->close();
      delete _tcpSocket;
    }
  }

  int TaskDelegation::connect(const std::string& socketStr) {
    try {
      _error = 0;

      std::stringstream ss(socketStr);
      std::getline(ss, _ip, ':');
      std::getline(ss, _port, ':');

      Log::info("Trying to connect to the server at " + _ip + ":" + _port + "...");
      boost::asio::connect(*_tcpSocket, _tcpResolver->resolve({_ip, _port}));
      Log::success("Connection succeeded.");

    }
    catch(boost::system::system_error const& e) {
      Log::error("Could not connect to the server. " + std::string(e.what()));
      _error = -1;
    }

    return _error;
  }

  int TaskDelegation::reconnect() {
    try {
      _error = 0;

      Log::info("Trying to reconnect to the server.");
      boost::asio::connect(*_tcpSocket, _tcpResolver->resolve({_ip, _port}));
      Log::success("Reconnection succeeded.");
    }
    catch(boost::system::system_error const& e) {
      Log::error("Could not reconnect to the server. " + std::string(e.what()));
      _error = -1;
    }

    return _error;
  }

  void TaskDelegation::run(const cv::Mat& mat, paper_t& paper) {
    if(_error < 0) {
      while(reconnect() < 0) {
        usleep(1 * 1000 * 1000); // Wait 1 second before trying to reconnect
      }
    }

    addCvMat(mat);
    send();
    receive(paper);
  }

  int TaskDelegation::send() {
    try {
      _error = 0;

      int buff_size = _buff.size();
      Log::info("Sending " + std::to_string(buff_size) + " bytes...");
      int bytes = boost::asio::write(*_tcpSocket, boost::asio::buffer(_buff, buff_size));
      Log::success(std::to_string(bytes) + " bytes sent.");
      _buff.clear();

      return bytes;
    }
    catch(boost::system::system_error const& e) {
      Log::error("Conection lost with the server when sending. " + std::string(e.what()));
      _buff.clear();
      _error = -1;
    }

    return 0;
  }

  void TaskDelegation::addMatrix16f(const float* matrix) {
    int size = 16 * sizeof(float);
    unsigned char sMatrix[size];
    int type = Type::MATRIX_16F;
    memcpy(sMatrix, matrix, size);

    unsigned char packet_type[sizeof(int)];
    memcpy(packet_type, &type, sizeof(int));

    unsigned char packet_size[sizeof(int)];
    memcpy(packet_size, &size, sizeof(int));

    _buff.insert(_buff.end(), &packet_type[0], &packet_type[sizeof(int)]); // Tipo
    _buff.insert(_buff.end(), &packet_size[0], &packet_size[sizeof(int)]); // Tamaño
    _buff.insert(_buff.end(), &sMatrix[0], &sMatrix[size]);                // Datos
  }

  void TaskDelegation::addVectori(const std::vector<int>& vector) {
    int size = vector.size() * sizeof(int);
    unsigned char sVector[size];
    int type = Type::VECTOR_I;
    memcpy(sVector, &vector[0], size);

    unsigned char packet_type[sizeof(int)];
    memcpy(packet_type, &type, sizeof(int));

    unsigned char packet_size[sizeof(int)];
    memcpy(packet_size, &size, sizeof(int));

    _buff.insert(_buff.end(), &packet_type[0], &packet_type[sizeof(int)]); // Tipo
    _buff.insert(_buff.end(), &packet_size[0], &packet_size[sizeof(int)]); // Tamaño
    _buff.insert(_buff.end(), &sVector[0], &sVector[size]);                // Datos
  }

  void TaskDelegation::addCvMat(const cv::Mat& mat) {
    std::vector<unsigned char> mat_buff;
    std::vector<int> params;
    int type = Type::CV_MAT;

    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(80);
    cv::imencode(".jpg", mat, mat_buff, params);

    unsigned char packet_type[sizeof(int)];
    memcpy(packet_type, &type, sizeof(int));

    int length = mat_buff.size();
    unsigned char packet_size[sizeof(int)];
    memcpy(packet_size, &length, sizeof(int));

    _buff.insert(_buff.end(), &packet_type[0], &packet_type[sizeof(int)]); // Tipo
    _buff.insert(_buff.end(), &packet_size[0], &packet_size[sizeof(int)]); // Tamaño
    _buff.insert(_buff.end(), mat_buff.begin(), mat_buff.end());           // Datos
  }

  int TaskDelegation::error() const {
    return _error;
  }

  int TaskDelegation::readStreamTypeFromSocket(tcp::socket &socket, StreamType &st) {
    int bytes = 0;

    unsigned char type_buf[sizeof(int)];
    unsigned char size_buf[sizeof(int)];
    unsigned char* data_buf;

    Log::info("Waiting for paper...");
    bytes += boost::asio::read(socket, boost::asio::buffer(&type_buf, sizeof(int)));  // Type
    memcpy(&st.type, &type_buf, sizeof(int));

    if(st.type != Type::SKIP) {
      bytes += boost::asio::read(socket, boost::asio::buffer(&size_buf, sizeof(int))); // Size
      memcpy(&st.size, &size_buf, sizeof(int));
      data_buf = new unsigned char[st.size];
      bytes += boost::asio::read(socket, boost::asio::buffer(data_buf, st.size)); // Data
      st.data.insert(st.data.end(), &data_buf[0], &data_buf[st.size]);
      delete [] data_buf;

      Log::info("PAPER received.");
    }
    else {
      Log::info("SKIP received.");
    }

    return bytes;
  }

  int TaskDelegation::receive(paper_t& paper) {
    int bytes = 0;

    try {
      _error = 0;

      StreamType st;
      bytes += readStreamTypeFromSocket(*_tcpSocket, st);

      switch(st.type) {
      case Type::SKIP:
      case Type::PAPER:
        processPaper(st, paper);
        break;
      default:
        break;
      }
    }
    catch(boost::system::system_error const& e) {
      Log::error("Conection lost with the server when receiving. " + std::string(e.what()));
      _tcpSocket->close();
      _error = -1;
    }

    return bytes;
  }

  void TaskDelegation::processCvMat(StreamType& st, cv::Mat& mat) {
    Log::success("New cv::Mat received. Size: " + std::to_string(st.size));
    mat = cv::imdecode(st.data, CV_LOAD_IMAGE_COLOR);
  }

  void TaskDelegation::processPaper(StreamType& st, paper_t& paper) {
    if(st.type == Type::SKIP) {
      paper.id = 0;
      for(int i = 0; i < 16; ++i)
        paper.modelview_matrix[i] = 0.0f;
    }
    else {
      _offset = 0;
      nextInt(st, paper.id);
      nextMatrix16f(st, paper.modelview_matrix);
      //nextInt(st, paper.num_calling_functions);
      //nextCallingFunctionData(st, paper.num_calling_functions, paper.cfds);

      //Log::success("Id: " + std::to_string(paper.id) + ". Num. functions: " + std::to_string(paper.cfds.size()));
      Log::matrix(paper.modelview_matrix);
    }
  }

  void TaskDelegation::nextInt(StreamType& st, int& value) {
    memcpy(&value, &st.data[_offset], sizeof(int));
    _offset += sizeof(int);
  }

  void TaskDelegation::nextChars(StreamType& st, char* chars, int num_chars) {
    memcpy(&chars, &st.data[_offset], sizeof(char) * num_chars);
    _offset += sizeof(char) * num_chars;
  }

  void TaskDelegation::nextMatrix16f(StreamType& st, float* matrix) {
    memcpy(&matrix[0], &st.data[_offset], sizeof(float) * 16);
    _offset += sizeof(float)*16;
  }

  void TaskDelegation::nextCallingFunctionData(StreamType& st, int num, std::vector<CallingFunctionData>& cfds) {
    for(int i = 0; i < num; ++i) {
      CallingFunctionData cfd;
      int id = static_cast<int>(cfd.id);
      nextInt(st, id);
      cfd.id = static_cast<CallingFunctionType>(id);

      char svalue[32] = "";
      int ivalue = 0;
      float fvalue = 0.0f;

      switch(cfd.id) {
      case NONE:
        break;
      case CREATE_IMAGE_FROM_FILE:
        break;
      case CREATE_VIDEO_FROM_FILE:
        break;
      case CREATE_CORNERS:
        break;
      case CREATE_AXIS:
        break;
      case CREATE_VIDEO_STREAM:
        break;
      case CREATE_TEXT_PANEL:
        break;
      case CREATE_HIGHLIGHT:
        break;
      case CREATE_BUTTON:
        break;
      case CREATE_FACTURE_HINT:
        break;
      case PLAY_SOUND:
        nextChars(st, svalue, 32); // filename
        nextInt(st, ivalue); // loops
        cfd.args.push_back(std::string(svalue));
        cfd.args.push_back(std::to_string(ivalue));
        break;
      case PLAY_SOUND_DELAYED:
        nextChars(st, svalue, 32); // filename
        nextInt(st, ivalue); // seconds to wait among plays
        cfd.args.push_back(std::string(svalue));
        cfd.args.push_back(std::to_string(ivalue));
        break;
      }

      cfds.push_back(cfd);
    }
  }

}
