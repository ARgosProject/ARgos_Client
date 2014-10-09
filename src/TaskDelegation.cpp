#include "TaskDelegation.h"
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "Log.h"

namespace argosClient {

  TaskDelegation::TaskDelegation() : _ip("-1"), _port("-1"), _error(false) {
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

  int TaskDelegation::connect(const std::string& ip, const std::string& port) {
    try {
      _error = false;

      Log::info("Intentando conectar al servidor " + ip + ":" + port + "...");
      boost::asio::connect(*_tcpSocket, _tcpResolver->resolve({ip, port}));
      Log::success("Conectado satisfactoriamente.");
      _ip = ip; _port = port;

      return 0;
    }
    catch(boost::system::system_error const& e) {
      Log::error("No se pudo conectar al servidor de ARgos. " + std::string(e.what()));
      _error = true;
    }

    return -1;
  }

  int TaskDelegation::reconnect() {
    try {
      _error = false;

      Log::info("Intentando reconectar al servidor de ARgos...");
      boost::asio::connect(*_tcpSocket, _tcpResolver->resolve({_ip, _port}));
      Log::success("Reconectado satisfactoriamente.");

      return 0;
    }
    catch(boost::system::system_error const& e) {
      Log::error("No se pudo reconectar al servidor de ARgos. " + std::string(e.what()));
      _error = true;
    }

    return -1;
  }

  int TaskDelegation::send() {
    try {
      _error = false;

      int buff_size = _buff.size();
      Log::info("Intentando enviar " + std::to_string(buff_size) + " bytes...");
      int bytes = boost::asio::write(*_tcpSocket, boost::asio::buffer(_buff, buff_size));
      Log::success(std::to_string(bytes) + " bytes enviados.");
      _buff.clear();

      return bytes;
    }
    catch(boost::system::system_error const& e) {
      Log::error("Se perdió la conexión al servidor de ARgos al enviar. " + std::string(e.what()));
      _buff.clear();
      _error = true;
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

  bool TaskDelegation::error() {
    return _error;
  }

  int TaskDelegation::receive(paper_t& paper) {
    StreamType st;
    unsigned char type_buf[sizeof(int)];
    unsigned char size_buf[sizeof(int)];
    unsigned char* data_buf;
    int bytes = 0;

    try {
      _error = false;

      bytes += boost::asio::read(*_tcpSocket, boost::asio::buffer(&type_buf, sizeof(int)));  // Type
      memcpy(&st.type, &type_buf, sizeof(int));
      bytes += boost::asio::read(*_tcpSocket, boost::asio::buffer(&size_buf, sizeof(int))); // Size
      memcpy(&st.size, &size_buf, sizeof(int));

      data_buf = new unsigned char[st.size];
      bytes += boost::asio::read(*_tcpSocket, boost::asio::buffer(data_buf, st.size)); // Data
      st.data.insert(st.data.end(), &data_buf[0], &data_buf[st.size]);
      delete [] data_buf;

      switch(st.type) {
      case Type::VECTOR_I:
      case Type::MATRIX_16F:
        break;
      case Type::CV_MAT:
        //proccessCvMat(st, mat);
        break;
      case Type::PAPER:
        proccessPaper(st, paper);
        break;
      default:
        break;
      }
    }
    catch(boost::system::system_error const& e) {
      Log::error("Se perdió la conexión al servidor de ARgos al recibir. " + std::string(e.what()));
      _tcpSocket->close();
      //mat = cv::Scalar::all(0);
      _error = true;
    }

    return bytes;
  }

  void TaskDelegation::proccessCvMat(StreamType& st, cv::Mat& mat) {
    Log::success("Nueva cv::Mat recibida. Size: " + std::to_string(st.size));
    mat = cv::imdecode(st.data, CV_LOAD_IMAGE_COLOR);
  }

  void TaskDelegation::proccessPaper(StreamType& st, paper_t& paper) {
    Log::success("Nuevo Paper recibido. Size: " + std::to_string(st.size));

    proccessInt(st, paper.id);
    proccessMatrix16f(st, paper.modelview_matrix);
  }

  void TaskDelegation::proccessInt(StreamType& st, int& value) {
    memcpy(&value, &st.data[0], sizeof(int));
  }

  void TaskDelegation::proccessMatrix16f(StreamType& st, float* matrix) {
    Log::success("Nueva matriz de 16 float recibida. Size: " + std::to_string(st.size));

    //int num_floats = st.size/sizeof(float);
    //float tmp[num_floats];
    memcpy(&matrix[0], &st.data[sizeof(int)], sizeof(float)*(16));

    // Procesar matriz (tmp) aquí

  }

}
