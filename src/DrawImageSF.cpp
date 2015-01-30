#include "DrawImageSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  unsigned long long DrawImageSF::counter = 0;

  DrawImageSF::DrawImageSF()
    : ScriptFunction("Image_", "DrawImageSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawImageSF::_execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createImageFromFile(_name + "_id:" + std::to_string(id) + "_num:" + std::to_string(counter),
                                                  args[0],
                                                  glm::vec3(getArgAsFloat(args[1]), getArgAsFloat(args[2]), getArgAsFloat(args[3])),
                                                  glm::vec2(getArgAsFloat(args[4]), getArgAsFloat(args[5]))
                                                  )->show(true);

    (counter > 9999)?counter = 0:++counter;
  }

}
