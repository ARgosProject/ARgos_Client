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
      _gcManager(GraphicComponentsManager::getInstance()) {

  }

  GLContext::~GLContext() {
    GraphicComponentsManager::getInstance().destroy();
    AudioManager::getInstance().destroy();
    delete _fingerPoint;
    delete _projArea;
  }

  void GLContext::start() {
    // Audio dependencies
    AudioManager::getInstance().setSoundsPath("data/sounds/");
    AudioManager::getInstance().preloadAll();

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
    _fingerPoint = new RectangleComponent(1.0f, 1.0f);
    _fingerPoint->setProjectionMatrix(_projectionMatrix);
    _fingerPoint->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    _fingerPoint->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _fingerPoint->show(true);

    // Projection area
    _projArea = new ImageComponent("data/images/background.jpg", 1.0f, 1.0f);
    _projArea->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _projArea->noUpdate();
    _projArea->show(true);

    // Videostream
    _gcManager.createVideostream("Videostream", "videoconference.jpg", glm::vec2(10.5f, 14.85f), 9999);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Log::success("OpenGL ES 2.0 context initialized.");
  }

  bool GLContext::update(paper_t paper) {
    static int oldId = -2;

    glm::mat4 modelview_matrix = glm::make_mat4(paper.modelview_matrix);
    GraphicComponentsManager::getInstance().update(modelview_matrix);

    glm::vec4 transformed_point = _projectionMatrix * modelview_matrix * glm::vec4(glm::vec3(paper.x, paper.y, 0.0f), 1.0f);
    _fingerPoint->setPosition(glm::vec3(transformed_point));
    _fingerPoint->setModelViewMatrix(modelview_matrix);
    Log::info("Finger point: (" + std::to_string(transformed_point.x) + ", " + std::to_string(transformed_point.y) + ")");

    if(paper.id != oldId) {
      _gcManager.cleanForId(oldId);
    }

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

}
