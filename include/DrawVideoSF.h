#ifndef DRAWVIDEOSF_H
#define DRAWVIDEOSF_H

#include <vector>
#include <string>

#include "ScriptFunction.h"

namespace argosClient {

  class GraphicComponentsManager;

  class DrawVideoSF : public ScriptFunction {
  public:
    DrawVideoSF();

    void execute(const std::vector<std::string>& args) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;

    static std::string name;
    static unsigned long long counter;
  };

}

#endif
