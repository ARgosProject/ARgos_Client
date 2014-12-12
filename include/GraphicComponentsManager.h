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
    using GCCollectionPtr = std::shared_ptr<GCCollection>;
    using GCCollectionMap = std::map<std::string, GCCollectionPtr>;

  public:
    GraphicComponentsManager();
    ~GraphicComponentsManager();

    GCCollectionPtr getGCCollection(const std::string& name);
    void removeGCCollection(const std::string& name);

    void showGCCollection(const std::string& name, bool show = true);
    void render(const std::string& name);
    void renderAll();
    void update(const glm::mat4& modelViewMatrix);

    // Builders
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    GCCollectionPtr createVideoFromFile(const std::string& name, const std::string& file_name, float width, float height);
    GCCollectionPtr createCorners(const std::string& name, float length, float wide, const glm::vec4& colour);
    GCCollectionPtr createAxis(const std::string& name, float axis_length, const glm::vec3& pos);
    GCCollectionPtr createVideoStream(const std::string& name, const std::string& bg_file, float width, float height, int port);
    GCCollectionPtr createTextPanel(const std::string& name, const glm::vec4& colour, const std::string& text);
    GCCollectionPtr createHighlight(const std::string& name, const glm::vec4& colour, const glm::vec3& pos, const glm::vec3& scale);
    GCCollectionPtr createButton(const std::string& name, const glm::vec4& colour, const std::string& text);
    GCCollectionPtr createFactureHint(const std::string& name, const glm::vec2& size, const glm::vec4& colour, const std::wstring& title,
                                      const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks);

  private:
    GCCollectionMap _gcCollections;
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
  };

}

#endif
