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
#include "RenderToTextureComponent.h"
#include "VideoStreamComponent.h"
#include "AudioManager.h"
#include "Log.h"

namespace argosClient {

  GLContext::GLContext(EGLconfig* config)
    : EGLWindow(config), _frame(NULL), _projectionMatrix(glm::mat4(1.0f)) {

  }

  GLContext::~GLContext() {
    for(GraphicComponent* gc : _gcList) {
      delete gc;
    }
  }

  void GLContext::start() {
    // Paper dimensions
    float width = 21.0f / 2.0f;
    float height = 29.7f / 2.0f;

    // Audio preloading
    AudioManager::getInstance().preload("Ayuda:Videoconferencia", "media/ayudaVideoConferencia.wav");

    // Background
    ImageComponent* bg1 = new ImageComponent("background.jpg", 1.0, 1.0);
    _gcMap["Background1"] = bg1;
    _gcList.push_back(bg1);

    // Camera frame
    //ImageComponent* im = new ImageComponent(1.0, 1.0);
    //im->setProjectionMatrix(_projectionMatrix);
    //_gc.push_back(im);

    // Video stream background
    ImageComponent* bg2 = new ImageComponent("videoconference.jpg", width, height);
    bg2->setProjectionMatrix(_projectionMatrix);
    bg2->show(false);
    _gcMap["Background2"] = bg2;
    _gcList.push_back(bg2);

    // Video stream
    VideoStreamComponent* videoStream = new VideoStreamComponent(width / 4.0, height / 3.0);
    videoStream->startReceivingVideo(9999);
    videoStream->setProjectionMatrix(_projectionMatrix);
    _gcMap["Videostream"] = videoStream;
    _gcList.push_back(videoStream);

    // Sample text
    std::vector<GraphicComponent*> textLines = makeText(L"", 1.0f, 1.0f, 1.0f);
    _gcMap["Line1"] = textLines[0];
    _gcMap["Line2"] = textLines[1];
    _gcMap["Line3"] = textLines[2];
    _gcList.insert(_gcList.end(), textLines.begin(), textLines.end());

    // Corners
    std::vector<GraphicComponent*> corners = makeCorners(1.0f, 1.0f, 1.0f, 1.0f);
    _gcList.insert(_gcList.end(), corners.begin(), corners.end());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Log::success("Contexto OpenGL ES 2.0 inicializado");
  }

  std::vector<GraphicComponent*> GLContext::makeAxis(GLfloat axis_length) {
    // Lines
    std::vector<GraphicComponent*> lines = {
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(axis_length, 0, 0)),
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, axis_length, 0)),
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, 0, axis_length))
    };

    lines[0]->setColor(1, 0, 0, 1);
    lines[0]->setProjectionMatrix(_projectionMatrix);

    lines[1]->setColor(0, 1, 0, 1);
    lines[1]->setProjectionMatrix(_projectionMatrix);

    lines[2]->setColor(0, 0, 1, 1);
    lines[2]->setProjectionMatrix(_projectionMatrix);

    return lines;
  }

  std::vector<GraphicComponent*> GLContext::makeCorners(GLfloat axis_length, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    // Corners
    std::vector<GraphicComponent*> corners = {
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
    //float width = 14.8f;
    //float height = 21.0f;
    glm::vec3 topLeftCorner(-width / 2.0f, height / 2.0f, 0.0f);
    glm::vec3 topRightCorner(width / 2.0f, height / 2.0f, 0.0f);
    glm::vec3 botLeftCorner(-width / 2.0f, -height / 2.0f, 0.0f);
    glm::vec3 botRightCorner(width / 2.0f, -height / 2.0f, 0.0f);

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
    }

    return corners;
  }

  std::vector<GraphicComponent*> GLContext::makeText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    float width = 21.0f / 2.0f;
    float height = 29.7f / 2.0f;

    // 1.5f, 24.0f
    float offsetX = 1.7;
    float offsetY = 6.9f;
    float scaleFactor = 0.008f;
    float x = (-width + offsetX) / scaleFactor; // -600
    float y = (-height + offsetY) / scaleFactor; // -700

    std::vector<GraphicComponent*> textLines = {
      new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 128),
      new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 128),
      new TextComponent("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 128)
    };

    textLines[0]->setProjectionMatrix(_projectionMatrix);
    textLines[0]->setScale(glm::vec3(-scaleFactor, scaleFactor, scaleFactor));
    textLines[0]->setPosition(glm::vec3(x, y, 0.0f));
    static_cast<TextComponent*>(textLines[0])->setText(strtext, r, g, b, a);

    textLines[1]->setProjectionMatrix(_projectionMatrix);
    textLines[1]->setScale(glm::vec3(-scaleFactor, scaleFactor, scaleFactor));
    textLines[1]->setPosition(glm::vec3(x, y - (1.024f / scaleFactor), 0.0f));
    static_cast<TextComponent*>(textLines[1])->setText(strtext, r, g, b, a);

    textLines[2]->setProjectionMatrix(_projectionMatrix);
    textLines[2]->setScale(glm::vec3(-scaleFactor, scaleFactor, scaleFactor));
    textLines[2]->setPosition(glm::vec3(x, y - (2.048f / scaleFactor), 0.0f));
    static_cast<TextComponent*>(textLines[2])->setText(strtext, r, g, b, a);

    return textLines;
  }

  void GLContext::update(cv::Mat& currentFrame, paper_t& paper) {
    size_t gc_length = _gcList.size();
    for(size_t i = 1; i < gc_length; ++i) {
      _gcList[i]->setModelViewMatrix(glm::make_mat4(paper.modelview_matrix));
    }
    _frame = &currentFrame;
    fetchPaperId(paper.id);
  }

  void GLContext::fetchPaperId(int id) {
    static bool runOnce[4] = {false, false, false, false};

    switch(id) {
    case 1:
      runOnce[1] = false; runOnce[2] = false; runOnce[3] = false;

      _gcMap["Background2"]->show(false);
      static_cast<TextComponent*>(_gcMap["Line1"])->setText(L"Neobiz:");
      static_cast<TextComponent*>(_gcMap["Line2"])->setText(L"Archivar en carpeta");
      static_cast<TextComponent*>(_gcMap["Line3"])->setText(L"AZUL");

      if(!runOnce[0]) {
        runOnce[0] = true;
      }
      break;
    case 2:
      runOnce[0] = false; runOnce[2] = false; runOnce[3] = false;

      _gcMap["Background2"]->show(false);
      static_cast<TextComponent*>(_gcMap["Line1"])->setText(L"Sinovo:");
      static_cast<TextComponent*>(_gcMap["Line2"])->setText(L"Gire factura para AYUDA");
      static_cast<TextComponent*>(_gcMap["Line3"])->setText(L"");

      if(!runOnce[1]) {
        runOnce[1] = true;
        AudioManager::getInstance().play("Ayuda:Videoconferencia");
      }
      break;
    case 3:
      runOnce[0] = false; runOnce[1] = false; runOnce[3] = false;

      _gcMap["Background2"]->show(false);
      static_cast<TextComponent*>(_gcMap["Line1"])->setText(L"Active:");
      static_cast<TextComponent*>(_gcMap["Line2"])->setText(L"Comprobar IMPORTES");
      static_cast<TextComponent*>(_gcMap["Line3"])->setText(L"");

      if(!runOnce[2]) {
        runOnce[2] = true;
      }
      break;
    case 999:
      runOnce[0] = false; runOnce[1] = false; runOnce[2] = false;

      _gcMap["Background2"]->show(true);
      _gcMap["Line1"]->show(false);
      _gcMap["Line2"]->show(false);
      _gcMap["Line3"]->show(false);

      if(!runOnce[3]) {
        runOnce[3] = true;
      }
      break;
    default:
      runOnce[0] = false; runOnce[1] = false; runOnce[2] = false; runOnce[3] = false;
      _gcMap["Background2"]->show(false);
      _gcMap["Line1"]->show(false);
      _gcMap["Line2"]->show(false);
      _gcMap["Line3"]->show(false);
      break;
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
    size_t gc_length = _gcList.size();
    for(size_t i = 0; i < gc_length; ++i) {
      _gcList[i]->render();
    }

    // To update we need to swap the buffers
    swapBuffers();
  }

  void GLContext::setFrame(cv::Mat& currentFrame) {
    _frame = &currentFrame;
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

}
