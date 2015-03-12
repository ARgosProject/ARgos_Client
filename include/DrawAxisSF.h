#ifndef DRAWAXISSF_H
#define DRAWAXISSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawAxisSF : public ScriptFunction {
  public:
    DrawAxisSF();

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
  };

}

#endif
