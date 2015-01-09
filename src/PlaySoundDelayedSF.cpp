#include "PlaySoundDelayedSF.h"
#include "AudioManager.h"

namespace argosClient {

  PlaySoundDelayedSF::PlaySoundDelayedSF()
    : ScriptFunction(),
      _audioManager(AudioManager::getInstance()) {

  }

  void PlaySoundDelayedSF::execute(const std::vector<std::string>& args) {
    AudioManager::getInstance().play(args[0], getArgAsInt(args[1]));
  }

}
