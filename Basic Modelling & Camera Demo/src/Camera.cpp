#pragma once
#include "Camera.h"

using namespace std;




Camera::Camera(float radius, int horizontalAngle, int verticalAngle){
	this->rad = radius;
	//Set angles for instance and set up angles in radians.
	this->horzAngle = horizontalAngle;
	this->vertAngle = verticalAngle;
	updateRadians();
	// This next line creates two vectors, the one to the right of the camera,
	// and the camera's up.
	updatePosVec();
}

// Converts a horizontal angle and a vertical angle measured in degrees into
// radians.
void Camera::updateRadians(){
	this->horzRad = this->horzAngle * 3.1415926535896f / 180.0f;
	this->vertRad = this->vertAngle * 3.1415926535896f / 180.0f;
}

//	Updates the camera's position based on the spherical coordinates.
//  It also sets the directional vectors.
void Camera::updatePosVec(){
	// Camera's position is translation from spherical to Cartesean coordinates
	this->cameraPos = glm::vec3(rad * sinf(vertRad) * cosf(horzRad), 
		rad * sinf(horzRad), 
		rad * cosf(horzRad) * cosf(vertRad));
	// To point to the origin, the direction is position multiplied by -1 so it
	// points toward the origin.
	this->cameraDir = -1.0f * cameraPos;
	// The camera's right is a vector that is orthogonal to the direction, but
	// does not interact with the y-plane (so it is always horizontal).
	this->cameraRgt = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDir); 
	// The camera's up is then the vector orthogonal to the direction and the
	// right of the camera.
	this->cameraUp = glm::cross(cameraDir, cameraRgt);
}

// This returns a matrix that can be used as the modelview matrix based on the
// camera's current position and orientation.
glm::mat4 Camera::getCameraMatrix(){
		updateRadians();
		updatePosVec();
		return glm::lookAt(this->cameraPos, this->cameraDir, this->cameraUp);
}

glm::vec3 Camera::getCameraPosition(Camera &cam) {
	return cam.cameraPos;
}

// Change the horizontal angle by the amount passed. Decrease the angle by
// passing a negative value. The horizontal angle is limited to 
//  ( -89 =< x =< 89)
void Camera::incHorzAngle(Camera &cam, int amount){
	if ((cam.horzAngle + amount) <= 89 && (cam.horzAngle + amount) >= -89){
		cam.horzAngle += amount;
	}
}


// Change the vertical angle by the amount passed. Decrease the angle by
// passing a negative value. The vertical angle is limited to 
// 0 <= x <=359. This is done to avoid the possiblity of creating impossibly
// large numbers to deal with.
void Camera::incVertAngle(Camera &cam, int amount){
	if (cam.vertAngle + amount <= 0){
		cam.vertAngle = cam.vertAngle + 360 + amount;
	} else if (cam.vertAngle + amount >= 360){
		cam.vertAngle = cam.vertAngle - 360 + amount;
	} else {
		cam.vertAngle += amount;
	}
}
