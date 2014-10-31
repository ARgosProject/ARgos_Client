#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

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
    : EGLWindow(config), _state(State::FETCH_PAPERS), _frame(NULL), _projectionMatrix(glm::mat4(1.0f)) {

  }

  GLContext::~GLContext() {
    for(auto& gc : _graphicComponents) {
      delete gc.second;
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
    bg1->noUpdate(true);
    _graphicComponents["Background1"] = bg1;

    // Camera frame
    //ImageComponent* im = new ImageComponent(1.0, 1.0);
    //im->setProjectionMatrix(_projectionMatrix);
    //_gc.push_back(im);

    // Video stream background
    ImageComponent* bg2 = new ImageComponent("videoconference.jpg", width, height);
    bg2->setProjectionMatrix(_projectionMatrix);
    bg2->show(false);
    _graphicComponents["Background2"] = bg2;

    // Video stream
    VideoStreamComponent* videoStream = new VideoStreamComponent(height / 1.77, width / 1.77);
    videoStream->startReceivingVideo(9999);
    videoStream->setProjectionMatrix(_projectionMatrix);
    videoStream->setPosition(glm::vec3(0.0f, 5.6f, 0.0f));
    videoStream->setScale(glm::vec3(1.055f, 0.85f, 1.0f));
    _graphicComponents["Videostream"] = videoStream;

    // Sample text
    //std::vector<GraphicComponent*> textLines = makeText(L"", 1.0f, 1.0f, 1.0f);
    //_gcMap["Line1"] = textLines[0];
    //_gcMap["Line2"] = textLines[1];
    //_gcMap["Line3"] = textLines[2];
    //_gcList.insert(_gcList.end(), textLines.begin(), textLines.end());

    // Corners
    std::vector<GraphicComponent*> corners = makeCorners(1.0f, 1.0f, 1.0f, 1.0f);
    std::for_each(_graphicComponents.begin(), _graphicComponents.end(),
                  [&corners](const std::map<std::string, GraphicComponent*>::value_type& p) {
                    corners.push_back(p.second);
                  });

    // Facturas
    // -96.4 59.7
    std::vector<GraphicComponent*> factures = makeFactures();
    _graphicComponents["Active"] = factures[0];
    _graphicComponents["Sinovo"] = factures[1];
    _graphicComponents["Neobiz"] = factures[2];

    _state = State::FETCH_PAPERS;

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

  std::vector<GraphicComponent*> GLContext::makeCorners(GLfloat length, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    // Corners
    std::vector<GraphicComponent*> corners = {
      // Top-Left
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(length, 0, 0)),   // 0
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -length, 0)),  // 1

      // Top-Right
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-length, 0, 0)),  // 2
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, -length, 0)),  // 3

      // Bot-Left
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(length, 0, 0)),   // 4
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, length, 0)),   // 5

      // Bot-Right
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(-length, 0, 0)),  // 6
      new LineComponent(glm::vec3(0, 0, 0), glm::vec3(0, length, 0))    // 7
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

  std::vector<GraphicComponent*> GLContext::makeFactures() {
    float width = 5.7f;
    float height = 6.43f;//9.85f;

    std::vector<GraphicComponent*> factures = {
      new ImageComponent("active.jpg", width / 1.41, height / 1.41),
      new ImageComponent("sinovo.jpg", width / 1.41, height / 1.41),
      new ImageComponent("neobiz.jpg", width / 1.41, height / 1.41)
    };

    float x = 5.64f;
    float y = -8.67f;

    for(GraphicComponent* facture : factures) {
      facture->setProjectionMatrix(_projectionMatrix);
      facture->show(false);
      facture->setPosition(glm::vec3(x, y, 0.0f));
      facture->setScale(glm::vec3(-1.0f, 1.0f, 1.0f));
    }

    return factures;
  }

  std::vector<GraphicComponent*> GLContext::makeText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    float width = 21.0f / 2.0f;
    float height = 29.7f / 2.0f;

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

  void GLContext::fetchPaperId(int id) {
    static bool runOnce[4] = {false, false, false, false};

    //static_cast<TextComponent*>(_gcMap["Line1"])->setText(L"Neobiz:");
    //static_cast<TextComponent*>(_gcMap["Line2"])->setText(L"Archivar en carpeta");
    //static_cast<TextComponent*>(_gcMap["Line3"])->setText(L"AZUL");

    if((id >= 1) && (id <= 3)) {
      _graphicComponents["Background2"]->show(false);
    }

    switch(id) {
    case 1:
      runOnce[1] = false; runOnce[2] = false; runOnce[3] = false;

      _graphicComponents["Neobiz"]->show(true);
      _graphicComponents["Sinovo"]->show(false);
      _graphicComponents["Active"]->show(false);

      if(!runOnce[0]) {
        runOnce[0] = true;
      }
      break;
    case 2:
      runOnce[0] = false; runOnce[2] = false; runOnce[3] = false;

      _graphicComponents["Neobiz"]->show(false);
      _graphicComponents["Sinovo"]->show(true);
      _graphicComponents["Active"]->show(false);

      if(!runOnce[1]) {
        runOnce[1] = true;
        AudioManager::getInstance().play("Ayuda:Videoconferencia");
      }
      break;
    case 3:
      runOnce[0] = false; runOnce[1] = false; runOnce[3] = false;

      _graphicComponents["Neobiz"]->show(false);
      _graphicComponents["Sinovo"]->show(false);
      _graphicComponents["Active"]->show(true);

      if(!runOnce[2]) {
        runOnce[2] = true;
      }
      break;
    case 999:
      runOnce[0] = false; runOnce[1] = false; runOnce[2] = false;

      _graphicComponents["Background2"]->show(true);
      _graphicComponents["Neobiz"]->show(false);
      _graphicComponents["Sinovo"]->show(false);
      _graphicComponents["Active"]->show(false);

      if(!runOnce[3]) {
        runOnce[3] = true;
      }
      break;
    default:
      runOnce[0] = false; runOnce[1] = false; runOnce[2] = false; runOnce[3] = false;
      _graphicComponents["Neobiz"]->show(false);
      _graphicComponents["Sinovo"]->show(false);
      _graphicComponents["Active"]->show(false);
      break;
    }
  }

  bool GLContext::update(cv::Mat& currentFrame, paper_t& paper) {
    bool next = true;

    glm::mat4 modelview_matrix = glm::make_mat4(paper.modelview_matrix);
    for(auto& gc : _graphicComponents) {
      gc.second->setModelViewMatrix(modelview_matrix);
    }
    _frame = &currentFrame;

    switch(_state) {
    case State::INTRODUCTION:
      std::cout << "------------------> " << paper.id << std::endl;
      next = paper.id != 0;
      break;
    case State::FETCH_PAPERS:
      fetchPaperId(paper.id);
      break;
    }

    return next;
  }

  void GLContext::addGraphicComponent(const std::string& name, GraphicComponent* gc) {
    _graphicComponents[name] = gc;
  }

  GraphicComponent* GLContext::getGraphicComponent(const std::string& name) {
    return _graphicComponents[name];
  }

  void GLContext::removeGraphicComponent(const std::string& name) {
    auto it = _graphicComponents.find(name);
    _graphicComponents.erase(it);
    delete it->second;
  }

  void GLContext::setState(State state) {
    _state = state;
  }

  void GLContext::render() {
    // Clears the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    // Render the camera frame
    //((ImageComponent*)_gc[0])->loadImageFromMat(*_frame);
    //_gc[0]->render();
    //((ImageComponent*)_gc[0])->deleteTexture();

    // Render all objects
    for(auto& gc : _graphicComponents) {
      gc.second->render();
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
