//Developed By: Emanuel Rosu & Erik Harvey
//Demonstration of modelling and camera manipulation,
//using a blend of legacy and modern GL techniques.

#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Camera.h"
#include "Frustum.h"
#include "OrthoSphere.h"
#include "Globals.h"

using namespace glm;
using namespace std;




#define		BAD_GL_VALUE				GLuint(-1)
float		k_diffuseLightParams[]	=	{1.0f, 1.0f,  1.0f, 1.0f};
float		k_ambientLightParams[]	=	{0.0f, 0.0f,  0.0f, 1.0f};
float		k_specularLightParams[]	=	{1.0f, 1.0f, 1.0f, 1.0f};
float		k_lightPos[]			=	{1.0f, 1.0f, 0.0f, 0.0f};
Globals		global;

void DrawDebugLines() {
	if (!global._debugWorldLines) {
		return;
	}
	// make sure lights and fog not affected by lighting and fog
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	
	// draw planes to give world context
	glBegin(GL_LINES);
	for(int i = 0; i <= 5; i += 1) {
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, (float)i);
		glVertex3f(5.0f, 0.0f, (float)i);
		glVertex3f((float)i, 0.0f, 0.0f);
		glVertex3f((float)i, 0.0f, 5.0f);
	};
	glEnd();
	glBegin(GL_LINES);
	for(int i = 0; i <= 5; i += 1) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, (float)i);
		glVertex3f(0.0f, 5.0f, (float)i);
		glVertex3f(0.0f, (float)i, 0.0f);
		glVertex3f(0.0f, (float)i, 5.0f);
	};
	glEnd();
	glBegin(GL_LINES);
	for(int i = 0; i <= 5; i += 1) {
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f((float)i, 0.0f, 0.0f);
		glVertex3f((float)i, 5.0f, 0.0f);
		glVertex3f(0.0f, (float)i, 0.0f);
		glVertex3f(5.0f, (float)i, 0.0f);
	};
	glEnd();

	// re-enable lighting to current states
	if (global._lighting) {
		glEnable(GL_LIGHTING);
	}
	if (global._fog) {
		glEnable(GL_FOG);
	}
}

// Function to write text to screen in "gui" space (close to camera)
// Pass in position in viewport, size of text, text to write, and viewport bounds
void WriteToScreen(vec3 pos, float size, char *text, vec4 viewPortBounds) {
	// make sure lighting and fog don't affect text
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
    
	char *p;
	//set all text to one color
	glColor3f(1.0f, 0.1f, 0.6f);
	
	//set up an orthographic matrix for projection
	mat4 prj = ortho(viewPortBounds.x, viewPortBounds.y, viewPortBounds.z, viewPortBounds.w, global._nearPlane, global._farPlane + 1000.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(prj));
	
	glMatrixMode(GL_MODELVIEW);
    mat4 modelView(1.0f);
	mat4 textView(1.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, value_ptr(modelView));
	pos = vec3(pos.x, pos.y, pos.z);
	
	// get model view matrix, set text position to passed in position - you'll generally want the z position to be 0, as you want to display in 
	// front of the camera
	
	textView = translate(modelView, pos);
	// normalize text size - apparently glutStrokeCharacter will normally render text to large sizes
	textView = scale(textView, vec3(size/152.0f, size/152.0f, size/152.0f));
	glLoadMatrixf(value_ptr(textView));
    for (p = text; *p; p++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
    glLoadMatrixf(value_ptr(modelView));
	
	// reset lighting and fog to current display values
	if (global._lighting) {
		glEnable(GL_LIGHTING);
	}
	if (global._fog) {
		glEnable(GL_FOG);
	}
}

void DisplayFunctionWindowOne()
{
	glClearColor(0.25f, 0.25f, 0.50f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, (global._wireframe == true) ? GL_LINE : GL_FILL);
	glViewport(0, 0, global._windowOneSize.x, global._windowOneSize.y);
	 
	// write text before changing projection and mv matrices
	float textSize = 25.0f;
	float textPosY = 10.0f;
	float textPosX = 10.0f;
	vec4 viewPortBounds(0.0f, (float)global._windowOneSize.x, 0.0f, (float)global._windowOneSize.y);
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "w - toggle wireframe; d - toggle debug lines (world)", viewPortBounds);
	WriteToScreen(vec3(textPosX, textPosY += textSize, 0.0f), textSize, "f - fog; l - lighting", viewPortBounds);
	WriteToScreen(vec3(textPosX, textPosY += textSize, 0.0f), textSize, "Up/Down/Left/Right; Page Up/Page Down", viewPortBounds);
	
	textPosY += textSize;
	textSize = 40.0f;
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "PERSPECTIVE VIEW", viewPortBounds);

	// create perspective projection for spaceships
	mat4 prj = perspective(global.fov, float(global._windowOneSize.x) / float(global._windowOneSize.y), global._nearPlane, global._farPlane);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(prj));
	glMatrixMode(GL_MODELVIEW);

	// get camera matrix from camera
	glLoadMatrixf(value_ptr(global.camera.getCameraMatrix()));
	
	// draw world context planes (function will check for them being turned on/off);
	DrawDebugLines();

	for (vector<Model *>::iterator iter = global._ships.begin(); iter < global._ships.end(); iter++)
		(*iter)->Draw();
	
	// update the frustrum
	global.frust.updateVectors(global.frust, global.camera);
	glutSwapBuffers();
}


void DisplayFunctionWindowTwo()
{
	glClearColor(0.25f, 0.25f, 0.50f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, (global._wireframe == true) ? GL_LINE : GL_FILL);
	
	// get second window's aspect ratio
	float aspect = (global._windowTwoSize.x / 2.0f) / (global._windowTwoSize.y / 2.0f);

	// draw left viewport contents
	glViewport(0, (int)(global._windowTwoSize.y / 2.5f), global._windowTwoSize.x / 2, global._windowTwoSize.y / 2);
	
	// set orthographic projection matrix
	mat4 prj = ortho(aspect * -10.0f, aspect * 10.0f, -10.0f, 10.0f, global._nearPlane, global._farPlane + 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(prj));

	glMatrixMode(GL_MODELVIEW);

	//easy way to get desired view: set camera lookat point
	mat4 lookat = glm::lookAt(vec3(-10, 0, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	glLoadMatrixf(value_ptr(lookat));
	DrawDebugLines();
	for (vector<Model *>::iterator iter = global._ships.begin(); iter < global._ships.end(); iter++)
		(*iter)->Draw();
	
	// wrap frustrum/sphere drawing in light/fog safe statements
	if (global._lighting == true){
		glDisable(GL_LIGHTING);
	}
	if (global._fog == true){
		glDisable(GL_FOG);
	}
	
	global.frust.Draw();
	global.oSphere.Draw2();
	
	if (global._lighting == true){
		glEnable(GL_LIGHTING);
	}
	if (global._fog == true){
		glEnable(GL_FOG);
	}

	// draw right viewport contents
	glViewport(global._windowTwoSize.x/2, (int)(global._windowTwoSize.y / 2.5f), global._windowTwoSize.x / 2, global._windowTwoSize.y / 2);
	mat4 prj2 = ortho(aspect * -10.0f, aspect * 10.0f, -10.0f, 10.0f, global._nearPlane, global._farPlane * 100);
	// keeping same lookat matrix as before, rotate view to look desired second axis rotations - MAKE SURE TO UNDO THIS ROTATION
	lookat = rotate(lookat, 270.0f, vec3(0, 1, 0));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(prj2));
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(lookat));
	DrawDebugLines();
	for (vector<Model *>::iterator iter = global._ships.begin(); iter < global._ships.end(); iter++)
		(*iter)->Draw();
	
	if (global._lighting == true){
		glDisable(GL_LIGHTING);
	}
	if (global._fog == true){
		glDisable(GL_FOG);
	}
	global.frust.Draw();
	global.oSphere.Draw();
	if (global._lighting == true){
		glEnable(GL_LIGHTING);
	}
	if (global._fog == true){
		glEnable(GL_FOG);
	}

	// reset rotation on matrix for writing text
	lookat = rotate(lookat, -270.0f, vec3(0, 1, 0));
	glLoadMatrixf(value_ptr(lookat));

	// create new viewports for writing text to screen in bottom of second window
	glViewport(0, 0, global._windowTwoSize.x / 2, (int)(global._windowTwoSize.y / 2.5));
	vec4 viewPortBounds(0.0f, global._windowTwoSize.x / 2.0f, 0.0f, global._windowTwoSize.y / 2.0f);
	float textSize = 25.0f;
	float textPosY = 10.0f;
	float textPosX = 10.0f;
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "w - toggle wireframe; d - toggle debug lines (world)", viewPortBounds);
	WriteToScreen(vec3(textPosX, textPosY += textSize, 0.0f), textSize, "f - fog; l - lighting", viewPortBounds);
	WriteToScreen(vec3(textPosX, textPosY += textSize, 0.0f), textSize, "Up/Down/Left/Right; Page Up/Page Down", viewPortBounds);
	
	textPosY += textSize;
	textSize = 40.0f;
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "THIRD PERSON VIEW", viewPortBounds);
	textPosY += textSize * 2;
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "X-AXIS VIEW", viewPortBounds);

	glViewport(global._windowTwoSize.x/2, 0, global._windowTwoSize.x / 2, (int)(global._windowTwoSize.y / 2.5f));
	WriteToScreen(vec3(textPosX, textPosY, 0.0f), textSize, "Z-AXIS VIEW", viewPortBounds);
	glutSwapBuffers();
}


void ReshapeFuncWindowOne(int w, int h)
{
	if (global.window_closed == false && h > 0)
	{
		global._windowOneSize = ivec2(w, h);
		glutPostWindowRedisplay(global._windowOneHandle);
	}
}

void ReshapeFuncWindowTwo(int w, int h)
{
	if (global.window_closed == false && h > 0) {
		global._windowTwoSize = ivec2(w, h);
		glutPostWindowRedisplay(global._windowTwoHandle);
	}
}

void CloseFunc()
{
	glutLeaveMainLoop();

	while (global._ships.size() > 0)
	{
		delete *(global._ships.end() - 1);
		global._ships.pop_back();
	}
}

void NonStandardInputHandler (int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			//up movement
			global.camera.incHorzAngle(global.camera, 1);
			break;
		case GLUT_KEY_DOWN:
			//down movement
			global.camera.incHorzAngle(global.camera, -1);
			return;
		case GLUT_KEY_LEFT:
			//left movement
			global.camera.incVertAngle(global.camera, -1);
			return;
		case GLUT_KEY_RIGHT:
			//right movement
			global.camera.incVertAngle(global.camera, 1);
			return;
		case GLUT_KEY_PAGE_UP:
			//Increase FOV
			if (global.fov <= 79.0f){
				global.fov++;
			}
			return;
		case GLUT_KEY_PAGE_DOWN:
			//Decrease FOV
			if (global.fov >= 11.0f){
				global.fov--;
			}
			return;
	}
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'f':
		global._fog = !global._fog;
		if (global._fog) {
			glDisable(GL_FOG);
		} else {
			glEnable(GL_FOG);
		}
		break;
	case 'd': 
		global._debugWorldLines = !global._debugWorldLines;
		break;
	case 'w':
		global._wireframe = !global._wireframe;
		break;

	case 27:
	case 'x':
		CloseFunc();
		return;
	
	case 'l':
		global._lighting = !global._lighting;
		if (global._lighting) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}
		return;
	}
}

void TimerFunc(int value)
{
	if (global.window_closed == false)
	{
		glutTimerFunc(global.period, TimerFunc, 0);
		glutPostWindowRedisplay(global._windowOneHandle);
		glutPostWindowRedisplay(global._windowTwoHandle);
		
	}
}

// Must be called when GL initialized!
bool InitializeGL()
{
	for (int i = 0; i < global._numShips / 4; i++)
	{
		for (int j = 0; j < 4; j++) {
			vec3 _startPos;
			_startPos = vec3(-4.5f + 3.0f * i, 2.0f, -4.5 + 3.0f * j);
			

			Model * s = new Model(vec3(1.0f, 0.0f, 0.0f), _startPos);
			if (!s->Initialize())
				return false;
			global._ships.push_back(s);
		}
	}
	global.frust = Frustum(global.camera);
	global.frust.Initialize();
	global.oSphere = OrthoSphere();
	global.oSphere.Initialize();
	return true;
}

int main(int argc, char * argv[])
{
	glutInit(&argc , argv);
	global = Globals();
	glutSetOption ( GLUT_RENDERING_CONTEXT , GLUT_USE_CURRENT_CONTEXT ); // Allows sharing of ships between windows (same context)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(global._windowOneSize.x, global._windowOneSize.y);
	global._windowOneHandle = glutCreateWindow("First Person View");
	glEnable(GL_DEPTH_TEST); // THIS MUST BE INCLUDED FOR PROPER DEPTH HANDLING
	
	glEnable (GL_LIGHTING ); // Set up lighting parameters, legacy GL
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT , k_ambientLightParams);
		glLightfv(GL_LIGHT0, GL_SPECULAR, k_specularLightParams);
		glLightfv(GL_LIGHT0, GL_DIFFUSE , k_diffuseLightParams);
		glLightfv(GL_LIGHT0, GL_POSITION, k_lightPos);
		glEnable (GL_LIGHT0 ); // enabling one light
	}
	
	glEnable (GL_COLOR_MATERIAL); // Use color materials, so color affects lighting
	
	glEnable(GL_FOG); // Set up fog parameters, legacy GL
	{
      GLfloat fogColor[4] = {0.25f, 0.25f, 0.50f, 1.0f}; // Set fog color to window background color, for more tasteful appearance
	  glFogi (GL_FOG_MODE, GL_EXP2);
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, 0.10f);
	  glHint (GL_FOG_HINT, GL_NICEST); // Default to highest quality
      glFogf (GL_FOG_START, 1.0f);
      glFogf (GL_FOG_END, 10.0f);
	}

	global.window_closed = false;
	glutInitWindowSize(global._windowTwoSize.x, global._windowTwoSize.y);
	glutDisplayFunc(DisplayFunctionWindowOne);
	glutReshapeFunc(ReshapeFuncWindowOne);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(NonStandardInputHandler);
	glutCloseFunc(CloseFunc);
	
	if (!InitializeGL())
	{
		CloseFunc();
		return -1;
	}

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	global._windowTwoHandle = glutCreateWindow("Third Person View");
	global.window_closed = false;
	
	// make sure we are setting separate display and reshape functions though - trying to draw different things, and 
	// reshape windows independently of one another
	glutDisplayFunc(DisplayFunctionWindowTwo);
	glutReshapeFunc(ReshapeFuncWindowTwo);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(NonStandardInputHandler);
	glutCloseFunc(CloseFunc);
	
	if (!InitializeGL())
	{
		CloseFunc();
		return -1;
	}

	glutTimerFunc(global.period, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();
	return 0;
}
