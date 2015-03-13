#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLContext.h"
#include "GraphicComponentsManager.h"

#include "TaskDelegation.h"
#include "Log.h"
#include "AudioManager.h"

#include "DrawImageSF.h"
#include "DrawVideoSF.h"
#include "DrawCornersSF.h"
#include "DrawAxisSF.h"
#include "InitVideostreamSF.h"
#include "DrawTextPanelSF.h"
#include "DrawHighlightSF.h"
#include "DrawButtonSF.h"
#include "DrawFactureHintSF.h"
#include "PlaySoundSF.h"
#include "PlaySoundDelayedSF.h"

#include "ImageComponent.h"
#include "RectangleComponent.h"

namespace argosClient {

  GLContext::GLContext(EGLconfig* config)
    : EGLWindow(config), _projectionMatrix(glm::mat4(1.0f)),
      _gcManager(GraphicComponentsManager::getInstance()),
      _audioManager(AudioManager::getInstance()),
      _isVideostream(0), _isVideo1(0), _isVideo2(0), _isClothes(0) {

  }

  GLContext::~GLContext() {
    GraphicComponentsManager::getInstance().destroy();
    AudioManager::getInstance().destroy();

    for(auto& pair : _handlers) {
      delete pair.second;
    }

    delete _fingerPoint;
    delete _projArea;

    for(int i = 0; i < 2; ++i) {
      delete _videoButtonInv[i];
      delete _handButtonInv[i];
      delete _helpButtonInv[i];
    }
    //delete _helpButtonInv[2];
  }

  void GLContext::start() {
    // Audio dependencies
    _audioManager.setSoundsPath("data/sounds/");
    _audioManager.getInstance().preloadAll();

    // Handlers
    _handlers[CallingFunctionType::DRAW_IMAGE] = new DrawImageSF;
    _handlers[CallingFunctionType::DRAW_VIDEO] = new DrawVideoSF;
    _handlers[CallingFunctionType::DRAW_CORNERS] = new DrawCornersSF;
    _handlers[CallingFunctionType::DRAW_AXIS] = new DrawAxisSF;
    _handlers[CallingFunctionType::INIT_VIDEO_STREAM] = new InitVideostreamSF;
    _handlers[CallingFunctionType::DRAW_TEXT_PANEL] = new DrawTextPanelSF;
    _handlers[CallingFunctionType::DRAW_HIGHLIGHT] = new DrawHighlightSF;
    _handlers[CallingFunctionType::DRAW_BUTTON] = new DrawButtonSF;
    _handlers[CallingFunctionType::DRAW_FACTURE_HINT] = new DrawFactureHintSF;
    _handlers[CallingFunctionType::PLAY_SOUND] = new PlaySoundSF;
    _handlers[CallingFunctionType::PLAY_SOUND_DELAYED] = new PlaySoundDelayedSF;

    // Graphic dependencies
    _gcManager.setProjectionMatrix(_projectionMatrix);
    _gcManager.setImagesPath("data/images/");
    _gcManager.setVideosPath("data/videos/");
    _gcManager.setFontsPath("data/fonts/");

    // Finger point
    _fingerPoint = new RectangleComponent(0.5f, 0.5f);
    _fingerPoint->setProjectionMatrix(_projectionMatrix);
    _fingerPoint->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    _fingerPoint->setPosition(glm::vec3(-6.69f, -3.072f, 0.0f));
    _fingerPoint->show(true);

    // Projection area
    _projArea = new ImageComponent("data/images/background.jpg", 1.0f, 1.0f);
    _projArea->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _projArea->noUpdate();
    _projArea->show(true);

    // Videostream
    _gcManager.createVideostream("Videostream", "videoconference.jpg", glm::vec2(10.5f, 14.85f), 9999);

    // Inverted buttons
    _videoButtonInv[0] = new ImageComponent("data/images/VideoButton_inv.jpg", -1.25f, 1.25f);
    _videoButtonInv[0]->setProjectionMatrix(_projectionMatrix);
    _videoButtonInv[0]->setPosition(glm::vec3(-9.00f, -2.00f, 0.00f));
    _videoButtonInv[0]->show(false);
    _videoButtonInv[1] = new ImageComponent("data/images/VideoButton_inv.jpg", -1.25f, 1.25f);
    _videoButtonInv[1]->setProjectionMatrix(_projectionMatrix);
    _videoButtonInv[1]->setPosition(glm::vec3(-9.00f, 3.75f, 0.00f));
    _videoButtonInv[1]->show(false);

    _handButtonInv[0] = new ImageComponent("data/images/HandButton_inv.jpg", -1.25f, 1.25f);
    _handButtonInv[0]->setProjectionMatrix(_projectionMatrix);
    _handButtonInv[0]->setPosition(glm::vec3(-9.00f, 4.00f, 0.00f));
    _handButtonInv[0]->show(false);
    _handButtonInv[1] = new ImageComponent("data/images/HandButton_inv.jpg", -1.25f, 1.25f);
    _handButtonInv[1]->setProjectionMatrix(_projectionMatrix);
    _handButtonInv[1]->setPosition(glm::vec3(-9.00f, -2.50f, 0.00f));
    _handButtonInv[1]->show(false);

    _helpButtonInv[0] = new ImageComponent("data/images/HelpButton_inv.jpg", -1.25f, 1.25f);
    _helpButtonInv[0]->setProjectionMatrix(_projectionMatrix);
    _helpButtonInv[0]->setPosition(glm::vec3(-9.00f, 3.50f, 0.00f));
    _helpButtonInv[0]->show(false);
    _helpButtonInv[1] = new ImageComponent("data/images/HelpButton_inv.jpg", -1.25f, 1.25f);
    _helpButtonInv[1]->setProjectionMatrix(_projectionMatrix);
    _helpButtonInv[1]->setPosition(glm::vec3(-9.00f, -2.65f, 0.00f));
    _helpButtonInv[1]->show(false);

    _helpButtonInv[2] = new ImageComponent("data/images/HelpButton_inv.jpg", -1.25f, 1.25f);
    _helpButtonInv[2]->setProjectionMatrix(_projectionMatrix);
    _helpButtonInv[2]->setPosition(glm::vec3(-9.00f, -8.85f, 0.00f));
    _helpButtonInv[2]->show(false);


    // PointsFlags
    for(int i = 0; i < 7; ++i) {
      _pointsFlags[i] = false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Log::success("OpenGL ES 2.0 context initialized.");
  }

  bool GLContext::update(paper_t paper) {
    static int oldId = -2;

    glm::mat4 modelview_matrix = glm::make_mat4(paper.modelview_matrix);
    GraphicComponentsManager::getInstance().update(modelview_matrix);

    glm::vec3 point(paper.x, paper.y, 0.0f);
    _fingerPoint->setModelMatrix(glm::mat4(1.0f));
    _fingerPoint->setPosition(point);
    _fingerPoint->setModelViewMatrix(modelview_matrix);
    Log::info("Finger point: (" + std::to_string(point.x) + ", " + std::to_string(point.y) + ")");

    // Operarios
    if(paper.id == 0) {
      for(int i = 0; i < 2; ++i) {
        _videoButtonInv[i]->setModelViewMatrix(modelview_matrix);
      }

      _isClothes = 0;

      if(isInRegion(point, glm::vec4(-9.00f, -2.00f, 2.50f, 2.50f)) && !_pointsFlags[0]) {
        _pointsFlags[0] = true;
        _videoButtonInv[0]->show(true);

        _audioManager.stop();
        _audioManager.play("success.wav", 0);
        _audioManager.play("iniciar_videoconferencia.wav", 0);

        _isVideostream = 1;
      }
      else {
        _pointsFlags[0] = false;
        _videoButtonInv[0]->show(false);
      }

      if(isInRegion(point, glm::vec4(-9.00f, 3.75f, 2.50f, 2.50f)) && !_pointsFlags[1]) {
        _pointsFlags[1] = true;
        _videoButtonInv[1]->show(true);

        _audioManager.stop();
        _audioManager.play("success.wav", 0);
        _audioManager.play("iniciar_videoconferencia.wav", 0);

        _isVideostream = 1;
      }
      else {
        _pointsFlags[1] = false;
        _videoButtonInv[1]->show(false);
      }
    }
    // Estampaciones
    else if(paper.id == 1) {
      for(int i = 0; i < 2; ++i) {
        _handButtonInv[i]->setModelViewMatrix(modelview_matrix);
      }

      if(isInRegion(point, glm::vec4(-9.00f, 4.00f, 2.50f, 2.50f)) && !_pointsFlags[2] && !_isVideo1) {
        _pointsFlags[2] = true;
        _handButtonInv[0]->show(true);
        _isVideo1 = 1;
        _isVideo2 = 0;

        _audioManager.play("success.wav", 0);
        _gcManager.removeGCCollection("Tampo_id:1_num:1");
        _gcManager.createVideoFromFile("Tampo_id:1_num:0", "Signos.avi",
                                       glm::vec3(-1.25f, 2.15f, 0.00f),
                                       glm::vec2(-3.50f, 2.00f))->show(true);
      }
      else {
        _pointsFlags[2] = false;
        _handButtonInv[0]->show(false);
      }

      if(isInRegion(point, glm::vec4(-9.00f, -2.50f, 2.50f, 2.50f)) && !_pointsFlags[3] && !_isVideo2) {
        _pointsFlags[3] = true;
        _handButtonInv[1]->show(true);
        _isVideo1 = 0;
        _isVideo2 = 1;

        _audioManager.play("success.wav", 0);
        _gcManager.removeGCCollection("Tampo_id:1_num:0");
        _gcManager.createVideoFromFile("Tampo_id:1_num:1", "Signos.avi",
                                       glm::vec3(-1.25f, -4.50f, 0.00f),
                                       glm::vec2(-3.50f, 2.00f))->show(true);
      }
      else {
        _pointsFlags[3] = false;
        _handButtonInv[1]->show(false);
      }
    }
    // Textil
    else if(paper.id == 2) {
      for(int i = 0; i < 2; ++i) {
        _helpButtonInv[i]->setModelViewMatrix(modelview_matrix);
      }

      _isVideostream = 0;

      if(isInRegion(point, glm::vec4(-9.00f, 3.50f, 2.50f, 2.50f)) && !_pointsFlags[4]) {
        _pointsFlags[4] = true;
        _helpButtonInv[0]->show(true);
        _isClothes = 1;
        _audioManager.play("success.wav", 0);
        _audioManager.play("estampacion.wav", 0);
      }
      else {
        _pointsFlags[4] = false;
        _helpButtonInv[0]->show(false);
      }

      if(isInRegion(point, glm::vec4(-9.00f, -2.65f, 2.50f, 2.50f)) && !_pointsFlags[5]) {
        _pointsFlags[5] = true;
        _helpButtonInv[1]->show(true);
        _isClothes = 2;
        _audioManager.play("success.wav", 0);
        _audioManager.play("estampacion.wav", 0);
      }
      else {
        _pointsFlags[5] = false;
        _helpButtonInv[1]->show(false);
      }

      if(isInRegion(point, glm::vec4(-9.00f, -8.85f, 2.50f, 2.50f)) && !_pointsFlags[6]) {
        _pointsFlags[6] = true;
        _helpButtonInv[2]->show(true);
        _isClothes = 3;
        _audioManager.play("success.wav", 0);
        _audioManager.play("estampacion.wav", 0);
      }
      else {
        _pointsFlags[6] = false;
        _helpButtonInv[2]->show(false);
      }

    }

    if(paper.id != oldId) {
      _isVideo1 = _isVideo2 = 0;
      for(int i = 0; i < 2; ++i) {
        _videoButtonInv[i]->show(false);
        _handButtonInv[i]->show(false);
        _helpButtonInv[i]->show(false);
      }
      _helpButtonInv[2]->show(false);
      _gcManager.cleanForId(oldId);
    }

    // Blank
    if(paper.id != 999) {
      _gcManager.showGCCollection("Videostream", false);
    }

    int sentences = paper.cfds.size();
    for(int i = 0; i < sentences; ++i) {
      _handlers[paper.cfds[i].id]->execute(paper.cfds[i].args, paper.id);
    }

    oldId = paper.id;

    return true;
  }

  void GLContext::render() {
    // Clears the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    // Render background
    _projArea->render();

    // Render all objects
    GraphicComponentsManager::getInstance().renderAll();
    for(int i = 0; i < 2; ++i) {
      _videoButtonInv[i]->render();
      _handButtonInv[i]->render();
      _helpButtonInv[i]->render();
    }
    _helpButtonInv[2]->render();

    _fingerPoint->render();

    // To update we need to swap the buffers
    swapBuffers();
  }

  void GLContext::setProjectionMatrix(glm::mat4 projectionMatrix) {
    _projectionMatrix = projectionMatrix;
  }

  const glm::mat4& GLContext::getProjectionMatrix() const {
    return _projectionMatrix;
  }

  bool GLContext::isInRegion(const glm::vec3& p, const glm::vec4& r) const {
    float rx = r.x;
    float ry = r.y;
    float rw = r.z;
    float rh = r.w;
    float px = p.x;
    float py = p.y;
    float m  = 1.0f;

    if((px > (rx - (m * rw))) && (px < (rx + (m * rw))) && (py > (ry - (m * rh))) && (py < (ry + (m * rh))))
      return true;

    return false;
  }

  void GLContext::setIsVideoStreaming(int isVideostream) {
    _isVideostream = isVideostream;
  }

  int GLContext::isVideoStreaming() const {
    return _isVideostream;
  }

  void GLContext::setIsClothes(int isClothes) {
    _isClothes = isClothes;
  }

  int GLContext::isClothes() const {
    return _isClothes;
  }

}
