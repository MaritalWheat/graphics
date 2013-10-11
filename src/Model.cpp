#include <iostream>
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;




GLuint Model::_sphereHandle		= BAD_GL_VALUE;
GLuint Model::_cylinderHandle	= BAD_GL_VALUE;
GLuint Model::_coneHandle		= BAD_GL_VALUE;
int Model::_slices				= 20;
int Model::_stacks				= 20;
bool Model::is_initialized		 = false;

Model::Model(vec3 & _color, vec3 & _position)
{
	this->_color = _color;
	this->_position = _position;
	this->_rotation = 270;
}

// Axes drawer, can be called to draw axes at origin of current matrix context - helped debugging when building first spaceship
// component by componenet
void Model::DrawDebugLines() {
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
	glColor3fv(value_ptr(this->_color));
}

void Model::Draw()
{
	glColor3fv(value_ptr(this->_color));
	mat4 body(1.0f);
	mat4 modelView(1.0f);
	// This is how we pick up the previous and compound.
	glGetFloatv(GL_MODELVIEW_MATRIX, value_ptr(modelView));
	
	body = translate(modelView, this->_position);
	body = rotate(body, this->_rotation, vec3(1.0f, 0.0f, 0.0f));
	body = scale(body, vec3(0.5f, 0.5f, 2.0f));
	glLoadMatrixf(value_ptr(body));
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glCallList(this->_sphereHandle);
	// DrawDebugLines(); - don't want these showing up anymore, useful for drawing first model and seeing components' spatial
	// relationships to each other
	
	//reset scale so fins aren't skewed
	body = scale(body, vec3(2.0f, 2.0f, 0.5f));
	
	//create fins
	float angle;
	for (angle = 0.0f; angle <= 360.0f; angle += 90.0f) {
		mat4 fin (1.0f);
		fin = body; //get spaceship space
		
		// printf("Angle: %f/\n", angle); - wanted to make sure visuals matched the numbers I was getting for current angle
		
		fin = translate(fin, vec3(0.0f, 0.0f, -1.25f));
		fin = rotate (fin, 90.0f, vec3(1.0f, 0.0f, 0.0f));
		fin = rotate (fin, angle, vec3(0.0f, 90.0f, 0.0f));
		fin = scale(fin, vec3(.10f, .25f, .75f));
		if (angle < 90) glColor3f(1.0f, 1.0f, 1.0f);
		else if (angle < 180) glColor3f(1.0f, 0.0f, 0.0f);
		else if (angle < 270) glColor3f(0.0f, 1.0f, 0.0f);
		else if (angle < 360) glColor3f(0.0f, 0.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glLoadMatrixf(value_ptr(fin));
		glCallList(this->_coneHandle);
		//DrawDebugLines();
		
		fin = scale(fin, vec3(1.0f / 0.10f, 1.0f / 0.25f, 1.0f / 0.75f));
		mat4 tube (1.0f);
		tube = fin;
		tube = translate(tube, vec3(0.0f, -0.5f, .75f));
		tube = rotate(tube, -90.0f, vec3(1.0f, 0.0f, 0.0f));
		tube = scale(tube, vec3(0.20f, 0.20f, 1.0f));
		glColor3f(1.0f, 1.0f, 0.25f);
		glLoadMatrixf(value_ptr(tube));
		glCallList(this->_cylinderHandle);
		//DrawDebugLines();
		
		tube = scale(tube, vec3(1.0f / 0.20f, 1.0f / 0.20f, 1.0f));
		int num_fin_caps = 2;
		for (int num_fin_caps = 0; num_fin_caps <= 1; num_fin_caps++) {
			mat4 cap (1.0f);
			cap = tube;
			glColor3f(0.25f, 0.25f, 1.0f);
			cap = translate(cap, vec3(0.0f, 0.0f, num_fin_caps));
			cap = scale(cap, vec3(0.20f, 0.20f, 0.20f));
			glLoadMatrixf(value_ptr(cap));
			glCallList(this->_sphereHandle);
		}
	}
	glLoadMatrixf(value_ptr(modelView));
}


bool Model::Initialize()
{
	if (this->_sphereHandle == BAD_GL_VALUE)
	{
		GLUquadric * q = gluNewQuadric();
		if (q != NULL)
		{
			if ((this->_sphereHandle = glGenLists(1)) == 0)
			{
				cout << "Model::Initialize() - Failed to GenLists()" << endl;
				return false;
			}
			glNewList(this->_sphereHandle, GL_COMPILE);
			//gluQuadricNormals(q, GLU_FLAT); - had to change to GLU_SMOOTH for better shading
			gluQuadricNormals(q, GLU_SMOOTH);
			gluSphere(q, 1.0, this->_slices, this->_stacks);
			glEndList();

			gluDeleteQuadric(q);
			this->is_initialized = true;
		}
		else
		{
			return false;
		}
	}

	if (this->_cylinderHandle == BAD_GL_VALUE)
	{
		GLUquadric * q = gluNewQuadric();
		if (q != NULL)
		{

			if ((this->_cylinderHandle = glGenLists(1)) == 0)
			{
				cout << "Model::Initialize() - Failed to GenLists()" << endl;
				return false;
			}
			glNewList(this->_cylinderHandle, GL_COMPILE);
			gluQuadricNormals(q, GLU_SMOOTH);
			gluCylinder(q, 1.0, 1.0, 1.0, this->_slices, this->_stacks);
			glEndList();

			gluDeleteQuadric(q);
			this->is_initialized = true;
		}
		else
		{
			return false;
		}
	}

	if (this->_coneHandle == BAD_GL_VALUE)
	{
		GLUquadric * q = gluNewQuadric();
		if (q != NULL)
		{

			if ((this->_coneHandle = glGenLists(1)) == 0)
			{
				cout << "Model::Initialize() - Failed to GenLists()" << endl;
				return false;
			}
			glNewList(this->_coneHandle, GL_COMPILE);
			gluQuadricNormals(q, GLU_SMOOTH);
			gluCylinder(q, 1.0, 0.65, 1.0, this->_slices, this->_stacks);
			glEndList();

			gluDeleteQuadric(q);
			this->is_initialized = true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

