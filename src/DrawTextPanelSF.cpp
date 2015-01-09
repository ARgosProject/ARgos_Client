#include "DrawTextPanelSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string DrawTextPanelSF::name = "TextPanel_";
  unsigned long long DrawTextPanelSF::counter = 0;

  DrawTextPanelSF::DrawTextPanelSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawTextPanelSF::execute(const std::vector<std::string>& args) {
    std::wstring text;
    text.assign(args[3].begin(), args[3].end());

    _graphicComponentsManager.createTextPanel(name + std::to_string(counter),
                                              glm::vec4(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2]), 1.0f),
                                              text,
                                              glm::vec3(getArgAsFloat(args[4]), getArgAsFloat(args[5]), getArgAsFloat(args[6]))
                                              )->show(true);

    ++counter;
  }

}
