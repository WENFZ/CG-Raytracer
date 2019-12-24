#include <Windows.h>
//#include <gl/glut.h>
#include "light.h"
#include "glut.h"
#include "glCanvas.h"
#include "scene_parser.h"
#include "light.h"
#include "camera.h"
#include "group.h"
#include "rayTree.h"
// Included files for OpenGL Rendering


// ========================================================

// STATIC VARIABLES

// These function will get called from the 'keyboard' routine
void (*GLCanvas::renderFunction)(void);
void (*GLCanvas::traceRayFunction)(float,float);

// A pointer to the global SceneParser
SceneParser *GLCanvas::scene;

// State of the mouse cursor
int GLCanvas::mouseButton;
int GLCanvas::mouseX;
int GLCanvas::mouseY;

// ========================================================
// ========================================================

#ifdef SPECULAR_FIX
// global variable allows (hacky) communication with Material::setMaterial()
int SPECULAR_FIX_WHICH_PASS = 0;
#endif


// ========================================================
// Draw the x-, y-, and z-axes with lighting disabled
// ========================================================

void GLCanvas::drawAxes(void)
{
  glDisable(GL_LIGHTING);
  // Draw the x-axis in red
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(100,0,0);
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(1.0, 0.0,  0.0 );
    glVertex3f(0.8, 0.07, 0.0 );
    glVertex3f(0.8, 0.0,  0.07);
    glVertex3f(0.8,-0.07, 0.0 );
    glVertex3f(0.8, 0.0, -0.07);
    glVertex3f(0.8, 0.07, 0.0 );
  glEnd();

  // Draw the y-axis in green
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,100,0);
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f( 0.0,  1.0, 0.0 );
    glVertex3f( 0.07, 0.8, 0.0 );
    glVertex3f( 0.0,  0.8, 0.07);
    glVertex3f(-0.07, 0.8, 0.0 );
    glVertex3f( 0.0,  0.8,-0.07);
    glVertex3f( 0.07, 0.8, 0.0 );
  glEnd();

  // Draw the z-axis in blue
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,100);
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f( 0.0,  0.0,  1.0);
    glVertex3f( 0.07, 0.0,  0.8);
    glVertex3f( 0.0,  0.07, 0.8);
    glVertex3f(-0.07, 0.0,  0.8);
    glVertex3f( 0.0, -0.07, 0.8);
    glVertex3f( 0.07, 0.0,  0.8);
  glEnd();
}

// ========================================================
// Callback for the OpenGL display loop.  To request that
// this function get called, use glutPostRedisplay()
// ========================================================

void GLCanvas::display(void)
{
  // Clear the display buffer
  Vec3f bgColor = scene->getBackgroundColor();
  glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the camera parameters
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  scene->getCamera()->glPlaceCamera();

  // ========================================================
  // DRAW AXES
  // remove this line once you've started rendering primitive objects
  drawAxes(); 
  // ========================================================

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  // Place each of the lights in the scene
  for (int i=0; i<scene->getNumLights(); i++) {
    scene->getLight(i)->glInit(i);
  }

#if !SPECULAR_FIX

  // DEFAULT: single pass rendering
  // Draw the scene once
  SPECULAR_FIX_WHICH_PASS = 0;
  scene->getGroup()->paint();

//  int sectorCount = 50;
//  int stackCount = 50;
//
//  float m_radius = 1;
//  GLfloat* pos = new GLfloat[(stackCount + 1) * (sectorCount + 1) * 3];
//  GLfloat* normals = new GLfloat[(stackCount + 1) * (sectorCount + 1) * 3];
//  float PI = 3.1415926;
//  float sectorStep = 2 * PI / sectorCount;
//  float stackStep = PI / stackCount;
//  float sectorAngle, stackAngle;
//  float x, y, z, nx, ny, nz, lengthInv = 1.0f / m_radius;
//  float xy;
//  for (int i = 0; i <= stackCount; ++i)
//  {
//	  stackAngle = 3.1415926 / 2 - i * stackStep;        // starting from pi/2 to -pi/2
//	  xy = m_radius * cosf(stackAngle);             // r * cos(u)
//	  z = m_radius * sinf(stackAngle);              // r * sin(u)
//
//	  // add (sectorCount+1) vertices per stack
//	  // the first and last vertices have same position and normal, but different tex coords
//	  for (int j = 0; j <= sectorCount; ++j)
//	  {
//		  sectorAngle = j * sectorStep;           // starting from 0 to 2pi
//
//		  // vertex position (x, y, z)
//		  x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
//		  y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
//
//
//		  pos[(i * sectorCount + j) * 3] = x;
//		  pos[(i * sectorCount + j) * 3 + 1] = y;
//		  pos[(i * sectorCount + j) * 3 + 2] = z;
//
//		  // normalized vertex normal (nx, ny, nz)
//		  nx = x * lengthInv;
//		  ny = y * lengthInv;
//		  nz = z * lengthInv;
//		  normals[(i * sectorCount + j) * 3] = x;
//		  normals[(i * sectorCount + j) * 3 + 1] = y;
//		  normals[(i * sectorCount + j) * 3 + 2] = z;
//}
//	}
//  std::vector<int> indices;
//  int k1, k2;
//  for (int i = 0; i < stackCount; ++i)
//  {
//	  k1 = i * (sectorCount + 1);     // beginning of current stack
//	  k2 = k1 + sectorCount + 1;      // beginning of next stack
//
//	  for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
//	  {
//		  // 2 triangles per sector excluding first and last stacks
//		  // k1 => k2 => k1+1
//		  if (i != 0)
//		  {
//			  indices.push_back(k1);
//			  indices.push_back(k2);
//			  indices.push_back(k1 + 1);
//		  }
//
//		  // k1+1 => k2 => k2+1
//		  if (i != (stackCount - 1))
//		  {
//			  indices.push_back(k1 + 1);
//			  indices.push_back(k2);
//			  indices.push_back(k2 + 1);
//		  }
//	  }
//  }
//  glColor3f(1,0,0);
//  glEnableClientState(GL_VERTEX_ARRAY);
//  glEnableClientState(GL_NORMAL_ARRAY);
//  glVertexPointer(3, GL_FLOAT, 0, pos);
//  glNormalPointer(GL_FLOAT, 0, normals);
//  glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());
//  glDisableClientState(GL_VERTEX_ARRAY);
//  glDisableClientState(GL_NORMAL_ARRAY);



#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight 
  // artifact for small specular exponents (wide specular lobe)

  // First pass, draw the specular highlights
  SPECULAR_FIX_WHICH_PASS = 0;
  scene->getGroup()->paint();
  
  glDepthFunc(GL_EQUAL);
  glEnable(GL_BLEND);

  // Second pass, multiply specular highlights by normal dot light
  SPECULAR_FIX_WHICH_PASS = 1;
  glBlendFunc(GL_DST_COLOR, GL_ZERO);
  scene->getGroup()->paint();
  
  // Third pass, add diffuse & ambient components
  SPECULAR_FIX_WHICH_PASS = 2;
  glBlendFunc(GL_ONE, GL_ONE);
  scene->getGroup()->paint();

  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);

#endif

  // Draw the ray tree
  glDisable(GL_LIGHTING);
  RayTree::paint();
  glEnable(GL_LIGHTING);

  // Swap the back buffer with the front buffer to display
  // the scene
  glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void GLCanvas::reshape(int w, int h) {
  // Set the OpenGL viewport to fill the entire window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  // Set the camera parameters to reflect the changes
  scene->getCamera()->glInit(w, h);

  // OPTIONAL: If you'd also like to resize your rendering, 
  // add code to do that here
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void GLCanvas::mouse(int button, int state, int x, int y) {
  // Save the current state of the mouse.  This will be
  // used by the 'motion' function
  mouseButton = button;
  mouseX = x;
  mouseY = y;
}

// ========================================================
// Callback function for mouse drag
// ========================================================

void GLCanvas::motion(int x, int y) {
  // Left button = rotation
  // (rotate camera around the up and horizontal vectors)
  if (mouseButton == GLUT_LEFT_BUTTON) {
    scene->getCamera()->rotateCamera(0.005*(mouseX-x), 0.005*(mouseY-y));
    mouseX = x;
    mouseY = y;
  }
  // Middle button = translation
  // (move camera perpendicular to the direction vector)
  else if (mouseButton == GLUT_MIDDLE_BUTTON) {
    scene->getCamera()->truckCamera((mouseX-x)*0.05, (y-mouseY)*0.05);
    mouseX = x;
    mouseY = y;
  }
  // Right button = zoom
  // (move camera along the direction vector)
  else if (mouseButton == GLUT_RIGHT_BUTTON) {
    scene->getCamera()->dollyCamera((x-mouseX)*0.05);
    mouseX = x;
    mouseY = y;
  }

  // Redraw the scene with the new camera parameters
  glutPostRedisplay();
}

// ========================================================
// Callback function for keyboard events
// ========================================================

void GLCanvas::keyboard(unsigned char key, int i, int j) {
  switch (key) {
  case 'r':  case 'R':
    printf("Rendering scene... "); 
    fflush(stdout);
    if (renderFunction) renderFunction();
    printf("done.\n");
    break;
  case 't':  case 'T': {
    // visualize the ray tree for the pixel at the current mouse position
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    // flip up & down
    j = height-j; 
    int max = (width > height) ? width : height;
    // map the pixel coordinates: (0,0) -> (width-1,height-1);
    //      to screenspace: (0.0,0.0) -> (1.0,1.0);
    float x = ((i + 0.5) -  width/2.0) / float(max) + 0.5;
    float y = ((j + 0.5) - height/2.0) / float(max) + 0.5;
    RayTree::Activate();
    if (traceRayFunction) traceRayFunction(x,y);
    RayTree::Deactivate();
    // redraw
    display();
    break; }
  case 'q':  case 'Q':
    exit(0);
    break;
  default:
    printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
  }
}

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void GLCanvas::initialize(SceneParser *_scene, void (*_renderFunction)(void), void (*_traceRayFunction)(float,float)) {
  scene = _scene;
  renderFunction = _renderFunction;
  traceRayFunction = _traceRayFunction;

  // Set global lighting parameters
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


  // Set window parameters
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glEnable(GL_DEPTH_TEST);
  // OPTIONAL: If you'd like to set the window size from 
  // the command line, do that here
  glutInitWindowSize(200,200);
  glutInitWindowPosition(100,100);
  glutCreateWindow("OpenGL Viewer");

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_NORMALIZE);

  // Ambient light
  Vec3f ambColor = scene->getAmbientLight();
  GLfloat ambArr[] = { ambColor.x(), ambColor.y(), ambColor.z(), 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

  // Initialize callback functions
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Enter the main rendering loop
  glutMainLoop();
}

// ========================================================
// ========================================================

