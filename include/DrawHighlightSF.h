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

    void execute(const std::vector<std::string>& args) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;

    static std::string name;
    static unsigned long long counter;
  };

}

#endif
