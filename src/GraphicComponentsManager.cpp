#include "GraphicComponentsManager.h"
#include "GraphicComponent.h"
#include "RenderToTextureComponent.h"
#include "RectangleComponent.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "VideoStreamComponent.h"
#include "Log.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace argosClient {

  GraphicComponentsManager::GraphicComponentsManager()
    : _projectionMatrix(glm::mat4(1.0f)) {

  }

  GraphicComponentsManager::~GraphicComponentsManager() {
    Log::info("Releasing all graphic components...");
    _graphicComponents.clear();
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::getGraphicComponent(const std::string& name) {
    if(_graphicComponents.find(name) == _graphicComponents.end()) {
      Log::error("Could not get the graphic component named '" + name + "'");
      return nullptr;
    }

    return _graphicComponents[name];
  }

  void GraphicComponentsManager::removeGraphicComponent(const std::string& name) {
    auto it = _graphicComponents.find(name);
    _graphicComponents.erase(it);
  }

  void GraphicComponentsManager::showGraphicComponent(const std::string& name, bool show) {
    if(_graphicComponents.find(name) == _graphicComponents.end()) {
      Log::error("Could not show the graphic component named '" + name + "'");
    }

    _graphicComponents[name]->show(show);
  }

  void GraphicComponentsManager::update(const glm::mat4& modelViewMatrix) {
    for(auto& gc : _graphicComponents) {
      gc.second->setModelViewMatrix(modelViewMatrix);
    }
  }

  void GraphicComponentsManager::setProjectionMatrix(const glm::mat4& projectionMatrix) {
    _projectionMatrix = projectionMatrix;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createCorners(const std::string& name, GLfloat length, GLfloat width,
                                                                                        const glm::vec4& colour) {
    // TODO
    return nullptr;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createAxis(const std::string& name, GLfloat axis_length,
                                                                                     const glm::vec3& pos) {
    // TODO
    return nullptr;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createVideoStream(const std::string& name, const std::string& bg_file,
                                                                                            GLfloat width, GLfloat height, int port) {
    float scaleFactor = 0.008f;
    std::shared_ptr<RenderToTextureComponent> rtt = std::make_shared<RenderToTextureComponent>(width, height);
    rtt->setScale(glm::vec3(-scaleFactor, scaleFactor, scaleFactor));
    rtt->setProjectionMatrix(_projectionMatrix);

    ImageComponent* bg = new ImageComponent("media/videoconference.jpg", width, height);
    bg->setProjectionMatrix(_projectionMatrix);
    rtt->addGraphicComponent(bg);

    VideoStreamComponent* videoStream = new VideoStreamComponent(height / 1.77, width / 1.77);
    videoStream->startReceivingVideo(port);
    videoStream->setPosition(glm::vec3(0.0f, 5.6f, 0.0f));
    videoStream->setScale(glm::vec3(1.055f, 0.85f, 1.0f));
    rtt->addGraphicComponent(videoStream);

    rtt->show(false);
    _graphicComponents[name] = rtt;

    return rtt;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createTextPanel(const std::string& name, const glm::vec4& colour,
                                                                                          const std::string& text) {
    // TODO
    return nullptr;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createHighlight(const std::string& name, const glm::vec4& colour,
                                                                                          const glm::vec3& pos, const glm::vec3& scale) {
    // TODO
    return nullptr;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createButton(const std::string& name, const glm::vec4& colour,
                                                                                       const std::string& text) {
    // TODO
    return nullptr;
  }

  GraphicComponentsManager::GraphicComponentPtr GraphicComponentsManager::createFactureHint(const std::string& name, const glm::vec2& size,
                                                                                            const glm::vec4& colour, const std::wstring& title,
                                                                                            const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks) {
    float scaleFactor = 0.008f;
    std::shared_ptr<RenderToTextureComponent> rtt = std::make_shared<RenderToTextureComponent>(size.x, size.y);
    rtt->setScale(glm::vec3(-scaleFactor, scaleFactor, scaleFactor));
    rtt->setProjectionMatrix(_projectionMatrix);

    RectangleComponent* bg = new RectangleComponent(size.x, size.y);
    bg->setColor(colour.r, colour.g, colour.b, colour.a);
    rtt->addGraphicComponent(bg);

    TextComponent* tcTitle = new TextComponent("media/ProximaNova-Bold.ttf", 72);
    tcTitle->setScale(glm::vec3(1.0f, -1.0f, 1.0f));
    tcTitle->setPosition(glm::vec3(50.0f, 50.0f, 0.0f));
    tcTitle->setText(title);
    rtt->addGraphicComponent(tcTitle);

    for(auto& block : textBlocks) {
      TextComponent* textComponent = new TextComponent("media/ProximaNova-Bold.ttf", 54);
      textComponent->setScale(glm::vec3(1.0f, -1.0f, 1.0f));
      textComponent->setPosition(block.second);
      textComponent->setText(block.first);
      rtt->addGraphicComponent(textComponent);
    }

    rtt->show(false);

    _graphicComponents[name] = rtt;

    return rtt;
  }

}
