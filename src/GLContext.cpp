#include <iostream>
#include <cassert>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLContext.h"
#include "GraphicComponent.h"
#include "LineComponent.h"
#include "TextComponent.h"
#include "RectangleComponent.h"
#include "CircleComponent.h"
#include "ImageComponent.h"
#include "TextureRender.h"
#include "VideoStreamComponent.h"
#include "Log.h"

GLContext::GLContext(EGLconfig* config)
  : EGLWindow(config), _frame(NULL), _projectionMatrix(glm::mat4(1.0f)) {

}

GLContext::~GLContext() {
  for(GraphicComponent* gc : _gc) {
    delete gc;
  }
}

void GLContext::start() {
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

  makeCorners(2.0f, 1.0f, 0.0f, 0.0f);
  makeAxis(2.0f);

  TextComponent* text = new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32);
  text->setProjectionMatrix(_projectionMatrix);
  text->setScale(glm::vec3(0.05, 0.05, 0.05));
  text->setText(L"Holaaa asdasd asd", 1, 0, 0);
  _gc.push_back(text);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  std::cout << "GLContext initialized" << std::endl;
}

void GLContext::makeAxis(GLfloat axis_length) {
  // Lines
  LineComponent* x_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0));
  x_line->setColor(1, 0, 0, 1);
  x_line->setProjectionMatrix(_projectionMatrix);
  LineComponent* y_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0));
  y_line->setColor(0, 1, 0, 1);
  y_line->setProjectionMatrix(_projectionMatrix);
  LineComponent* z_line = new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, 0, axis_length));
  z_line->setColor(0, 0, 1, 1);
  z_line->setProjectionMatrix(_projectionMatrix);

  _gc.push_back(x_line);
  _gc.push_back(y_line);
  _gc.push_back(z_line);
}

void GLContext::makeCorners(GLfloat axis_length, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  // Corners
  LineComponent* corners[8] = {
    // Top-Left
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0)),   // 0
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -axis_length, 0)),  // 1

    // Top-Right
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-axis_length, 0, 0)),  // 2
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -axis_length, 0)),  // 3

    // Bot-Left
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0)),   // 4
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0)),   // 5

    // Bot-Right
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-axis_length, 0, 0)),  // 6
    new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0))    // 7
  };

  // Positions
  float width = 21.0f;
  float height = 29.7f;
  glm::vec3 topLeftCorner(-width / 2.0f, height / 2.0f, 0.0f);
  glm::vec3 topRightCorner(width / 2.0f, height / 2.0f, 0.0f);
  glm::vec3 botLeftCorner(-width / 2.0f, -height / 2.0f, 0);
  glm::vec3 botRightCorner(width / 2.0f, -height / 2.0f, 0);

  corners[0]->setPosition(topLeftCorner);
  corners[1]->setPosition(topLeftCorner);
  corners[2]->setPosition(topRightCorner);
  corners[3]->setPosition(topRightCorner);
  corners[4]->setPosition(botLeftCorner);
  corners[5]->setPosition(botLeftCorner);
  corners[6]->setPosition(botRightCorner);
  corners[7]->setPosition(botRightCorner);

  for(int i = 0; i < 8; ++i) {
    corners[i]->setColor(r, g, b, a);
    corners[i]->setProjectionMatrix(_projectionMatrix);
    _gc.push_back(corners[i]);
  }
}

void GLContext::update(paper_t& paper) {
  std::cout << "- id: " << paper.id << std::endl;
  Logger::Log::matrix(paper.modelview_matrix);

  size_t gc_length = _gc.size();
  for(size_t i = 1; i < gc_length; ++i) {
    _gc[i]->setModelViewMatrix(glm::make_mat4(paper.modelview_matrix));
  }
}

void GLContext::render() {
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

  // To update we need to swap the buffers
  swapBuffers();
}

void GLContext::setFrame(cv::Mat* currentFrame) {
  _frame = currentFrame;
}

cv::Mat& GLContext::getFrame() {
  return *_frame;
}

void GLContext::setProjectionMatrix(glm::mat4 projectionMatrix) {
  _projectionMatrix = projectionMatrix;
}

const glm::mat4& GLContext::getProjectionMatrix() const {
  return _projectionMatrix;
}

void GLContext::setFps(float fps) {
  wchar_t buffer[24];
  swprintf(buffer, 24, L"%.2f FPS", fps);
  //_fpsComponent->setText(buffer);
}
