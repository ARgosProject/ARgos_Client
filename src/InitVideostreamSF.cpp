#include "InitVideostreamSF.h"
#include "GraphicComponentsManager.h"
#include "GLContext.h"
#include "AudioManager.h"

#include <glm/glm.hpp>

namespace argosClient {

  InitVideostreamSF::InitVideostreamSF()
    : ScriptFunction("Videostream_", "InitVideoStreamSF"),
      _graphicComponentsManager(GraphicComponentsManager::getInstance()),
      _audioManager(AudioManager::getInstance()),
      _glContext(GLContext::getInstance()) {

  }

  void InitVideostreamSF::_execute(const std::vector<std::string>& args, int id) {
    /*_graphicComponentsManager.createVideostream(_name + std::to_string(id),
                                                args[0],
                                                glm::vec2(getArgAsFloat(args[1]), getArgAsFloat(args[2])),
                                                getArgAsInt(args[3])
                                                )->show(true);*/

    int isVideoStreaming = _glContext.isVideoStreaming();
    int isClothes = _glContext.isClothes();

    if(_glContext.isVideoStreaming()) {
      _glContext.setIsVideoStreaming(0);
      _graphicComponentsManager.showGCCollection("Videostream", true);
      _audioManager.stop();
      _audioManager.play("iniciando_videoconferencia.wav", 0);
    }
    else {
      _graphicComponentsManager.showGCCollection("Videostream", false);
    }

    switch(_glContext.isClothes()) {
    case 1:
      _graphicComponentsManager.createImageFromFile("Textil_id:999_num:0", "trousers.jpg",
                                                   glm::vec3(0.0f), glm::vec2(10.5f, 14.85f))->show(true);
      _glContext.setIsClothes(0);
      break;
    case 2:
      _graphicComponentsManager.createImageFromFile("Textil_id:999_num:0", "frontShirt.jpg",
                                                   glm::vec3(0.0f), glm::vec2(10.5f, 14.85f))->show(true);
      _glContext.setIsClothes(0);
      break;
    case 3:
      _graphicComponentsManager.createImageFromFile("Textil_id:999_num:0", "backShirt.jpg",
                                                   glm::vec3(0.0f), glm::vec2(10.5f, 14.85f))->show(true);
      _glContext.setIsClothes(0);
      break;
    default:
      break;
    }


  }

}
