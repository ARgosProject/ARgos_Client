#ifndef GRAPHICCOMPONENTSMANAGER_H
#define GRAPHICCOMPONENTSMANAGER_H

#include <map>
#include <string>
#include "Singleton.h"

#include <glm/vec3.hpp>

namespace argosClient {

  class GraphicComponent;

  class GraphicComponentsManager : public Singleton<GraphicComponentsManager> {
    using GraphicComponentsMap = std::map<std::string, GraphicComponent*>;

  public:
    GraphicComponentsManager();
    ~GraphicComponentsManager();

    //void update();

    // Builders
    void buildTextPanel(const glm::vec3& colour, const std::string& text);
    void buildHighlight(const glm::vec3& colour, const glm::vec3& pos, const glm::vec3& scale);
    void buildButton(const glm::vec3& colour, const std::string& text);

  private:
    GraphicComponentsMap _graphicComponents;
  };

}

#endif
