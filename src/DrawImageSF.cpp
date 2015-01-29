#include "DrawImageSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawImageSF::DrawImageSF()
    : ScriptFunction("Image_", "DrawImageSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawImageSF::_execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createImageFromFile(_name + std::to_string(id),
                                                  args[0],
                                                  glm::vec3(getArgAsFloat(args[1]), getArgAsFloat(args[2]), getArgAsFloat(args[3])),
                                                  glm::vec2(getArgAsFloat(args[4]), getArgAsFloat(args[5]))
                                                  )->show(true);
  }

}
