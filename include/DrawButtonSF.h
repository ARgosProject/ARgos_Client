#ifndef DRAWBUTTONSF_H
#define DRAWBUTTONSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawButtonSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
