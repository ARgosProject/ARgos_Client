#ifndef TASKDELEGATION_H
#define TASKDELEGATION_H

#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

/**
 * Protocolo de envío de datos:
 *
 *   Tipo  Tamaño     Datos
 * +-------------------------------
 * | int  | int  | unsigned char* |
 * +-------------------------------
 */

using boost::asio::ip::tcp;

struct paper_t {
  int id;
  float modelview_matrix[16];
};

class TaskDelegation {
public:
  enum Type {
    VECTOR_I      =  0,
    MATRIX_16F    =  1,
    CV_MAT        =  2,
    VIDEO_STREAM  =  3,
    PAPER         =  4,
  };

public:
  struct StreamType {
    Type type;
    int size;
    std::vector<unsigned char> data;
  };

public:
  TaskDelegation();
  ~TaskDelegation();

  int connect(const std::string& ip, const std::string& port);
  int reconnect();

  int receive(paper_t& paper);
  void proccessCvMat(StreamType& st, cv::Mat& mat);
  void proccessPaper(StreamType& st, paper_t& paper);
  void proccessInt(StreamType& st, int& value);
  void proccessMatrix16f(StreamType& st, float* matrix);

  int send();
  void addMatrix16f(const float* matrix);
  void addVectori(const std::vector<int>& vector);
  void addCvMat(const cv::Mat& mat);

  bool error();

private:
  boost::asio::io_service _ioService;
  tcp::socket* _tcpSocket;
  tcp::resolver* _tcpResolver;
  std::vector<unsigned char> _buff;
  std::string _ip, _port;
  bool _error;
};

#endif
