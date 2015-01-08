#ifndef PLAYSOUNDSF_H
#define PLAYSOUNDSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class PlaySoundSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
