#pragma once
#include "Frustum.h"
#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



GLuint Frustum::sphereHandle = BAD_GL_VALUE;
bool Frustum::is_initialized = false;

Frustum::Frustum(Camera &camera){
	updateVectors(*this, camera);
}

Frustum::Frustum(){};

// Update the vectors that find the world coordinates from clipping coordinates
void Frustum::updateVectors(Frustum &frustum, Camera &camera){
	// Get camera position
	frustum.cameraPosition = camera.getCameraPosition(camera);
	// Update near plane
	frustum.nearV1 = getWorldCoords(glm::vec3(-1.0f, 1.0f, -1.0f));
	frustum.nearV2 = getWorldCoords(glm::vec3(1.0f, 1.0f, -1.0f));
	frustum.nearV3 = getWorldCoords(glm::vec3(1.0f, -1.0f, -1.0f));
	frustum.nearV4 = getWorldCoords(glm::vec3(-1.0f, -1.0f, -1.0f));
	// Update far plane
	frustum.farV1 = getWorldCoords(glm::vec3(-1.0f, 1.0f, 1.0f));
	frustum.farV2 = getWorldCoords(glm::vec3(1.0f, 1.0f, 1.0f));
	frustum.farV3 = getWorldCoords(glm::vec3(1.0f, -1.0f, 1.0f));
	frustum.farV4 = getWorldCoords(glm::vec3(-1.0f, -1.0f, 1.0f));
}

// Draws the components of the frustum. That includes 9 spheres, and 13 lines
void Frustum::Draw()
{
	// Note: Assumes that matrix mode is MODELVIEW and that the "origin" is 0.0,0.0,0.0
	// The getting and putting back of the modelview matrix is an artifact of legacy GL.
	// In modern GL every matrix is of your own making. You have total control by what
	// to compound and what to start fresh.
	glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
	glColor3fv(glm::value_ptr(white));
	
	glm::mat4 camSphere(1.0f);
	glm::mat4 modelView(1.0f);
	// This is how we pick up the previous and compound.
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));

	// Draw the camera sphere
	camSphere = glm::translate(modelView, this->cameraPosition);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset Scale
	camSphere = glm::scale(camSphere, glm::vec3(0.1f, 0.1f, 0.1f));

	// Draw the first point on the near plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, nearV1);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset Scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the second point on the near plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, nearV2);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the third point on the near plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, nearV3);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the fourth point on the near plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, nearV4);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);
	
	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the first point on the far plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, farV1);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the second point on the far plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, farV2);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the third point on the far plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, farV3);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));

	// Draw the fourth point on the far plane
	camSphere = glm::mat4(1.0f);
	camSphere = glm::translate(modelView, farV4);
	camSphere = glm::scale(camSphere, glm::vec3(0.125f, 0.125f, 0.125f));
	glLoadMatrixf(glm::value_ptr(camSphere));
	glCallList(sphereHandle);

	// Reset the scale
	camSphere = glm::scale(camSphere, glm::vec3(8.0f, 8.0f, 8.0f));
	
	// Reset the matrix so the lines draw correctly
	glm::mat4 resetMat(1.0f);
	resetMat = camSphere;
	resetMat = glm::translate(modelView, glm::vec3(0.0f, 0.0f, 0.0f));
	resetMat = glm::scale(resetMat, glm::vec3(1.0f, 1.0f, 1.0f));
	glLoadMatrixf(glm::value_ptr(resetMat));

	// A normalize vector for the camera position, used in the camera ray
	glm::vec3 camPosNorm = glm::normalize(this->cameraPosition);

	glBegin(GL_LINES);
    glLineWidth(1.0f);
	glVertex3f(nearV1.x, nearV1.y, nearV1.z); //line 1
	glVertex3f(nearV2.x, nearV2.y, nearV2.z);
	glVertex3f(nearV2.x, nearV2.y, nearV2.z); //line 2
	glVertex3f(nearV3.x, nearV3.y, nearV3.z);
	glVertex3f(nearV3.x, nearV3.y, nearV3.z); //line 3
	glVertex3f(nearV4.x, nearV4.y, nearV4.z);
	glVertex3f(nearV4.x, nearV4.y, nearV4.z); //line 4
	glVertex3f(nearV1.x, nearV1.y, nearV1.z);
	glVertex3f(nearV1.x, nearV1.y, nearV1.z); //line 5
	glVertex3f(farV1.x, farV1.y, farV1.z);
	glVertex3f(nearV2.x, nearV2.y, nearV2.z); //line 6
	glVertex3f(farV2.x, farV2.y, farV2.z);
	glVertex3f(nearV3.x, nearV3.y, nearV3.z); //line 7
	glVertex3f(farV3.x, farV3.y, farV3.z);
	glVertex3f(nearV4.x, nearV4.y, nearV4.z); //line 8
	glVertex3f(farV4.x, farV4.y, farV4.z);
	glVertex3f(farV1.x, farV1.y, farV1.z); // line 9
	glVertex3f(farV2.x, farV2.y, farV2.z);
	glVertex3f(farV2.x, farV2.y, farV2.z); // line 10
	glVertex3f(farV3.x, farV3.y, farV3.z);
	glVertex3f(farV3.x, farV3.y, farV3.z); // line 11
	glVertex3f(farV4.x, farV4.y, farV4.z);
	glVertex3f(farV4.x, farV4.y, farV4.z); // line 12
	glVertex3f(farV1.x, farV1.y, farV1.z);
	glVertex3f(cameraPosition.x, cameraPosition.y, cameraPosition.z); //camera ray
	glVertex3f((-5.0f * camPosNorm.x), (-5.0f * camPosNorm.y), (-5.0f * camPosNorm.z));
	glEnd();

	glLoadMatrixf(glm::value_ptr(modelView));

}

// Computes the geometry needed for the spheres
bool Frustum::Initialize()
{
		GLUquadric * q = gluNewQuadric();
		if (q != NULL)
		{
			if ((this->sphereHandle = glGenLists(1)) == 0)
			{
				std::cout << "Model::Initialize() - Failed to GenLists()" << std::endl;
				return false;
			}
			glNewList(this->sphereHandle, GL_COMPILE);
			gluQuadricNormals(q, GLU_FLAT);
			gluSphere(q, 1.0, 20, 20);
			glEndList();
			gluDeleteQuadric(q);
			this->is_initialized = true;
		}
		else
		{
			return false;
		}
	return true;
}

// Takes a vector and transforms it by multipling by inverse projection matrix
// and the inverse modelview matrix to regain it's world coordinates
glm::vec3 Frustum::getWorldCoords(glm::vec3 vector){
	// Get the inverse modelview matrix and inverse projection matrix
	glm::mat4 modelView(1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));
	glm::mat4 invModelView = glm::inverse(modelView);
	glm::mat4 projection(1.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
	glm::mat4 invProjection = glm::inverse(projection);
	
	//'Undo' the transformations
	glm::vec4 tmp = glm::vec4(vector, 1.0f);
	tmp = invModelView * invProjection * tmp;
	tmp = tmp / tmp.w;

	//Setup the return vector to return a vec3
	vector.x = tmp.x;
	vector.y = tmp.y;
	vector.z = tmp.z;
	
	return vector;
}
