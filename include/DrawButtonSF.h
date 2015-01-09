#ifndef DRAWBUTTONSF_H
#define DRAWBUTTONSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawButtonSF : public ScriptFunction {
  public:
    DrawButtonSF();

    void execute(const std::vector<std::string>& args) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;

    static std::string name;
    static unsigned long long counter;
  };

}

#endif
