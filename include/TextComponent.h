#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <GLES2/gl2.h>

#include <freetypeGlesRpi/texture-atlas.h>
#include <freetypeGlesRpi/texture-font.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class TextComponent : public GraphicComponent {
public:
  TextComponent(std::string filename, GLint atlas_size);
  ~TextComponent();

  void translate(glm::vec3 const & translation) override;

  void setFont(const std::string& filename, GLfloat size);
  void setText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
  void addText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

  void render() override;

private:
  void setUpShader() override;

private:
  vector_t* _vector;
  texture_font_t* _font;
  texture_atlas_t* _atlas;
  vec2 _pen, _orig;
};

#endif
