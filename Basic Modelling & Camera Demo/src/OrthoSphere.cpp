#pragma once
#include "OrthoSphere.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;




GLuint OrthoSphere::_sphereHandle	= BAD_GL_VALUE;
int OrthoSphere::_slices			= 40;
int OrthoSphere::_stacks			= 18;
bool OrthoSphere::is_initialized	= false;

void OrthoSphere::DrawDebugLines() {
	glColor3f(0.0, 0.0f, 1.0f);
	glBegin(GL_LINES); //Begin drawing lines
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);
    glEnd();

	glColor3f(0.0, 1.0, 0.0f);
	glBegin(GL_LINES); //Begin drawing lines
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);
    glEnd();

	glColor3f(1.0, 0.0f, 0.0f);
	glBegin(GL_LINES); //Begin drawing lines
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);
    glEnd();
}

void OrthoSphere::Draw()
{
	// Note: Assumes that matrix mode is MODELVIEW and that the "origin" is 0.0,0.0,0.0
	// The getting and putting back of the modelview matrix is an artifact of legacy GL.
	// In modern GL every matrix is of your own making. You have total control by what
	// to compound and what to start fresh.
	glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	
	glm::mat4 oSphere(1.0f);
	glm::mat4 modelView(1.0f);
	// This is how we pick up the previous and compound.
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));

	glLoadMatrixf(glm::value_ptr(oSphere));
	glCallList(_sphereHandle);

	glLoadMatrixf(glm::value_ptr(modelView));
}

void OrthoSphere::Draw2()
{
	glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	
	glm::mat4 oSphere(1.0f);
	glm::mat4 modelView(1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));

	oSphere = glm::rotate(oSphere, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(glm::value_ptr(oSphere));
	glCallList(_sphereHandle);

	glLoadMatrixf(glm::value_ptr(modelView));
}

bool OrthoSphere::Initialize()
{
			if ((this->_sphereHandle = glGenLists(1)) == 0)
			{
				std::cout << "Model::Initialize() - Failed to GenLists()" << std::endl;
				return false;
			}
			glNewList(this->_sphereHandle, GL_COMPILE);
			glutWireSphere(10.0, this->_slices, this->_stacks);
			glEndList();
			this->is_initialized = true;
	return true;
}