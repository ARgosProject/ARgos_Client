#ifndef PLAYSOUNDDELAYEDSF_H
#define PLAYSOUNDDELAYEDSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class AudioManager;

  class PlaySoundDelayedSF : public ScriptFunction {
  public:
    PlaySoundDelayedSF();

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    AudioManager& _audioManager;
  };

}

#endif
