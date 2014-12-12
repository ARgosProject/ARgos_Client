#include "GCCollection.h"
#include "GraphicComponent.h"
#include "Log.h"

namespace argosClient {

  GCCollection::GCCollection()
    : GCCollection("") {

  }

  GCCollection::GCCollection(const std::string& name)
    : _name(name), _isShowing(false) {

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

  void GCCollection::add(GraphicComponentPtr graphicComponent) {
    _graphicComponents.push_back(graphicComponent);
  }

  const std::vector<GCCollection::GraphicComponentPtr>& GCCollection::get() {
    return _graphicComponents;
  }

  void GCCollection::show(bool show) {
    for(auto& gc : _graphicComponents) {
      gc->show(show);
    }
  }

  void GCCollection::render() {
    for(auto& gc : _graphicComponents) {
      gc->render();
    }
  }

  void GCCollection::update(const glm::mat4& modelViewMatrix) {
    for(auto& gc : _graphicComponents) {
      gc->setModelViewMatrix(modelViewMatrix);
    }
  }

  GCCollection::GCCollection(const GCCollection& other)
    : _name(other._name), _graphicComponents(other._graphicComponents), _isShowing(other._isShowing) {

  }

  GCCollection& GCCollection::operator=(const GCCollection& other) {
    if(this != &other) {
      _name = other._name;
      _graphicComponents = other._graphicComponents;
      _isShowing = other._isShowing;
    }

    return *this;
  }

}
