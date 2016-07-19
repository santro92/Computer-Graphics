/* Time Taken = approx 4 hrs */
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#define GL_GLEXT_PROTOTYPES //  OpenGL with prototypes for glext
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*  Lorenz Parameters  */
int n = 50000;
double s  = 16;
double b  = 4;
double r  = 45;
double dt = 0.001;

extern int th;       // Azimuth of view angle
extern int ph;       // Elevation of view angle
double w=1;

void display()
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //  Clear screen and Z buffer
   glLoadIdentity(); //  Reset transformations
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScaled(0.01,0.01,0.01);

   axes();
   //  Draw Lorenz
   glColor3d((double)rand() / RAND_MAX,(double)rand() / RAND_MAX,(double)rand() / RAND_MAX);
   glBegin(GL_LINE_STRIP);
   int i;
   double x=-13;
   double y=-12;
   double z=52;
   for(i=0; i<n; i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
  	   glVertex4f(x,y,z,w); 
   }
   glEnd();

   glColor3f(1, 1, 1);
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d",th,ph);
   glWindowPos2i(5,25);
   Print("Lorenz Parameter -> r=%0.1f, s=%0.1f, b=%0.1f, dt=%0.3f",r,s,b,dt);

   ErrCheck("display");
   glFlush();//  Make scene visible
   glutSwapBuffers();
}

void key(unsigned char ch,int x,int y)
{
   switch(ch)
   {
      case 27:
         exit(0);
      case '0':
         th = ph = 0;
         s = 10;
         b = 2.666666;
         r = 28;
         break;
      case 'R':
         r -= 1;
         break;
      case 'r':
         r += 1;
         break;
      case 'S':
         s -= 1;
         break;
      case 's':
         s += 1;
         break;
      case 'B':
         b -= 0.1;
         break;
      case 'b':
         b += 0.1;
         break;
      case 'T':
         dt -= 0.001;
         break;
      case 't':
         dt += 0.001;
         break;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

int main(int argc, char *argv[])
{
   glutInit(&argc,argv); //  Initialize GLUT
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  //  Request double buffered true color window
   glutInitWindowSize(500,500); //  Request 500 x 500 pixel window
   glutCreateWindow("HW1 - Santhanakrishnan Ramani"); //  Create window
   glutDisplayFunc(display); // Register function used to display scene
   glutReshapeFunc(reshape); // Register function used to reshape window
   glutSpecialFunc(special); // Register function used for key callbacks
   glutKeyboardFunc(key); //  Pass control to GLUT so it can interact with the user
   glutMainLoop();  //  Pass control to GLUT for events
   return 0; //  Return to OS
}