#ifndef DRAWTEXTPANELSF_H
#define DRAWTEXTPANELSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawTextPanelSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
