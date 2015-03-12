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

    void _execute(const std::vector<std::string>& args, int id) override;

  private:
    GraphicComponentsManager& _graphicComponentsManager;
    static unsigned long long counter;
  };

}

#endif
