#include "DrawAxisSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawAxisSF::DrawAxisSF()
    : ScriptFunction("Axis_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawAxisSF::execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createAxis(_name + std::to_string(id),
                                         getArgAsFloat(args[0]),
                                         getArgAsFloat(args[1]),
                                         glm::vec3(getArgAsFloat(args[2]), getArgAsFloat(args[3]), getArgAsFloat(args[4]))
                                         )->show(true);
  }

}
