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

namespace argosClient {

  GLContext::GLContext(EGLconfig* config)
    : EGLWindow(config), _projectionMatrix(glm::mat4(1.0f)) {

  }

  GLContext::~GLContext() {
    GraphicComponentsManager::getInstance().destroy();
    AudioManager::getInstance().destroy();
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
    GraphicComponentsManager& gcManager = GraphicComponentsManager::getInstance();
    gcManager.setProjectionMatrix(_projectionMatrix);
    gcManager.setImagesPath("data/images/");
    gcManager.setVideosPath("data/videos/");
    gcManager.setFontsPath("data/fonts/");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Log::success("OpenGL ES 2.0 context initialized.");
  }

  bool GLContext::update(paper_t& paper) {
    glm::mat4 modelview_matrix = glm::make_mat4(paper.modelview_matrix);
    GraphicComponentsManager::getInstance().update(modelview_matrix);

    int sentences = paper.cfds.size();
    for(int i = 0; i < sentences; ++i) {
      _handlers[paper.cfds[i].id]->execute(paper.cfds[i].args, paper.id);
    }

    return true;
  }

  void GLContext::render() {
    // Clears the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    // Render all objects
    GraphicComponentsManager::getInstance().renderAll();

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
