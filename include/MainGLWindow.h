#ifndef MAINGLWINDOW_H
#define MAINGLWINDOW_H

#include <opencv2/opencv.hpp>
#include <glm/mat4x4.hpp>

#include "paper.hpp"

#include "Singleton.h"
#include "EGLWindow.h"
#include "GfxProgram.h"
#include "GraphicComponent.h"
#include "TaskDelegation.h"

class LineComponent;

class MainGLWindow : public EGLWindow, public Singleton<MainGLWindow> {
 public:
  MainGLWindow(EGLconfig* config = NULL);
  ~MainGLWindow();

  // This is the main drawing function should only be called
  // once initalize has been called
  void render() override;
  void setFps(float fps);
  void update(paper_t& paper);

  void setPaper(Paper* p);
  void setFrame(cv::Mat* currentFrame);
  cv::Mat& getFrame();

  // Projection matrix
  void setProjectionMatrix(glm::mat4 projectionMatrix);
  const glm::mat4& getProjectionMatrix() const;

  // Once OpenGL initialisation
  void start() override;

  // Inherited from Singleton
  static MainGLWindow& getSingleton();
  static MainGLWindow* getSingletonPtr();

 private:
  std::vector<GraphicComponent*> _gc;
  std::vector<LineComponent*> _axis;
  std::vector<LineComponent*> _corners;

  Paper* _paper;
  cv::Mat* _frame;

  glm::mat4 _projectionMatrix;
};

#endif
