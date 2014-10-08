// C/C++ stuff
#include <time.h>
#include <unistd.h>
#include <iostream>

// ffmpeg
#ifdef __cplusplus
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#endif

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Matrix support
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Camera stuff
#include <raspicam/raspicam_cv.h>

// Task delegation and OpenGL Stuff
#include "TaskDelegation.h"
#include "GLContext.h"

// RaspberryPi stuff
#include "bcm_host.h"

// Logger
#include "Log.h"

#define SCREEN_W 800
#define SCREEN_H 600
#define SCREEN_W_CAMERA 800
#define SCREEN_H_CAMERA 600

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  atexit(bcm_host_deinit);
  bcm_host_init();
  Logger::Log::setColouredOutput(isatty(fileno(stdout)));
  Logger::Log::info("Inicializando ARgos...");

  // Task delegation stuff (client)
  TaskDelegation* td = NULL;
  if(argc > 2) {
    td = new TaskDelegation();
    while(td->connect(argv[1], argv[2]) < 0) {
      usleep(1 * 1000 * 1000); // Wait 1 second before trying to reconnect
    }
  }

  // Images
  cv::Mat currentFrame;		  // current frame
  cv::Mat projectorFrame;	  // projector openCV frame

  // Window
  const string projectorWindow = "Projector";

  cout << "Loading camera & projector parameters... " << endl;
  //- Camera Parameters ----
  CameraProjectorSystem cameraProjector;
  cameraProjector.load("calibrationCamera.yml", "calibrationProjector.yml", "CameraProjectorExtrinsics.yml");
  if(!cameraProjector.isValid()){
    cout << "! Camera or projector parameters is not set, need to run the calibrator tool" << endl;
    exit(1);
  }

  //-- Open VideoCapture -----
  raspicam::RaspiCam_Cv Camera; // Internal camera
  /*cv::VideoCapture Camera(0);*/ // USB camera
  Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
  Camera.set(CV_CAP_PROP_FRAME_WIDTH, SCREEN_W_CAMERA);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, SCREEN_H_CAMERA);
  Camera.set(CV_CAP_PROP_CONTRAST, 55);
  Camera.set(CV_CAP_PROP_SATURATION, 55);
  //Camera.set(CV_CAP_PROP_GAIN, 55);

  Logger::Log::info("Abriendo cámara...");
  Camera.open();
  if(!Camera.isOpened()) {
    Logger::Log::error("Ocurrió un error al abrir el dispositivo.");
    return -1;
  }
  Logger::Log::info("Cámara abierta correctamente.");
  Logger::Log::info("ARgos en ejecución.");

  //Set the appropriate projection matrix so that rendering is done in a enrvironment like the real camera (without distorsion)
  cv::Size imgSize(SCREEN_W_CAMERA, SCREEN_H_CAMERA);
  cv::Size GlWindowSize(SCREEN_W, SCREEN_H);

  vector<string> invoices = {"NONE","NEOBIZ","SINOVO","ACTIVE"};
  string lastSearch = "NONE";

  float projection_matrix[16];
  cameraProjector.getProjector().glGetProjectionMatrix(imgSize, GlWindowSize, projection_matrix, 0.05f, 1000.0f);
  Logger::Log::matrix(projection_matrix);

  /*float projection_matrix[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };*/

  // OpenGL stuff
  GLContext& glContext = GLContext::getInstance();
  //uint32_t w = glContext.getWidth();
  //uint32_t h = glContext.getHeight();
  glContext.setUpscale(false);
  glContext.setScreen(0, 0, SCREEN_W, SCREEN_H);
  glContext.setProjectionMatrix(glm::make_mat4(projection_matrix));
  glContext.start();

  //-- Main Loop -------------------------------------------------------------------------
  while(1) {
    // New Frame
    Camera.grab(); Camera.retrieve(currentFrame);
    paper_t paper;

    if(!td->error()) {
      td->addCvMat(currentFrame);

      if(td->send() != 0) {
        td->receive(paper);
      }
    }
    else {
      while(td->reconnect() < 0) {
        usleep(1 * 1000 * 1000); // Wait 1 second before trying to reconnect
      }
    }

    glContext.setFrame(&currentFrame);
    //win->update(glm::make_mat4(paper.modelview_matrix));
    glContext.update(paper);
    glContext.render();

    //cv::imshow(projectorWindow, projectorFrame);
    //cv::waitKey(1);

    //---------------------------------------------------------------------
  }
  Logger::Log::info("Deteniendo la cámara...");
  Camera.release();
  Logger::Log::info("Todo apagado correctamente.");

  return 0;
}
