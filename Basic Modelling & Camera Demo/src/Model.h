#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

class Model
{
public:
	Model(glm::vec3 & color, glm::vec3 & position);
	bool Initialize();
	void Draw();
	static bool is_initialized;
	static GLuint _sphereHandle;
	static GLuint _cylinderHandle;
	static GLuint _coneHandle;
	static int _slices;
	static int _stacks;
	glm::vec3 _color;
	glm::vec3 _position;
	float _rotation;
	void DrawDebugLines();

private:
	Model() {};
};
