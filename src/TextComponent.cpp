#include <cassert>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextComponent.h"

TextComponent::TextComponent(std::string filename, GLint fontSize)
  : _vector(NULL), _font(NULL), _atlas(NULL) {
  _atlas = texture_atlas_new(fontSize*8, fontSize*8, 1);
  _vector = vector_new(sizeof(GLfloat));

  _orig.x = 0;
  _orig.y = 0;
  _pen.x = 0;
  _pen.y = 0;

	// Load font and cache glyphs
  setFont(filename, fontSize);

	// Set the shader
	this->loadGLProgram("shaders/text.glvs", "shaders/text.glfs");
}

TextComponent::~TextComponent() {
  if(_font) {
    texture_font_delete(_font);
  }
  if(_atlas) {
    texture_atlas_delete(_atlas);
  }
  if(_vector) {
    vector_delete(_vector);
  }

  _font = NULL;
  _atlas = NULL;
  _vector = NULL;
}

void TextComponent::setUpShader() {
  GLint id = _shader.getId();

  _vertexHandler = glGetAttribLocation(id, "a_position");
  _texHandler = glGetAttribLocation(id, "a_st");
  _colorHandler = glGetAttribLocation(id, "a_color");
  _samplerHandler = glGetUniformLocation(id, "texture_uniform");
  _mvpHandler = glGetUniformLocation(id, "u_mvp");

  texture_atlas_upload(_atlas);
}

void TextComponent::translate(glm::vec3 const & translation) {
  _orig.x = translation.x;
  _orig.y = translation.y;
  _pen.x = translation.x;
  _pen.y = translation.y;

  glm::translate(_modelViewMatrix, translation);
}

void TextComponent::setFont(const std::string& filename, GLfloat size) {
  _font = texture_font_new(_atlas, filename.c_str(), size);
  texture_font_load_glyphs(_font,
			   L" !\"#$%&'()*+,-./0123456789:;<=>?"
			   L"@ABCDEFGHIJKLMNÑOPQRSTUVWXYZ[\\]^_"
			   L"`abcdefghijklmnñopqrstuvwxyz{|}~");
}

void TextComponent::setText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  _pen.x = _orig.x;
  _pen.y = _orig.y;
  vector_clear(_vector);
  addText(strtext, r, g, b, a);
}

void TextComponent::addText(std::wstring strtext, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  const wchar_t* text = strtext.c_str();

  for(size_t i = 0; i < wcslen(text); i++) {
    texture_glyph_t* glyph = texture_font_get_glyph(_font, text[i]);

    if(glyph != NULL) {
      int kerning = 0;

      if(i > 0) {
				kerning = texture_glyph_get_kerning(glyph, text[i-1]);
      }

      _pen.x += kerning;
      int x0  = (int)(_pen.x + glyph->offset_x);
      int y0  = (int)(_pen.y + glyph->offset_y);
      int x1  = (int)(x0 + glyph->width);
      int y1  = (int)(y0 - glyph->height);
      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1;
      float t1 = glyph->t1;

      // Data is x,y,z,s,t,r,g,b,a
      GLfloat vertices[] = {
				(GLfloat)x0,(GLfloat)y0,0,
				s0,t0,
				r, g, b, a,
				(GLfloat)x0,(GLfloat)y1,0,
				s0,t1,
				r, g, b, a,
				(GLfloat)x1,(GLfloat)y1,0,
				s1,t1,
				r, g, b, a,
				(GLfloat)x0,(GLfloat)y0,0,
				s0,t0,
				r, g, b, a,
				(GLfloat)x1,(GLfloat)y1,0,
				s1,t1,
				r, g, b, a,
				(GLfloat)x1,(GLfloat)y0,0,
				s1,t0,
				r, g, b, a
      };

      vector_push_back_data(_vector, vertices, 54);

      _pen.x += glyph->advance_x;
    }
  }
}

void TextComponent::render() {
  // Use the program object
  _shader.useProgram();

  // Load the vertex data
  glVertexAttribPointer(_vertexHandler, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), _vector->items);
  glEnableVertexAttribArray(_vertexHandler);
  glVertexAttribPointer(_texHandler, 2, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (GLfloat*)_vector->items+3);
  glEnableVertexAttribArray(_texHandler);
  glVertexAttribPointer(_colorHandler, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (GLfloat*)_vector->items+5);
  glEnableVertexAttribArray(_colorHandler);
  glUniformMatrix4fv(_mvpHandler, 1, GL_FALSE, glm::value_ptr(_projectionMatrix * _modelViewMatrix));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _atlas->id);

  glUniform1i(_samplerHandler, 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);

  glDrawArrays(GL_TRIANGLES, 0, _vector->size/9);

	glDisable(GL_BLEND);
}
