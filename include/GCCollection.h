#ifndef GCCOLLECTION_H
#define GCCOLLECTION_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace argosClient {

  class GraphicComponent;

  /**
   * A collection of graphic components for whole management
   */
  class GCCollection : public std::enable_shared_from_this<GCCollection> {
    using GCCollectionPtr = std::shared_ptr<GCCollection>;
    using GraphicComponentPtr = std::shared_ptr<GraphicComponent>;

  public:
    /**
     * Constructs a new GCCollection
     */
    GCCollection();

    /**
     * Constructs a new GCCollection with a name
     */
    GCCollection(const std::string& name);

    /**
     * Constructs a new GCCollection with a given list of graphic components to hold
     */
    GCCollection(const std::vector<GraphicComponentPtr>& graphicComponents);

    /**
     * Constructs a new GCCollection with a name and a given list of graphic components to hold
     */
    GCCollection(const std::string& name, const std::vector<GraphicComponentPtr>& graphicComponents);

    /**
     * Destroys the GCCollection
     */
    ~GCCollection();

    /**
     * Adds an existing graphic component to this collection
     * @param graphicComponent The graphic component to be added
     */
    GCCollectionPtr add(GraphicComponentPtr graphicComponent);

    /**
     * Retrieves a graphic component by its index
     * @param index The index of the graphic component to retrieve
     * @return The graphic component by its index
     */
    GraphicComponentPtr get(int index);

    /**
     * Retrieves the whole list of graphic components kept in this collection
     * @return The whole list of graphic components
     */
    const std::vector<GraphicComponentPtr>& get();

    /**
     * Tells whether to update all the graphic components of this collection with
     * the document model view matrix or not
     * @param noUpdate Whether to update the graphic components or not
     * @return This collection to chain methods
     */
    GCCollectionPtr noUpdate(bool noUpdate = true);

    /**
     * Tells whether to show all the graphic components of this collection
     * @param show Whether to show the graphic components or not
     * @return This collection to chain methods
     */
    GCCollectionPtr show(bool show = true);

    /**
     * Renders all the graphic components of the collections
     * @return This collection to chain methods
     */
    GCCollectionPtr render();

    /**
     * Update all the graphic components of this collections with the
     * document model view matrix
     * @param modelViewMatrix The model view matrix to update the graphic components
     * @return This collection to chain methods
     */
    GCCollectionPtr update(const glm::mat4& modelViewMatrix);

  private:
    std::string _name; ///< The name of the collections
    std::vector<GraphicComponentPtr> _graphicComponents; ///< A list of graphic components to hold
    bool _isShowing; ///< Whether the graphic components are being showed or not
    bool _isUpdating; ///< Whether the graphic components are being updated or not
  };

}

#endif
