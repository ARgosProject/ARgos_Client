#include "DrawTextPanelSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>
#include <iostream>

namespace argosClient {

  unsigned long long DrawTextPanelSF::counter = 0;

  DrawTextPanelSF::DrawTextPanelSF()
    : ScriptFunction("TextPanel_", "DrawTextPanelSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawTextPanelSF::_execute(const std::vector<std::string>& args, int id) {
    std::wstring text;
    text.assign(args[4].begin(), args[4].end());

    _graphicComponentsManager.createTextPanel(_name + std::to_string(id),
                                              glm::vec4(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2]), 1.0f),
                                              getArgAsInt(args[3]),
                                              text,
                                              glm::vec3(getArgAsFloat(args[5]), getArgAsFloat(args[6]), getArgAsFloat(args[7])),
                                              glm::vec2(getArgAsFloat(args[8]), getArgAsFloat(args[9]))
                                              )->show(true);

    (counter > 9999)?counter = 0:++counter;
  }

}
