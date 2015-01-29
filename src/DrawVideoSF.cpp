#include "DrawVideoSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  DrawVideoSF::DrawVideoSF()
    : ScriptFunction("Video_", "DrawVideoSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawVideoSF::_execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createVideoFromFile(_name + std::to_string(id),
                                                  args[0],
                                                  glm::vec3(getArgAsFloat(args[1]), getArgAsFloat(args[2]), getArgAsFloat(args[3])),
                                                  glm::vec2(getArgAsFloat(args[4]), getArgAsFloat(args[5]))
                                                  )->show(true);
  }

}
