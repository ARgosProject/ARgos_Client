#ifndef TEXTURERENDER_H
#define TEXTURERENDER_H

#include <vector>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "GfxProgram.h"

class TextureRender : public GraphicComponent {
public:
  TextureRender(GLfloat width, GLfloat height);
  ~TextureRender();

  GraphicComponent* getGraphicComponent(int index);
  void addGraphicComponent(GraphicComponent* graphicComponent);
  void render() override;

private:
  void setUpShader() override;

	int genFrameBufferObject();
	void renderToTexture();
	void drawTexture();

private:
  std::vector<GraphicComponent*> _graphicComponents;

  GLfloat* _vertexData;
  GLushort* _indices;
  GLuint _framebufferObject, _depthRenderbuffer, _texture;
  GLfloat _texWidth, _texHeight;
  uint32_t _screenWidth, _screenHeight;
};

#endif
