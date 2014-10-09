#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <GLES2/gl2.h>

#include <freetypeGlesRpi/texture-atlas.h>
#include <freetypeGlesRpi/texture-font.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

namespace argosClient {

  /**
   * A class representing a text string
   */
  class TextComponent : public GraphicComponent {
  public:
    /**
     * Constructs a new text object
     * @param filename The path to the font to use
     * @param fontSize The font size
     * @see setFont()
     */
    TextComponent(std::string filename, GLint fontSize);

    /**
     * Destroys the text object
     */
    ~TextComponent();

    /**
     * Translates this graphic component relatively
     * @param translation The movement amount to translate
     */
    void translate(glm::vec3 const & translation) override;

    /**
     * Sets the font for the text object
     * @param filename The path to the font to use
     * @param fontSize The font size
     */
    void setFont(const std::string& filename, GLfloat size);

    /**
     * Sets a string to the text object and a colour for it
     * @param strtext The string to sets the text object
     * @param r The red component
     * @param g The green component
     * @param b The blue component
     * @param a The alpha component
     */
    void setText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * Adds a string to the text object and a colour for it
     * @param strtext The string to sets the text object
     * @param r The red component
     * @param g The green component
     * @param b The blue component
     * @param a The alpha component
     */
    void addText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * Draws this graphic component
     */
    void render() override;

  private:
    /**
     * Sets up the shader for this graphic component
     */
    void setUpShader() override;

  private:
    vector_t* _vector; ///< A vector of vertices
    texture_font_t* _font; ///< The texture font
    texture_atlas_t* _atlas; ///< The atlas of characters
    vec2 _pen; ///< The position of a character in the atlas
    vec2 _orig; ///< The original position of the first character in the atlas
  };

}

#endif
