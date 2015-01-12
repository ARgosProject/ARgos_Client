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

    void execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
  };

}

#endif
