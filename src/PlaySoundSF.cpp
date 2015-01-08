#include "PlaySoundSF.h"
#include "AudioManager.h"

namespace argosClient {

  void PlaySoundSF::execute(const std::vector<std::string>& args) {
    AudioManager::getInstance().play(args[0], getArgAsInt(args[1]));
  }

}
