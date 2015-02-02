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

  /**
   * The graphic component manager used to handle all the graphic components related creation and
   * updating
   */
  class GraphicComponentsManager : public Singleton<GraphicComponentsManager> {
    using GCCollectionPtr = std::shared_ptr<GCCollection>;
    using GCCollectionMap = std::map<std::string, GCCollectionPtr>;

  public:
    /**
     * Constructs a new GraphicComponentManager
     */
    GraphicComponentsManager();

    /**
     * Destroys the GraphicComponentManager
     */
    ~GraphicComponentsManager();

    /**
     * Retrieves the graphic components collection specified by its name
     * @param name The name of the graphic components collection to retrieve
     * @return The graphic component collection
     */
    GCCollectionPtr getGCCollection(const std::string& name);

    /**
     * Removes and release the graphic components collection specified by its name
     * @param name The name of the graphic components collection to remove
     */
    void removeGCCollection(const std::string& name);

    /**
     * Tells to show or not a graphic components collections
     * @param name The name of the graphic components collection to show
     * @param show Whether to show or not the graphic component collection
     */
    void showGCCollection(const std::string& name, bool show = true);

    /**
     * Removes all the graphic components for the given document ID
     * @param id The document id
     */
    void cleanForId(int id);

    /**
     * Render the specified graphic components collection
     */
    void render(const std::string& name);

    /**
     * Renders all the graphic components collections
     */
    void renderAll();

    /**
     * Updates the graphic components collection with a model view matrix
     * @param modelViewMatrix The matrix to update the graphic components collections
     */
    void update(const glm::mat4& modelViewMatrix);

    /**
     * Sets the projection matrix
     * @param projectionMatrix The projection matrix used to update the graphic components collections
     */
    void setProjectionMatrix(const glm::mat4& projectionMatrix);

    /**
     * Sets the images directory path
     * @param path The path where the image files are located
     */
    void setImagesPath(const std::string& path);

    /**
     * Sets the videos directory path
     * @param path The path where the video files are located
     */
    void setVideosPath(const std::string& path);

    /**
     * Sets the fonts directory path
     * @param path The path where the font files are located
     */
    void setFontsPath(const std::string& path);

    /**
     * Creates a new image from file
     * @param name The given name to the resulting graphic components collection
     * @param file_name The image file to load
     * @param pos The position
     * @param size The size
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createImageFromFile(const std::string& name, const std::string& file_name, const glm::vec3& pos, const glm::vec2& size, bool flat = false);

    /**
     * Creates a new video from file
     * @param name The given name to the resulting graphic components collection
     * @param file_name The video file to load
     * @param pos The position
     * @param size The size
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createVideoFromFile(const std::string& name, const std::string& file_name, const glm::vec3& pos, const glm::vec2& size);

    /**
     * Creates 4 line corners adapted to the document
     * @param name The given name to the resulting graphic components collection
     * @param length The length of the lines
     * @param wide The width of the lines
     * @param colour The colour of the lines
     * @param size The size of the document (A4 = 21.0x29.7)
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createCorners(const std::string& name, float length, float wide, const glm::vec4& colour, const glm::vec2& size);

    /**
     * Creates an XYZ axes represented by RGB colour
     * @param name The given name to the resulting graphic components collection
     * @param length The length of each axis
     * @param wide The width of each axis
     * @param pos The position
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createAxis(const std::string& name, float length, float wide, const glm::vec3& pos);

    /**
     * Inits and creates a video stream
     * @param name The given name to the resulting graphic components collection
     * @param bg_file The background image of this video stream
     * @param size The size
     * @param port The port to listen for frames
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createVideostream(const std::string& name, const std::string& bg_file, const glm::vec2& size, int port);

    /**
     * Creates a new text panel
     * @param name The given name to the resulting graphic components collection
     * @param colour The colour
     * @param fontSize The font size
     * @param text The text
     * @param pos The position
     * @param size The size
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createTextPanel(const std::string& name, const glm::vec4& colour, int fontSize, const std::wstring& text, const glm::vec3& pos, const glm::vec2& size);

    /**
     * Creates an area highlight
     * @param name The given name to the resulting graphic components collection
     * @param colour The colour
     * @param pos The position
     * @param scale The scale
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createHighlight(const std::string& name, const glm::vec4& colour, const glm::vec3& pos, const glm::vec3& scale);

    /**
     * Creates a button
     * @param name The given name to the resulting graphic components collection
     * @param colour The colour
     * @param text The text
     * @param pos The position
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createButton(const std::string& name, const glm::vec4& colour, const std::wstring& text, const glm::vec3& pos);

    /**
     * Creates a hint to help in document management
     * @param name The given name to the resulting graphic components collection
     * @param pos The position
     * @param size The size
     * @param colour The colour
     * @param title The text title
     * @param textBlocks A list of pairs text-positions
     * @return The resulting graphic components collection
     */
    GCCollectionPtr createFactureHint(const std::string& name, const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour,
                                      const std::wstring& title, const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks);

  private:
    GCCollectionMap _gcCollections; ///< An associative list of graphic components collections indexed by their name
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
    std::string _imagesPath; ///< The image files directory
    std::string _videosPath; ///< The video files directory
    std::string _fontsPath; ///< The font files directory
  };

}

#endif
