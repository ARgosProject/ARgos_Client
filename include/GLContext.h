#ifndef MAINGLWINDOW_H
#define MAINGLWINDOW_H

#include <opencv2/opencv.hpp>
#include <glm/mat4x4.hpp>

#include "Singleton.h"
#include "EGLWindow.h"
#include "GfxProgram.h"
#include "GraphicComponent.h"
#include "TaskDelegation.h"

namespace argosClient {
  class TextComponent;

  /**
   * The OpenGL ES 2.0 context
   * Addresses all rendering stuff and updates the properties of all
   * graphic components
   */
  class GLContext : public EGLWindow, public Singleton<GLContext> {
    /** An alias for a map of graphic components pointers */
    typedef std::map<std::string, GraphicComponent*> GraphicComponentMap;

  public:
    enum State {
      INTRODUCTION,
      FETCH_PAPERS
    };

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
     * Passes the number of frames per second to the context in order to render it
     * @param fps The number of FPS (Frames Per Second) of the application
     */
    void setFps(float fps);

    /**
     * Updates the Model View matrix of all graphic components according to a paper
     * @param paper The paper we want to center all the graphic components
     */
    bool update(cv::Mat& currentFrame, paper_t& paper);

    /**
     * Matches a paper ID to difference among various
     * @param id The ID of the paper
     */
    void fetchPaperId(int id);

    /**
     * Passes a retrieved frame from the camera to stream it on video stream trigger
     * @param currentFrame A frame from the camera
     */
    void setFrame(cv::Mat& currentFrame);

    /**
     * Retrieves the reference of the saved frame
     * @return the saved frame
     */
    cv::Mat& getFrame();

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

    /**
     * A utility method used to draw an axis
     * @param axis_length The length of the axis lines
     * @return a list of graphic components representing the figure to draw
     */
    std::vector<GraphicComponent*> makeAxis(GLfloat axis_length);

    /**
     * A utility method used to draw the 4 corners of the paper
     * @param length The length of the corner lines
     * @param r The red component of the final colour for the corner lines
     * @param g The green component of the final colour for the corner lines
     * @param b The blue component of the final colour for the corner lines
     * @param a The alpha component of the final colour for the corner lines
     * @return a list of graphic components representing the figure to draw
     */
    std::vector<GraphicComponent*> makeCorners(GLfloat length, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * A utility method used to draw text
     * @param r The red component of the final colour for the corner lines
     * @param g The green component of the final colour for the corner lines
     * @param b The blue component of the final colour for the corner lines
     * @param a The alpha component of the final colour for the corner lines
     * @return a list of graphic components for each line of text
     */
    std::vector<GraphicComponent*> makeText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * A utility method used to draw some factures
     * @return a list of graphic components with the factures to draw
     */
    std::vector<GraphicComponent*> makeFactures();

    /**
     * A utility method used to build a facture widget
     */
    GraphicComponent* makeFacture(const glm::vec2& size,
                                  const glm::vec4& colour,
                                  const std::wstring& title,
                                  const std::vector<std::pair<std::wstring, glm::vec3>>& textBlocks);

    /**
     * Add a new graphic component to this context
     * @param name The name of the new graphic component
     * @param gc The graphic component to add
     */
    void addGraphicComponent(const std::string& name, GraphicComponent* gc);

    /**
     * Retrieves a specified graphic component
     * @return the specified graphic component
     */
    GraphicComponent* getGraphicComponent(const std::string& name);

    /**
     * Removes a specified graphic component
     * @param name the specified graphic component to remove
     */
    void removeGraphicComponent(const std::string& name);

    /**
     * Sets the state of the GLContext
     * @param state The new state
     */
    void setState(State state);

  private:
    State _state;
    GraphicComponentMap _graphicComponents; ///< A map of the created graphic components
    cv::Mat* _frame; ///< The current frame of the camera used on video streaming
    glm::mat4 _projectionMatrix; ///< The projection matrix used to update the graphic components transformations
  };

}

#endif
