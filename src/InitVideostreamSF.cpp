#include "InitVideostreamSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  std::string InitVideostreamSF::name = "Videostream_";
  unsigned long long InitVideostreamSF::counter = 0;

  InitVideostreamSF::InitVideostreamSF()
    : ScriptFunction(),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void InitVideostreamSF::execute(const std::vector<std::string>& args) {
    _graphicComponentsManager.createVideostream(name + std::to_string(counter),
                                                 args[0],
                                                 glm::vec2(getArgAsFloat(args[1]), getArgAsFloat(args[2])),
                                                 getArgAsInt(args[3])
                                                 )->show(true);

    ++counter;
  }

}
