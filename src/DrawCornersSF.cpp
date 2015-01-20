#include "DrawCornersSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawCornersSF::DrawCornersSF()
    : ScriptFunction("Corners_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawCornersSF::execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createCorners(_name + std::to_string(id),
                                            getArgAsFloat(args[0]),
                                            getArgAsFloat(args[1]),
                                            glm::vec4(getArgAsFloat(args[2]), getArgAsFloat(args[3]), getArgAsFloat(args[4]), 1.0f),
                                            glm::vec2(getArgAsFloat(args[5]), getArgAsFloat(args[6]))
                                            )->show(true);
  }

}
