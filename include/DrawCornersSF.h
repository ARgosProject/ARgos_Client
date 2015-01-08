#ifndef DRAWCORNERSSF_H
#define DRAWCORNERSSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawCornersSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
