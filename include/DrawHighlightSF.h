#ifndef DRAWHIGHLIGHTSF_H
#define DRAWHIGHLIGHTSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class DrawHighlightSF : public ScriptFunction {
  public:
    void execute(const std::vector<std::string>& args) override;
  };

}

#endif
