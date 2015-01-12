#include "DrawHighlightSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawHighlightSF::DrawHighlightSF()
    : ScriptFunction("Highlight_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawHighlightSF::execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createHighlight(_name + std::to_string(id),
                                              glm::vec4(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2]), 1.0f),
                                              glm::vec3(getArgAsFloat(args[3]), getArgAsFloat(args[4]), getArgAsFloat(args[5])),
                                              glm::vec3(getArgAsFloat(args[6]), getArgAsFloat(args[7]), 1.0f)
                                              )->show(true);
  }

}
