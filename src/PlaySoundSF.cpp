#include "PlaySoundSF.h"
#include "AudioManager.h"

namespace argosClient {

  PlaySoundSF::PlaySoundSF()
    : ScriptFunction("Sound", "PlaySoundSF"),
      _audioManager(AudioManager::getInstance()),
      _lastSound("") {

  }

  void PlaySoundSF::_execute(const std::vector<std::string>& args, int id) {
    if((args[0].compare(_lastSound) != 0) || (_lastSound.compare("success.wav") == 0)) {
      _lastSound = args[0];

      Log::info("Playing sound: " + args[0]);
      _audioManager.stop();
      _audioManager.play(args[0], getArgAsInt(args[1]));
    }
  }

}
