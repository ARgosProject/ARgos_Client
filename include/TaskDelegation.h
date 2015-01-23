#ifndef TASKDELEGATION_H
#define TASKDELEGATION_H

#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

using boost::asio::ip::tcp;

namespace argosClient {

  /**
   * An enum used to hold the function calls from the
   * script engine
   */
  enum CallingFunctionType {
    NONE                     = -1,

    DRAW_IMAGE               =  0,
    DRAW_VIDEO               =  1,
    DRAW_CORNERS             =  2,
    DRAW_AXIS                =  3,
    INIT_VIDEO_STREAM        =  4,
    DRAW_TEXT_PANEL          =  5,
    DRAW_HIGHLIGHT           =  6,
    DRAW_BUTTON              =  7,
    DRAW_FACTURE_HINT        =  8,

    PLAY_SOUND               =  9,
    PLAY_SOUND_DELAYED       = 10
  };

  struct CallingFunctionData {
    CallingFunctionType id;
    std::vector<std::string> args;
  };

  /**
   * A symbolic paper struct used to hold
   * important data from the real paper class
   */
  struct paper_t {
    int id; ///< The Paper id
    float modelview_matrix[16]; ///< The model view matrix of the Paper
    int x, y; ///< The point of the document where the finger is
    int num_calling_functions; ///< The number of calling functions for this Paper
    std::vector<CallingFunctionData> cfds; ///< A list of calling function for this Paper
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
      SKIP          = -1,

      VECTOR_I      =  0,
      MATRIX_16F    =  1,
      CV_MAT        =  2,
      PAPER         =  3,
      VIDEO_STREAM  =  4
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
    int connect(const std::string& socketStr);

    /**
     * Tries to reconnect to the server if communication has been lost
     * @return 0 if no errors. Otherwise < 0
     */
    int reconnect();

    /**
     * Starts the main loop of the Task Delegation
     *
     */
    void run(const cv::Mat& mat, paper_t& paper, sig_atomic_t& g_loop);

    /**
     * Read a StreamType structure from the socket
     * @param socket The socket from read
     * @param st The StreamType structure to fill
     * @return The number of bytes received
     */
    int readStreamTypeFromSocket(tcp::socket &socket, StreamType &st);

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
    void processCvMat(StreamType& st, cv::Mat& mat);

    /**
     * Processes and build a paper from raw data
     * @param st The raw data structure
     * @param paper A reference to the paper we want to build against
     */
    void processPaper(StreamType& st, paper_t& paper);

    /**
     * Processes and build an integer value from raw data
     * @param st The raw data structure
     * @param value The value to save the result
     */
    void nextInt(StreamType& st, int& value);

    /**
     * Processes and build a float value from raw data
     * @param st The raw data structure
     * @param value The value to save the result
     */
    void nextFloat(StreamType& st, float& value);

    /**
     * Processes and build a string from raw data
     * @param st The raw data structure
     * @param chars The value to save the result
     * @param num_chars The number of characters to save
     */
    void nextChars(StreamType& st, char* chars, int num_chars);

    /**
     * Processes and build a 16 floats array (matrix) from raw data
     * @param st The raw data structure
     * @param paper A reference to the floats array we want to build against
     */
    void nextMatrix16f(StreamType& st, float* matrix);

    /**
     * Processes and build a CallingFunctionData structure
     * @param st The raw data structure
     * @param cfd A reference to the calling function data we want to build against
     */
    void nextCallingFunctionData(StreamType& st, int num, std::vector<CallingFunctionData>& cfds);

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
     * @return < 0 if there was any error
     */
    int error() const;

  private:
    boost::asio::io_service _ioService; ///< The needed I/O service for establishing communications
    tcp::socket* _tcpSocket; ///< The TCP socket object used for communication
    tcp::resolver* _tcpResolver; ///< Query resolver to a list of endpoints
    std::vector<unsigned char> _buff; ///< Raw data buffer used to be sent to the server
    std::string _ip; ///< The IP of the connected endpoint
    std::string _port; ///< The Port of the connected endpoint
    int _error; ///< Control variable used to handle errors
    int _offset; ///< The offset used by "next" functions
  };

}

#endif
