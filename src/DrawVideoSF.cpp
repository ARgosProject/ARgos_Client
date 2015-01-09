#include "DrawVideoSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string DrawVideoSF::name = "Video_";
  unsigned long long DrawVideoSF::counter = 0;

  DrawVideoSF::DrawVideoSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void DrawVideoSF::execute(const std::vector<std::string>& args) {
    _graphicComponentsManager.createVideoFromFile(name + std::to_string(counter),
                                                  args[0],
                                                  glm::vec3(getArgAsFloat(args[1]), getArgAsFloat(args[2]), getArgAsFloat(args[3])),
                                                  glm::vec2(getArgAsFloat(args[4]), getArgAsFloat(args[5]))
                                                  )->show(true);

    ++counter;
  }

}
