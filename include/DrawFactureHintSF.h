#ifndef DRAWFACTUREHINTSF_H
#define DRAWFACTUREHINTSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawFactureHintSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
