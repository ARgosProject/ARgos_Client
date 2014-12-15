#include "GCCollection.h"
#include "GraphicComponent.h"
#include "Log.h"

namespace argosClient {

  GCCollection::GCCollection()
    : GCCollection("") {

  }

  GCCollection::GCCollection(const std::string& name)
    : _name(name), _isShowing(false), _isUpdating(true) {

  }

  GCCollection::GCCollection(const std::string& name, const std::vector<GraphicComponentPtr>& graphicComponents)
    : GCCollection(name) {
    _graphicComponents = graphicComponents;
  }

  GCCollection::GCCollection(const std::vector<GraphicComponentPtr>& graphicComponents) {
    _graphicComponents = graphicComponents;
  }

  GCCollection::~GCCollection() {
    Log::info("Releasing all graphic components from collection '" + _name + "'...");
    _graphicComponents.clear();
  }

  GCCollection::GCCollectionPtr GCCollection::add(GraphicComponentPtr graphicComponent) {
    _graphicComponents.push_back(graphicComponent);

    return shared_from_this();
  }

  GCCollection::GraphicComponentPtr GCCollection::get(int index) {
    return _graphicComponents[index];
  }

  const std::vector<GCCollection::GraphicComponentPtr>& GCCollection::get() {
    return _graphicComponents;
  }

  GCCollection::GCCollectionPtr GCCollection::noUpdate(bool noUpdate) {
    for(auto& gc : _graphicComponents) {
      gc->noUpdate(noUpdate);
    }

    _isUpdating = noUpdate;

    return shared_from_this();
  }

  GCCollection::GCCollectionPtr GCCollection::show(bool show) {
    for(auto& gc : _graphicComponents) {
      gc->show(show);
    }

    _isShowing = show;

    return shared_from_this();
  }

  GCCollection::GCCollectionPtr GCCollection::render() {
    for(auto& gc : _graphicComponents) {
      gc->render();
    }

    return shared_from_this();
  }

  GCCollection::GCCollectionPtr GCCollection::update(const glm::mat4& modelViewMatrix) {
    for(auto& gc : _graphicComponents) {
      gc->setModelViewMatrix(modelViewMatrix);
    }

    return shared_from_this();
  }

}
