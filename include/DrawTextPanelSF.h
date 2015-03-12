#ifndef DRAWTEXTPANELSF_H
#define DRAWTEXTPANELSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawTextPanelSF : public ScriptFunction {
  public:
    DrawTextPanelSF();

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
    static unsigned long long counter;
  };

}

#endif
