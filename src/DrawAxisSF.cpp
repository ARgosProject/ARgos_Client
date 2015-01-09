#include "DrawAxisSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string DrawAxisSF::name = "Axis_";
  unsigned long long DrawAxisSF::counter = 0;

  DrawAxisSF::DrawAxisSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawAxisSF::execute(const std::vector<std::string>& args) {
    _graphicComponentsManager.createAxis(name + std::to_string(counter),
                                         getArgAsFloat(args[0]),
                                         getArgAsFloat(args[1]),
                                         glm::vec3(getArgAsFloat(args[2]), getArgAsFloat(args[3]), getArgAsFloat(args[4]))
                                         )->show(true);

    ++counter;
  }

}
