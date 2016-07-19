/* Time Taken = approx 10 hrs */
#include "CSCIx229.h"

int th = 0;       // Azimuth of view angle
int ph = 0;       // Elevation of view angle
int mode = 1;     // To change projection mode 
int fov = 55;       //  Field of view (for perspective and fps)
int axes = 0;       //  Display axes
int fps = 0;        // To set FPS mode
double w = 1;       // Homogenous Coordinate       
double dim = 5.0;   //  Size of world
double asp = 1;     //  Aspect ratio
double len = 2.5; // Length of Axis

double EX = 0; // x-coordinate of camera position
double EY = 0; // y-coordinate of camera position
double EZ = 10; // z-coordinate of camera position

double AX = 0; // x-coordinate of where the camera is looking
double AY = 0; // y-coordinate of where the camera is looking
double AZ = 0; // z-coordinate of where the camera is looking

double UX = 0; // x-coordinate of the up vector
double UY = 1; // y-coordinate of the up vector
double UZ = 0; // z-coordinate of the up vector

static void Vertex(double th,double ph)
{
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

static void sphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Latitude bands
   glBegin(GL_QUAD_STRIP);
   glColor4f(1,1,1,1);
   for (ph=-90;ph<90;ph+=d)
   {   
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void cone(double x, double y, double z, double r, double h, double clr[])
{
   int i;
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(r,r,h);
   // Cone Bottom
   glBegin(GL_POLYGON);
   glColor4f(clr[0], clr[1], clr[2], clr[3]);
   for(i = 0; i <= 360; i += 15) 
   {
      glVertex3f(Cos(i), Sin(i), 0.0);
   }
   glEnd();
   // Cone Top
   glBegin(GL_TRIANGLES);
   for(i = 0; i <= 360; i += 15) 
   {
      glVertex3f(Cos(i), Sin(i), 0);
      glVertex3f(0, 0, 1);
      glVertex3f(Cos(i+15), Sin(i+15), 0);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void cylinder(double x, double y, double z, double r, double h)
{
   int i;
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(r,r,h);
   // Cylinder Bottom
   glBegin(GL_POLYGON);
   glColor4f(0.5, 0.35, 0.05, 1.0);
   for(i = 0; i <= 360; i += 15) 
   {
      glVertex3f(Cos(i), Sin(i), 0.0);
   }
   glEnd();
   // Cylinder Top
   glBegin(GL_POLYGON);
   for(i = 0; i <= 360; i += 15) 
   {
      glVertex3f(Cos(i), Sin(i), 2);
   }
   glEnd();
   // Cylinder "Cover"
   glBegin(GL_QUAD_STRIP);
   for(i = 0; i <=360; i += 15) 
   {
      glVertex3f(Cos(i), Sin(i), 0.0);
      glVertex3f(Cos(i), Sin(i), 2);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void cube(double x,double y,double z, double dx,double dy,double dz, double clr[])
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //cube
   glBegin(GL_QUADS);
   glColor3f(clr[0],clr[1],clr[2]);
   //  Front
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();   
}

static void house(double x,double y,double z, double dx,double dy,double dz)
{
   int i;
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //house 
   double clr[3] = {0.69,0.71,0.47};
   cube(x,y,z,dx,dy,dz,clr);
   //Road
   double clr1[3] = {0.2,0.2,0.2};
   cube(0,-3,-1.1,4,1.5,0.1,clr1);
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //  Door
   glBegin(GL_QUADS);
   glColor3f(0.3,0.15,0.2);
   glVertex3f(-0.8,-1,-1);
   glVertex3f(-0.2,-1,-1);
   glVertex3f(-0.2,-1,0);
   glVertex3f(-0.8,-1,0);
   glEnd();
   //  Window
   glBegin(GL_QUADS);
   glColor3f(0.5,0.5,1);
   glVertex3f(0.3,-1,-0.5);
   glVertex3f(0.7,-1,-0.5);
   glVertex3f(0.7,-1,0);
   glVertex3f(0.3,-1,0);
   glEnd();
   //  Stripes on Road
   glBegin(GL_QUADS);
   glColor3f(1,1,1);   
   for(i=-3;i<=3;i++)
   {
      glVertex3f(i,-3.3,-1);
      glVertex3f(i+0.4,-3.3,-1);
      glVertex3f(i+0.4,-3,-1);
      glVertex3f(i,-3,-1);
   }
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

static void star(double x,double y,double z, double dx,double dy,double dz)
{
    //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   glColor3f(1,1,1);
   glBegin(GL_TRIANGLES);  
   glVertex3f (-0.8,0,0);
   glVertex3f (0,0,1); 
   glVertex3f (0.8,0,0);
   glVertex3f (0.8,0,0);
   glVertex3f (2,0,0);
   glVertex3f (1.1,0,-1);
   glVertex3f (1.1,0,-1);
   glVertex3f (1.5,0,-2);
   glVertex3f (0,0,-1.5);
   glVertex3f (0,0,-1.5);
   glVertex3f (-1.5,0,-2);
   glVertex3f (-1.1,0,-1);
   glVertex3f(-1.1,0,-1);
   glVertex3f (-2,0,0);
   glVertex3f (-0.8,0,0);
   glEnd(); 

   glBegin(GL_POLYGON);
   glVertex3f (-0.8,0,0);
   glVertex3f (0.8,0,0);
   glVertex3f (1.1,0,-1);
   glVertex3f (0,0,-1.5);
   glVertex3f(-1.1,0,-1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      if(fps)
      {
      // Recalculate where the camera is looking
         AX = -2*dim*Sin(th)*Cos(ph);
         AY = -2*dim*Sin(ph);
         AZ = -2*dim*Cos(th)*Cos(ph);
      // Orient the scene so it imitates first person movement
         gluLookAt(EX,EY,EZ, EX+AX,EY+AY,EZ+AZ, 0,1,0);
      }
      else
      {
         double Ex = -2*dim*Sin(th)*Cos(ph);
         double Ey = +2*dim        *Sin(ph);
         double Ez = +2*dim*Cos(th)*Cos(ph);
         gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
      }
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Save transformation
   glPushMatrix();
   glRotatef(-90,1,0,0);

   //Tree
   double clr[4]={0.5, 1, 0, 1.0};
   cylinder(-3,0,-1,0.1,0.5);
   cone(-3,0,0,0.5,2,clr);

   cylinder(-2,0,-1,0.1,0.3);
   cone(-2,0,-0.5,0.3,1,clr);
   
   cylinder(2,0,-1,0.1,0.3);
   cone(2,0,-0.5,0.3,1,clr);

   cylinder(3,0,-1,0.1,0.5);
   cone(3,0,0,0.5,2,clr);
   
   //House
   house(0,0,0, 1,1,1);
   double clr1[4] = {0.3,0.15,0.2,1};
   cone(0,0,1,1.5,1,clr1);
   
   //Sun
   sphere2(2,0,3,0.5);

   //stars
   star(-4,1,3,0.1,0.1,0.1);
   star(-3,0.5,4,0.1,0.1,0.1);
   star(-2,0,3,0.1,0.1,0.1);
   star(-1,-0.5,4,0.1,0.1,0.1);
   star(0,-1,3,0.1,0.1,0.1);
   glColor3f(1,1,1);
   if(axes)
   {
      drawAxes(len);
   }
   if(mode && fps)
   {
      glWindowPos2i(5,25);
      Print("EX=%.1f,EY=%.1f,EZ=%.1f, AX=%.1f,AY=%.1f,AZ=%.1f, UX=%d,UY=%d,UZ=%d",EX,EY,EZ,AX,AY,AZ,UX,UY,UZ);
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d Dim=%.1f FOV=%d Projection=%s FPN=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal",fps?"ON":"OFF");
   //  Undo transformations
   glPopMatrix();
   ErrCheck("display");
   glFlush();//  Make scene visible
   glutSwapBuffers();
}

void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
      th = ph = 0;
      EX = 0;
      EY = 0;
      EZ = 2*dim;
   }  
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   // Toggle fps mode
   else if(ch == 'f' || ch == 'F')
   {
      fps = 1-fps;
   }
   //  Change field of view angle
   else if (ch == '-' && fov>1)
      fov--;
   else if (ch == '=' && fov<179)
      fov++;
   // Move forward in the scene
   else if(ch == 'w' || ch == 'W')
   {
      EX += AX*.1;
      EZ += AZ*.1;
   }
   // Move backwards in the scene
   else if(ch == 's' || ch == 'S')
   {
      EX -= AX*.1;
      EZ -= AZ*.1;
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc, char *argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(600,600);
   //  Create the window
   glutCreateWindow("HW2 - Santhanakrishnan Ramani"); //  Create window
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}