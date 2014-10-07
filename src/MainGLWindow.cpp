#include <iostream>
#include <cassert>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MainGLWindow.h"
#include "GraphicComponent.h"
#include "LineComponent.h"
#include "TextComponent.h"
#include "RectangleComponent.h"
#include "CircleComponent.h"
#include "ImageComponent.h"
#include "TextureRender.h"
#include "VideoStreamComponent.h"
#include "Log.h"

template<> MainGLWindow* Singleton<MainGLWindow>::_singleton = 0;

MainGLWindow::MainGLWindow(EGLconfig* config)
  : EGLWindow(config), _paper(NULL), _frame(NULL) {

}

MainGLWindow::~MainGLWindow() {
  for(GraphicComponent* gc : _gc) {
    delete gc;
  }
}

void MainGLWindow::start() {
  // Texture render
  /*_rectangleComponent = new RectangleComponent(1.0f, 1.0f, true); // Size [0..1]
    _rectangleComponent->setColor(1.0f, 0.0f, 0.0f, 1.0f);

    _textComponent = new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32, true);
    _textComponent->setColor(0.0f, 0.0f, 1.0f, 1.0f);
    _textComponent->setScale(0.01f);
    _textComponent->setPosition(-100.0f, 0.0f);
    _textComponent->setText(L"Texto en TextureRender");

    _textureRender = new TextureRender(1024.0f, 768.0f);
    _textureRender->addGraphicComponent(_rectangleComponent);
    _textureRender->addGraphicComponent(_textComponent);*/

  // Lonely rectangle
  //_rectangleComponent2 = new RectangleComponent(21.0f, 29.7f);
  //_rectangleComponent2 = new RectangleComponent(29.7f, 21.0f);
  //_rectangleComponent2 = new RectangleComponent(1.0f, 1.0f);
  //_rectangleComponent2->setScale(0.25f);
  //_rectangleComponent2->setPosition();
  //_rectangleComponent2->setColor(1.0f, 0.0f, 0.0f, 1.0f);

  // Lonely text
  /*_fpsComponent = new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32);
    _fpsComponent->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    _fpsComponent->setScale(0.01f);
    _fpsComponent->setPosition(-100.0f, 0.0f);*/

  // Lonely image
  /*_imageComponent = new ImageComponent("images/UCLM.png", 1.0f, 1.0f);
    _imageComponent->setPosition(0.0f, 0.0f, 0.0f);*/

  // Frame de la camara
  ImageComponent* im = new ImageComponent(1.0, 1.0);
  //im->setProjectionMatrix(_projectionMatrix);
  _gc.push_back(im);

  // Fondo
  //ImageComponent* image = new ImageComponent("background_distorted.jpg", 1.0, 1.0);
  //_gc.push_back(image);

  // Cuatro circulos dibujados sobre las cuatro esquinas del A4
  /*for(int i = 0; i < 4; ++i) {
    CircleComponent* corner = new CircleComponent(2.0f); // Corregir bug: Hay que cambiar el (size = 2(radius=center)) del shader.
    corner->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    corner->setProjectionMatrix(_projectionMatrix);
    _gc.push_back(corner);
    }*/

  //VideoComponent* video = new VideoComponent("test.h264", 0.5f, 0.5f);
  //video->setLoop(true);
  //video->setProjectionMatrix(_projectionMatrix);
  //_gc.push_back(video);

  // Video stream
  VideoStreamComponent* videoStream = new VideoStreamComponent(0.5f, 0.5f);
  videoStream->startReceivingVideo(9999);
  videoStream->setProjectionMatrix(_projectionMatrix);
  _gc.push_back(videoStream);

  // Axis
  LineComponent* x_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0));
  x_line->setColor(1, 0, 0, 1);
  x_line->setProjectionMatrix(_projectionMatrix);
  LineComponent* y_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, 2, 0));
  y_line->setColor(0, 1, 0, 1);
  y_line->setProjectionMatrix(_projectionMatrix);
  LineComponent* z_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, 0, 2));
  z_line->setColor(0, 0, 1, 1);
  z_line->setProjectionMatrix(_projectionMatrix);
  _axis.push_back(x_line);
  _axis.push_back(y_line);
  _axis.push_back(z_line);

  // Corners
  const float axis_length = 2.0f;
  LineComponent* corners[8] = {
    // Top-Left
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0)), // 0
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -axis_length, 0)),  // 1

    // Top-Right
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-axis_length, 0, 0)),  // 2
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -axis_length, 0)),  // 3

    // Bot-Left
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0)), // 4
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0)), // 5

    // Bot-Right
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-axis_length, 0, 0)),  // 6
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0))  // 7
  };
  for(int i = 0; i < 8; ++i) {
    corners[i]->setColor(1, 0, 0, 1);
    corners[i]->setProjectionMatrix(_projectionMatrix);
    _corners.push_back(corners[i]);
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  std::cout << "MainGLWindow initialized" << std::endl;
}

void MainGLWindow::update(paper_t& paper) {
  std::cout << "- id: " << paper.id << std::endl;
  Logger::Log::matrix(paper.modelview_matrix);

  size_t gc_length = _gc.size();
  for(size_t i = 1; i < gc_length; ++i) {
    _gc[i]->setModelViewMatrix(glm::make_mat4(paper.modelview_matrix));
  }

  size_t axis_length = _axis.size();
  for(size_t i = 0; i < axis_length; ++i) {
    _axis[i]->setModelViewMatrix(glm::make_mat4(paper.modelview_matrix));
  }

  size_t corners_length = _corners.size();
  for(size_t i = 0; i < corners_length; ++i) {
    _corners[i]->setModelViewMatrix(glm::make_mat4(paper.modelview_matrix));
  }

  float width = 21.0f;
  float height = 29.7f;
  glm::vec3 topLeftCorner(-width / 2.0f, height / 2.0f, 0.0f);
  glm::vec3 topRightCorner(width / 2.0f, height / 2.0f, 0.0f);
  glm::vec3 botLeftCorner(-width / 2.0f, -height / 2.0f, 0);
  glm::vec3 botRightCorner(width / 2.0f, -height / 2.0f, 0);

  _corners[0]->translate(topLeftCorner);
  _corners[1]->translate(topLeftCorner);
  _corners[2]->translate(topRightCorner);
  _corners[3]->translate(topRightCorner);
  _corners[4]->translate(botLeftCorner);
  _corners[5]->translate(botLeftCorner);
  _corners[6]->translate(botRightCorner);
  _corners[7]->translate(botRightCorner);

  //_gc[1]->translate(botLeftCorner);  // Bottom-Left
  //_gc[2]->translate(topLeftCorner);  // Top-Left
  //_gc[3]->translate(botRightCorner); // Bottom-Right
  //_gc[4]->translate(topRightCorner); // Top-Right
}

void MainGLWindow::render() {
  // Clears the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, _width, _height);

  // Renderizamos el frame de la cámara
  //((ImageComponent*)_gc[0])->loadImageFromMat(*_frame);
  //_gc[0]->render();
  //((ImageComponent*)_gc[0])->deleteTexture();

  // Renderizamos todos los objetos
  size_t gc_length = _gc.size();
  for(size_t i = 1; i < gc_length; ++i) {
    _gc[i]->render();
  }

  // Axis
  size_t axis_length = _axis.size();
  for(size_t i = 0; i < axis_length; ++i) {
    _axis[i]->render();
  }

  // Corners
  size_t corners_length = _corners.size();
  for(size_t i = 0; i < corners_length; ++i) {
    _corners[i]->render();
  }

  // To update we need to swap the buffers
  swapBuffers();
}

void MainGLWindow::setPaper(Paper* p) {
  _paper = p;
}

void MainGLWindow::setFrame(cv::Mat* currentFrame) {
  _frame = currentFrame;
}

cv::Mat& MainGLWindow::getFrame() {
  return *_frame;
}

void MainGLWindow::setProjectionMatrix(glm::mat4 projectionMatrix) {
  _projectionMatrix = projectionMatrix;
}

const glm::mat4& MainGLWindow::getProjectionMatrix() const {
  return _projectionMatrix;
}

void MainGLWindow::setFps(float fps) {
  wchar_t buffer[24];
  swprintf(buffer, 24, L"%.2f FPS", fps);
  //_fpsComponent->setText(buffer);
}

MainGLWindow* MainGLWindow::getSingletonPtr() {
  return _singleton;
}

MainGLWindow& MainGLWindow::getSingleton() {
  assert(_singleton);
  return *_singleton;
}
