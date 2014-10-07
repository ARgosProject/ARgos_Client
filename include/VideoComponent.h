#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <GLES2/gl2.h>

#include "GraphicComponent.h"
#include "VideoThread.h"
#include "GfxProgram.h"

class VideoComponent : public GraphicComponent {
public:
  VideoComponent(GLfloat width, GLfloat height);
	VideoComponent(const std::string& fileName, GLfloat width, GLfloat height);
  ~VideoComponent();

	void setLoop(bool loop);
	void loadVideoFromFile(const std::string& fileName);
  void render() override;

private:
  void setUpShader() override;

private:
  GLushort* _indices;
  GLfloat* _vertexData;
  GLfloat _width, _height;
	GLuint _textureId;

	bool _loop;
	std::string _fileName;
	void* _eglImage;
	VideoThread* _videoThread;
};

#endif
