#include "DrawFactureHintSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawFactureHintSF::DrawFactureHintSF()
    : ScriptFunction("FactureHint_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawFactureHintSF::execute(const std::vector<std::string>& args, int id) {
    std::wstring title, block1, block2;
    title.assign(args[8].begin(), args[8].end());
    block1.assign(args[9].begin(), args[9].end());
    block2.assign(args[10].begin(), args[10].end());

    _graphicComponentsManager.createFactureHint(_name + std::to_string(id),
                                                glm::vec3(getArgAsFloat(args[0]), getArgAsFloat(args[1]), getArgAsFloat(args[2])),
                                                glm::vec2(getArgAsFloat(args[3]), getArgAsFloat(args[4])),
                                                glm::vec4(getArgAsFloat(args[5]), getArgAsFloat(args[6]), getArgAsFloat(args[7]), 1.0f),
                                                title,
                                                {
                                                  std::make_pair(block1, glm::vec3(100.0f, 50.0f, 0.0f)),
                                                  std::make_pair(block2, glm::vec3(200.0f, 50.0f, 0.0f))
                                                }
                                                )->show(true);
  }

}
