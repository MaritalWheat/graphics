#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

class OrthoSphere
{
public:
	bool Initialize();
	void Draw();
	void Draw2();
	static bool is_initialized;
	static GLuint _sphereHandle;
	static int _slices;
	static int _stacks;
	void DrawDebugLines();
	OrthoSphere() {};

private:
};
