#ifndef DRAWCORNERSSF_H
#define DRAWCORNERSSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawCornersSF : public ScriptFunction {
  public:
    DrawCornersSF();

    void execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
  };

}

#endif
