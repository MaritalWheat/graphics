///////////////////////////////////////////////////////////////////////////////
// Global Variables for Project 1
// by Erik Harvey and Emanuel Rosu
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "model.h"
#include "Frustum.h"
#include "OrthoSphere.h"


class Globals
{
public:
	Globals();
	int _windowOneHandle;
	int _windowTwoHandle;
	int _numShips;
	float fov;
	float _nearPlane;
	float _farPlane;
	bool window_closed;
	glm::ivec2 _windowOneSize;
	glm::ivec2 _windowTwoSize;
	int period;
	bool _wireframe;
	bool _lighting;
	std::vector<Model *> _ships;
	Camera camera;
	Frustum frust;
	OrthoSphere oSphere;
	bool _debugWorldLines;
	bool _debugObjectLines;
	bool _fog;
};