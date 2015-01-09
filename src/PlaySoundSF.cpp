#include "PlaySoundSF.h"
#include "AudioManager.h"

namespace argosClient {

  PlaySoundSF::PlaySoundSF()
    : ScriptFunction(),
      _audioManager(AudioManager::getInstance()) {

  }

  void PlaySoundSF::execute(const std::vector<std::string>& args) {
    _audioManager.play(args[0], getArgAsInt(args[1]));
  }

}
