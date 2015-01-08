#ifndef DRAWAXISSF_H
#define DRAWAXISSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawAxisSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
