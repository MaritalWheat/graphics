#pragma once
#include <glm/glm.hpp>
#include "Globals.h"
#include "Camera.h"




Globals::Globals()
{
	this->_numShips = 16;
	this->window_closed = true;
	this->_windowOneSize = glm::ivec2(500, 500);
	this->_windowTwoSize = glm::ivec2(800, 600);
	this->period = 1000 / 120;
	this->_nearPlane = 1.0f;
	this->_farPlane = 12.0f;
	this->fov = 60.0f;
	this->_wireframe = false;
	this->_lighting = false;
	this->camera = Camera(10, 10, 45);
	this->_debugWorldLines = false;
	this->_fog = false;
}