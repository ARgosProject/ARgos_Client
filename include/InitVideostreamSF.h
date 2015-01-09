#ifndef INITVIDEOSTREAMSF_H
#define INITVIDEOSTREAMSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class InitVideostreamSF : public ScriptFunction {
  public:
    InitVideostreamSF();

    void execute(const std::vector<std::string>& args) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;

    static std::string name;
    static unsigned long long counter;
  };

}

#endif
