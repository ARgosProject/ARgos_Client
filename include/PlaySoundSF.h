#ifndef PLAYSOUNDSF_H
#define PLAYSOUNDSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class AudioManager;

  class PlaySoundSF : public ScriptFunction {
  public:
    PlaySoundSF();

    void execute(const std::vector<std::string>& args, int id) override;

  private:
    AudioManager& _audioManager;
  };

}

#endif
