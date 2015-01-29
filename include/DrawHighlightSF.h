#ifndef DRAWHIGHLIGHTSF_H
#define DRAWHIGHLIGHTSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawHighlightSF : public ScriptFunction {
  public:
    DrawHighlightSF();

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
  };

}

#endif
