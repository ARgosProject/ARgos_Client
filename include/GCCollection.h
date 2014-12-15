#ifndef GCCOLLECTION_H
#define GCCOLLECTION_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace argosClient {

  class GraphicComponent;

  class GCCollection : public std::enable_shared_from_this<GCCollection> {
    using GCCollectionPtr = std::shared_ptr<GCCollection>;
    using GraphicComponentPtr = std::shared_ptr<GraphicComponent>;

  public:
    GCCollection();
    GCCollection(const std::string& name);
    GCCollection(const std::vector<GraphicComponentPtr>& graphicComponents);
    GCCollection(const std::string& name, const std::vector<GraphicComponentPtr>& graphicComponents);
    ~GCCollection();

    GCCollectionPtr add(GraphicComponentPtr graphicComponent);
    GraphicComponentPtr get(int index);
    const std::vector<GraphicComponentPtr>& get();

    GCCollectionPtr noUpdate(bool noUpdate = true);
    GCCollectionPtr show(bool show = true);
    GCCollectionPtr render();
    GCCollectionPtr update(const glm::mat4& modelViewMatrix);

  private:
    std::string _name;
    std::vector<GraphicComponentPtr> _graphicComponents;
    bool _isShowing;
    bool _isUpdating;
  };

}

#endif
