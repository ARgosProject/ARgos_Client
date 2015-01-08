#ifndef PLAYSOUNDDELAYEDSF_H
#define PLAYSOUNDDELAYEDSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class PlaySoundDelayedSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
