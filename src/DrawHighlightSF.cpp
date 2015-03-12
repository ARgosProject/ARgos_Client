#include "DrawHighlightSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  unsigned long long DrawHighlightSF::counter = 0;

  DrawHighlightSF::DrawHighlightSF()
    : ScriptFunction("Highlight_", "DrawHighlightSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawHighlightSF::_execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createHighlight(_name + "_id:" + std::to_string(id) + "_num:" + std::to_string(counter),
                                              glm::vec4(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2]), 1.0f),
                                              glm::vec3(getArgAsFloat(args[3]), getArgAsFloat(args[4]), getArgAsFloat(args[5])),
                                              glm::vec3(getArgAsFloat(args[6]), getArgAsFloat(args[7]), 1.0f)
                                              )->show(true);

    (counter > 9999)?counter = 0:++counter;
  }

}
