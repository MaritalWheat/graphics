///////////////////////////////////////////////////////////////////////////////
// Frustum
// by Erik Harvey
// 
// This code is used for Fall 2013 CS 559, Project 1. The frustum is
// represented by four points on each plane (near and far) and one for the 
// camera's position. These plane vertices are connected by a line and each
// plane connected to its respective point on the other. There is also a ray
// for the camera's direction. 
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "Camera.h"

#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

class Frustum{
public:
	void Draw();
	bool Initialize();
	static void updateVectors(Frustum &frustum, Camera &camera);
	static bool is_initialized;
	Frustum(Camera &camera);
	Frustum();

private:
	glm::vec3 cameraPosition;
	glm::vec3 nearV1;
	glm::vec3 nearV2;
	glm::vec3 nearV3;
	glm::vec3 nearV4;
	glm::vec3 farV1;
	glm::vec3 farV2;
	glm::vec3 farV3;
	glm::vec3 farV4;
	static GLuint sphereHandle;
	static glm::vec3 getWorldCoords(glm::vec3 vector);
};
