   /* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: main.cpp
Purpose: Entry point for the program.  Houses some OpenGL extension intialization.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#include "Renderer.h"

int main_window; //The id of the GLUT window. used by GLUI
bool mouse_down = false;
Renderer *renderer = Renderer::GetSingleton();

void render()
{
  renderer->RenderScene();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case VK_ESCAPE:
    renderer->LoadNewMesh();
    break;
  case 'r':
    renderer->recompile_shaders();
    break;
  case 'R':
    renderer->recompile_shaders();
    break;
  case 's':
    renderer->scale_change();
    break;
  }
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	  case GLUT_LEFT_BUTTON:
      mouse_down = !mouse_down;
      if(mouse_down)
      {
        renderer->MouseClicked(x, y);
      }
      else
      {
        renderer->MouseReleased(x, y);
      }
		  break;
	}
}

void mouseMove(int x, int y)
{
  renderer->MouseMoved(x, y);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, Z_NEAR, Z_FAR);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
  glutSetWindow (main_window);
  glutPostRedisplay ();
  Sleep (10);
}


int main(int argc, char **argv) 
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);

	//Keeps track of the Window for GLUI
	main_window = glutCreateWindow("CS300");
	GLenum err = glewInit();
  if(err!=GLEW_OK)
  {
    std::cout << "Error in glewInit" << std::endl;
  } 
	//Creating a renderer class
  // register callbacks
  renderer->InitializeGluiWindow(main_window);
  glutDisplayFunc(render);
  glutIdleFunc(idle);
  glutReshapeFunc(changeSize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
  glEnable(GL_DEPTH_TEST);

  // enter GLUT event processing cycle
	//atexit(cleanup);
  glutMainLoop();

	return 1;

}