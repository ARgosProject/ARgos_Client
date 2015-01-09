#ifndef DRAWFACTUREHINTSF_H
#define DRAWFACTUREHINTSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawFactureHintSF : public ScriptFunction {
  public:
    DrawFactureHintSF();

    void execute(const std::vector<std::string>& args) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;

    static std::string name;
    static unsigned long long counter;
  };

}

#endif
