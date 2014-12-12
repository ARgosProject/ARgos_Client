#ifndef GRAPHICCOMPONENTSMANAGER_H
#define GRAPHICCOMPONENTSMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "GCCollection.h"

#include <glm/glm.hpp>

namespace argosClient {

  class GraphicComponentsManager : public Singleton<GraphicComponentsManager> {
    using GCCollectionMap = std::map<std::string, GCCollection>;

  public:
    GraphicComponentsManager();
    ~GraphicComponentsManager();

    const GCCollection& getGCCollections(const std::string& name);
    void removeGCCollection(const std::string& name);

    void showGCCollection(const std::string& name, bool show = true);
    void render(const std::string& name);
    void renderAll();
    void update(const glm::mat4& modelViewMatrix);

    // Builders
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    GCCollection& createVideoFromFile(const std::string& name, const std::string& file_name, float width, float height);
    GCCollection& createCorners(const std::string& name, float length, float wide, const glm::vec4& colour);
    GCCollection& createAxis(const std::string& name, float axis_length, const glm::vec3& pos);
    GCCollection& createVideoStream(const std::string& name, const std::string& bg_file, float width, float height, int port);
    GCCollection& createTextPanel(const std::string& name, const glm::vec4& colour, const std::string& text);
    GCCollection& createHighlight(const std::string& name, const glm::vec4& colour, const glm::vec3& pos, const glm::vec3& scale);
    GCCollection& createButton(const std::string& name, const glm::vec4& colour, const std::string& text);
    GCCollection& createFactureHint(const std::string& name, const glm::vec2& size, const glm::vec4& colour, const std::wstring& title,
                                    const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks);

  private:
    GCCollectionMap _gcCollections;
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
  };

}

#endif
