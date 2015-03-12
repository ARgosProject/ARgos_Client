#include "PlaySoundSF.h"
#include "AudioManager.h"

namespace argosClient {

  PlaySoundSF::PlaySoundSF()
    : ScriptFunction("Sound", "PlaySoundSF"),
      _audioManager(AudioManager::getInstance()) {

  }

  void PlaySoundSF::_execute(const std::vector<std::string>& args, int id) {
    Log::info("Playing sound: " + args[0]);
    _audioManager.stop();
    _audioManager.play(args[0], getArgAsInt(args[1]));
  }

}
