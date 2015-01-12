#include "DrawButtonSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawButtonSF::DrawButtonSF()
    : ScriptFunction("Button_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawButtonSF::execute(const std::vector<std::string>& args, int id) {
    std::wstring text;
    text.assign(args[3].begin(), args[3].end());

    _graphicComponentsManager.createButton(_name + std::to_string(id),
                                           glm::vec4(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2]), 1.0f),
                                           text,
                                           glm::vec3(getArgAsFloat(args[4]), getArgAsFloat(args[5]), getArgAsFloat(args[6]))
                                           )->show(true);
  }

}
