#ifndef TASKDELEGATION_H
#define TASKDELEGATION_H

#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

using boost::asio::ip::tcp;

/**
 * A symbolic paper struct used to hold
 * important data from the real paper class
 */
struct paper_t {
  int id; ///< The Paper id
  float modelview_matrix[16]; ///< The model view matrix of the Paper
};

/**
 * A class used to communicate with the server
 * It provides bidirectional communication with the server,
 * allowing to receive and send some data structures.
 */
class TaskDelegation {
public:
  /**
   * An enum stating the different data structures the
   * class can handle
   */
  enum Type {
    VECTOR_I      =  0,
    MATRIX_16F    =  1,
    CV_MAT        =  2,
    VIDEO_STREAM  =  3,
    PAPER         =  4,
  };

public:
  /**
   * The structure used to hold raw data
   * It matches the convention used for client and server
   * to exchange data:
   *
   *   Data type   Data size      Raw data
   * +----------------------------------------+
   * |    int   |     int    | unsigned char* |
   * +----------------------------------------+
   */
  struct StreamType {
    Type type; ///< The data type the structure holds
    int size; ///< The size of the data variable
    std::vector<unsigned char> data; ///< The raw data
  };

public:
  /**
   * Constructs a new Task Delegation module
   */
  TaskDelegation();

  /**
   * Destroys the Task Delegation
   */
  ~TaskDelegation();

  /**
   * Tries to connect the Task Delegation with the server
   * @param ip The IP of the server
   * @param port The port of the server application
   * @return 0 if no errors. Otherwise < 0
   */
  int connect(const std::string& ip, const std::string& port);

  /**
   * Tries to reconnect to the server if communication has been lost
   * @return 0 if no errors. Otherwise < 0
   */
  int reconnect();

  /**
   * Starts to receive paper's data from the server and process it
   * @param paper A reference to the paper we want to save the received data
   * @return the number of received bytes
   */
  int receive(paper_t& paper);

  /**
   * Processes and build an OpenCv::Mat from raw data
   * @param st The raw data structure
   * @param mat A reference to the OpenCV::Mat we want to build against
   */
  void proccessCvMat(StreamType& st, cv::Mat& mat);

  /**
   * Processes and build a paper from raw data
   * @param st The raw data structure
   * @param paper A reference to the paper we want to build against
   */
  void proccessPaper(StreamType& st, paper_t& paper);

  /**
   * Processes and build an integer value from raw data
   * @param st The raw data structure
   * @param paper A reference to the integer variable we want to build against
   */
  void proccessInt(StreamType& st, int& value);

  /**
   * Processes and build a 16 floats array (matrix) from raw data
   * @param st The raw data structure
   * @param paper A reference to the floats array we want to build against
   */
  void proccessMatrix16f(StreamType& st, float* matrix);

  /**
   * Sends the built _buff object to the server
   * @return the number of sent bytes
   */
  int send();

  /**
   * Adds a 16 floats array (matrix) to the _buff object
   * @param matrix A 16 floats array
   */
  void addMatrix16f(const float* matrix);

  /**
   * Adds an integer list to the _buff object
   * @param vector The list of integers
   */
  void addVectori(const std::vector<int>& vector);

  /**
   * Adds an OpenCV::Mat to the _buff object
   * @param mat The OpenCV::Mat
   */
  void addCvMat(const cv::Mat& mat);

  /**
   * Checks the _error variable looking for errors
   * @return true if there was any error
   */
  bool error();

private:
  boost::asio::io_service _ioService; ///< The needed I/O service for establishing communications
  tcp::socket* _tcpSocket; ///< The TCP socket object used for communication
  tcp::resolver* _tcpResolver; ///< Query resolver to a list of endpoints
  std::vector<unsigned char> _buff; ///< Raw data buffer used to be sent to the server
  std::string _ip; ///< The IP of the connected endpoint
  std::string _port; ///< The Port of the connected endpoint
  bool _error; ///< Control variable used to handle errors
};

#endif
