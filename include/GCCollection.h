#ifndef GCCOLLECTION_H
#define GCCOLLECTION_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace argosClient {

  class GraphicComponent;

  class GCCollection {
    using GraphicComponentPtr = std::shared_ptr<GraphicComponent>;

  public:
    GCCollection();
    GCCollection(const std::string& name);
    GCCollection(const std::vector<GraphicComponentPtr>& graphicComponents);
    GCCollection(const std::string& name, const std::vector<GraphicComponentPtr>& graphicComponents);
    ~GCCollection();

    void add(GraphicComponentPtr graphicComponent);
    const std::vector<GraphicComponentPtr>& get();

    void show(bool show);
    void render();
    void update(const glm::mat4& modelViewMatrix);

  private:
    std::string _name;
    std::vector<GraphicComponentPtr> _graphicComponents;
    bool _isShowing;
  };

}

#endif
