#include "InitVideostreamSF.h"
#include "GraphicComponentsManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  InitVideostreamSF::InitVideostreamSF()
    : ScriptFunction("Videostream_"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()) {

  }

  void InitVideostreamSF::execute(const std::vector<std::string>& args, int id) {
    _graphicComponentsManager.createVideostream(_name + std::to_string(id),
                                                args[0],
                                                glm::vec2(getArgAsFloat(args[1]), getArgAsFloat(args[2])),
                                                getArgAsInt(args[3])
                                                )->show(true);
  }

}
