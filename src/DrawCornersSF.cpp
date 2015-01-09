#include "DrawCornersSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string DrawCornersSF::name = "Corners_";
  unsigned long long DrawCornersSF::counter = 0;

  DrawCornersSF::DrawCornersSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawCornersSF::execute(const std::vector<std::string>& args) {
    _graphicComponentsManager.createCorners(name + std::to_string(counter),
                                            getArgAsFloat(args[0]),
                                            getArgAsFloat(args[1]),
                                            glm::vec4(getArgAsFloat(args[2]), getArgAsFloat(args[3]), getArgAsFloat(args[4]), 1.0f),
                                            glm::vec2(getArgAsFloat(args[5]), getArgAsFloat(args[6]))
                                            )->show(true);

    ++counter;
  }

}
