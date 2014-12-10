#ifndef GRAPHICCOMPONENTSMANAGER_H
#define GRAPHICCOMPONENTSMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

#include <glm/glm.hpp>

namespace argosClient {

  class GraphicComponent;

  class GraphicComponentsManager : public Singleton<GraphicComponentsManager> {
    using GraphicComponentPtr  = std::shared_ptr<GraphicComponent>;
    using GraphicComponentsMap = std::map<std::string, GraphicComponentPtr>;

  public:
    GraphicComponentsManager();
    ~GraphicComponentsManager();

    GraphicComponentPtr getGraphicComponent(const std::string& name);
    void removeGraphicComponent(const std::string& name);

    void showGraphicComponent(const std::string& name, bool show = true);
    void update(const glm::mat4& modelViewMatrix);

    // Builders
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    GraphicComponentPtr createCorners(const std::string& name, float length, float width, const glm::vec4& colour);
    GraphicComponentPtr createAxis(const std::string& name, float axis_length, const glm::vec3& pos);
    GraphicComponentPtr createVideoStream(const std::string& name, const std::string& bg_file, float width, float height, int port);
    GraphicComponentPtr createTextPanel(const std::string& name, const glm::vec4& colour, const std::string& text);
    GraphicComponentPtr createHighlight(const std::string& name, const glm::vec4& colour, const glm::vec3& pos, const glm::vec3& scale);
    GraphicComponentPtr createButton(const std::string& name, const glm::vec4& colour, const std::string& text);
    GraphicComponentPtr createFactureHint(const std::string& name, const glm::vec2& size, const glm::vec4& colour, const std::wstring& title,
                                          const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks);

  private:
    GraphicComponentsMap _graphicComponents;
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
  };

}

#endif
