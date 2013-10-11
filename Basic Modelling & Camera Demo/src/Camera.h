///////////////////////////////////////////////////////////////////////////////
// Camera
// by Erik Harvey
// 
// This code is used for Fall 2013 CS 559, Project 1. The camera is constructed
// to always look towards the origin using spherical coordinates to navigate.
// The camera is interacted with using angles in degrees, but operates in
// radians.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

class Camera{
public:
	Camera(){};
	Camera(float radius, int horizontalAngle, int verticalAngle);
	glm::mat4 getCameraMatrix();
	glm::vec3 getCameraPosition(Camera &cam);
	static void incHorzAngle(Camera &cam, int amount);
	static void incVertAngle(Camera &cam, int amount);

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	glm::vec3 cameraRgt;
	glm::vec3 cameraUp;
	float rad;
	int horzAngle;
	int vertAngle;
	float horzRad;
	float vertRad;
	void updateRadians();
	void updatePosVec();
};
