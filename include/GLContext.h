#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <opencv2/opencv.hpp>
#include <glm/mat4x4.hpp>

#include "Singleton.h"
#include "EGLWindow.h"
#include "GfxProgram.h"
#include "TaskDelegation.h"

namespace argosClient {

  class GraphicComponentsManager;
  class ScriptFunction;
  class ImageComponent;

  /**
   * The OpenGL ES 2.0 context
   * Addresses all rendering stuff and updates the properties of all
   * graphic components
   */
  class GLContext : public EGLWindow, public Singleton<GLContext> {

  public:
    /**
     * Constructs a new OpenGL ES 2.0 context
     * @param config The config we want for the context. If NULL, a default
     * config is built
     */
    GLContext(EGLconfig* config = NULL);

    /**
     * Destroys the OpenGL ES 2.0 context
     */
    ~GLContext();

    /**
     * The main drawing function
     */
    void render() override;

    /**
     * Updates the Model View matrix of all graphic components according to a paper
     * @param paper The paper we want to center all the graphic components
     */
    bool update(paper_t& paper);

    /**
     * Sets the projection matrix used to update the graphic components transforms
     * @param projectionMatrix The projection matrix used by the context to update the graphic
     * components transforms
     */
    void setProjectionMatrix(glm::mat4 projectionMatrix);

    /**
     * Retrieves the reference of the saved projection matrix
     * @return the projection matrix
     */
    const glm::mat4& getProjectionMatrix() const;

    /**
     * Sets up specific properties for the context
     */
    void start() override;

  private:
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
    std::map<int, ScriptFunction*> _handlers; ///< An associative list of function pointer to script functions
    GraphicComponentsManager& _gcManager; ///< A reference to the GraphicComponentsManager
    ImageComponent* _projArea;
  };

}

#endif
