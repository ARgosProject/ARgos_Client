#include "DrawImageSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string DrawImageSF::name = "Image_";
  unsigned long long DrawImageSF::counter = 0;

  DrawImageSF::DrawImageSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawImageSF::execute(const std::vector<std::string>& args) {
    _graphicComponentsManager.createImageFromFile(name + std::to_string(counter),
                                                  args[0],
                                                  glm::vec3(getArgAsFloat(args[1]), getArgAsFloat(args[2]), getArgAsFloat(args[3])),
                                                  glm::vec2(getArgAsFloat(args[4]), getArgAsFloat(args[5]))
                                                  )->show(true);

    ++counter;
  }

}
