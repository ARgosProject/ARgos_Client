#ifndef DRAWIMAGESF_H
#define DRAWIMAGESF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawImageSF : public ScriptFunction {
  public:
    DrawImageSF();

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
    static unsigned long long counter;
  };

}

#endif
