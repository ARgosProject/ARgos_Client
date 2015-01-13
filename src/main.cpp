// C/C++ stuff
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <csignal>

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Matrix support
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Camera stuff
#include <raspicam/raspicam_cv.h>
#include <CameraProjectorSystem.h>

// Task delegation, OpenGL Stuff and script engine
#include "TaskDelegation.h"
#include "GLContext.h"
#include "GraphicComponent.h"
#include "ImageComponent.h"
#include "AudioManager.h"

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
using namespace argosClient;

sig_atomic_t g_loop = true;

void signals_function_handler(int signum);

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cout << "Usage: " + std::string(argv[0]) + " <ip:port>" << std::endl;
    return 0;
  }

  atexit(bcm_host_deinit);
  bcm_host_init();

  // SIGINT registration
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = signals_function_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  Log::setColouredOutput(isatty(fileno(stdout)));
  Log::info("Launching ARgos Client...");

  // Task delegation stuff (client)
  TaskDelegation* td = new TaskDelegation();
  while((td->connect(argv[1]) < 0)) {
    usleep(1 * 1000 * 1000); // Wait 1 second before trying to reconnect
    if(!g_loop)
      exit(EXIT_FAILURE);
  }

  // Images
  cv::Mat currentFrame;     // current frame
  //cv::Mat projectorFrame;   // projector openCV frame

  // Window
  //const string projectorWindow = "Projector";

  Log::info("Loading camera and projector parameters...");
  //- Camera Parameters ----
  CameraProjectorSystem cameraProjector;
  cameraProjector.setCalibrationsPath("data/calibrations/");
  cameraProjector.load("calibrationCamera.yml", "calibrationProjector.yml", "CameraProjectorExtrinsics.yml");
  if(!cameraProjector.isValid()){
    Log::error("Camera or projector parameters is not set, need to run the calibrator tool.");
    exit(1);
  }

  //-- Open VideoCapture -----
  raspicam::RaspiCam_Cv Camera; // Internal camera
  /*cv::VideoCapture Camera(0);*/ // USB camera
  Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
  Camera.set(CV_CAP_PROP_FRAME_WIDTH, SCREEN_W_CAMERA);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, SCREEN_H_CAMERA);
  //Camera.set(CV_CAP_PROP_CONTRAST, 55);
  //Camera.set(CV_CAP_PROP_SATURATION, 55);
  //Camera.set(CV_CAP_PROP_GAIN, 55);

  Log::info("Opening camera...");
  Camera.open();
  if(!Camera.isOpened()) {
    Log::error("Failed opening the camera.");
    return -1;
  }
  Log::info("Camera open correctly.");
  Log::info("ARgos executing.");

  //Set the appropriate projection matrix so that rendering is done in a enrvironment like the real camera (without distorsion)
  cv::Size imgSize(SCREEN_W_CAMERA, SCREEN_H_CAMERA);
  cv::Size GlWindowSize(SCREEN_W, SCREEN_H);

  float projection_matrix[16];
  cameraProjector.getProjector().glGetProjectionMatrix(imgSize, GlWindowSize, projection_matrix, 0.05f, 1000.0f);
  Log::matrix(projection_matrix, Log::Colour::FG_DARK_GRAY);

  // OpenGL stuff
  GLContext& glContext = GLContext::getInstance();
  //uint32_t w = glContext.getWidth();
  //uint32_t h = glContext.getHeight();
  glContext.setUpscale(false);
  glContext.setScreen(0, 0, SCREEN_W, SCREEN_H);
  glContext.setProjectionMatrix(glm::make_mat4(projection_matrix));

  glContext.start();

  while(g_loop) {
    Camera.grab();
    Camera.retrieve(currentFrame);

    paper_t paper;
    td->run(currentFrame, paper, g_loop);

    glContext.update(paper);
    glContext.render();
  }

  Log::info("Stopping the camera...");
  Camera.release();

  AudioManager::getInstance().play("apagando.wav");
  usleep(2196 * 1000);

  Log::info("Releasing the OpenGL ES 2.0 context...");
  glContext.destroy();

  Log::info("Shutdown successed.");

  return 0;
}

void signals_function_handler(int signum) {
  switch(signum) {
  case SIGINT:
    Log::info("Signal " + std::to_string(signum) + " (SIGINT) caught.");
    g_loop = false;
    break;
  default:
    break;
  }
}
