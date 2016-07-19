/* Time Taken = approx 10 hrs */
#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
double len=2.0;  //  Length of axes
// Light values
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[10]; // Texture names

//  Set specular color to white
float white[] = {1,1,1,1};
float black[] = {0,0,0,1};
float yellow[] = {1,1,0,1};
float green[] = {0,1,0,1};
float brown[] = {0.5,0.35,0.05,1.0};

static void cube(double x,double y,double z, double dx,double dy,double dz, float clr[], int ntex)
{
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,clr);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[ntex]);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //cube
   glBegin(GL_QUADS);
   glColor3f(clr[0],clr[1],clr[2]);
   //  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();   
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

/*
 *  Draw a sphere
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere2(double x,double y,double z,double r)
{
   int th,ph;
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void cylinder(double x, double y, double z, double r, double h)
{
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,brown);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,brown);
   int i,k;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(r,r,h);
   // Cylinder Top & Bottom

   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }

   // Cylinder "Cover"
   glBegin(GL_QUAD_STRIP);
   for(i = 0; i <=360; i += 15) 
   {
      glNormal3f(Cos(i), Sin(i), 0);
      glTexCoord2f(0,0.5*i); glVertex3f(Cos(i), Sin(i), 1);
      glTexCoord2f(1,0.5*i); glVertex3f(Cos(i), Sin(i), -1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void cone(double x, double y, double z, double r, double h, float clr[], int ntex)
{
   int th;
   int inc = 10;
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,clr);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,clr);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[ntex]);
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(r,r,h);
   glColor4f(clr[0], clr[1], clr[2], clr[3]);
   // Cone Top
   glBegin(GL_TRIANGLES);
   for(th = 0; th <= 360; th += 2*inc) 
   {
      glTexCoord2f(0.5*Cos(th-inc)+0.5,0.5*Sin(th-inc)+0.5);
      glNormal3d(Cos(th-inc),Sin(th-inc),1);
      glVertex3d(Cos(th-inc), Sin(th-inc), 0);
      glTexCoord2f(0.5,0.5);
      glNormal3d(Cos(th),Sin(th),1);
      glVertex3d(0, 0, 1);
      glTexCoord2f(0.5*Cos(th+inc)+0.5,0.5*Sin(th+inc)+0.5);
      glNormal3d(Cos(th+inc),Sin(th+inc),1);
      glVertex3d(Cos(th+inc), Sin(th+inc), 0);
   }
   glEnd();
   
   // Cone Bottom
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.5,0.5);
   glVertex3d(0,0,0);
   for(th = -inc; th < 360; th += 2*inc) 
   {
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Cos(th), Sin(th), 0.0);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void house(double x,double y,double z, double dx,double dy,double dz)
{
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //house 
   float clr[3] = {0.69,0.71,0.47};
   cube(x,y,z,dx,dy,dz,clr,0);
   //Road
   float clr1[3] = {0.2,0.2,0.2};
   cube(0,-3,-1.1,4,1.5,0.1,clr1,1);
   //Land
   cube(0,0,-1.1,4,1.5,0.1,green,8);
   glDisable(GL_POLYGON_OFFSET_FILL);

   float brown[] = {0.5,0.35,0.05,1.0};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,brown);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,brown);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //  Door
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glColor3f(0.3,0.15,0.2);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(-0.8,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-0.2,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-0.2,-1,0);
   glTexCoord2f(0,1); glVertex3f(-0.8,-1,0);
   glEnd();
   //  Window
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);
   glColor3f(0.5,0.5,1);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(0.3,-1,-0.5);
   glTexCoord2f(1,0); glVertex3f(0.7,-1,-0.5);
   glTexCoord2f(1,1); glVertex3f(0.7,-1,0);
   glTexCoord2f(0,1); glVertex3f(0.3,-1,0);
   glEnd();

   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
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
   glNormal3f(0,1,0);
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
   glNormal3f(0,1,0);
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
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   glRotatef(-90,1,0,0);
   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      sphere2(Position[0],Position[1],Position[2], 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
   {
      glDisable(GL_LIGHTING);
   }

   //Tree
   cylinder(-3,0,-0.5,0.1,0.5);
   cylinder(-2,0,-0.75,0.1,0.3);
   cylinder(2,0,-0.75,0.1,0.3);
   cylinder(3,0,-0.5,0.1,0.5);

   float clr[4]={0.5, 1, 0, 1.0};
   cone(-3,0,-0.5,0.5,2,clr,6);
   cone(-2,0,-0.75,0.3,1,clr,6);  
   cone(2,0,-0.75,0.3,1,clr,6);
   cone(3,0,-0.5,0.5,2,clr,6);
   
   //House
   house(0,0,0, 1,1,1);
   float clr1[4] = {0.3,0.15,0.2,1};
   cone(0,0,1,1.5,1,clr1,7);
   
   //Moon
   sphere2(2,0,3,0.5);

   //stars
   star(-4,-1,3,0.1,0.1,0.1);
   star(-3,-1,4,0.1,0.1,0.1);
   star(-2,-1,3,0.1,0.1,0.1);
   star(-1,-1,4,0.1,0.1,0.1);
   star(0,-1,3,0.1,0.1,0.1);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if(axes)
   {
      drawAxes(len);
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f zh=%d",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight,zh);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
   //  Render the scene and make it visible
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
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && fov>1)
      fov--;
   else if (ch == '+' && fov<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   glutCreateWindow("HW3 - Santhanakrishnan Ramani");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Tell GLUT to call "idle" when idle
   glutIdleFunc(idle);
    //  Load textures
   texture[0] = LoadTexBMP("wall.bmp");
   texture[1] = LoadTexBMP("road.bmp");
   texture[2] = LoadTexBMP("door.bmp");
   texture[3] = LoadTexBMP("window.bmp");
   texture[4] = LoadTexBMP("moon.bmp");
   texture[5] = LoadTexBMP("tree.bmp");
   texture[6] = LoadTexBMP("leaf.bmp");
   texture[7] = LoadTexBMP("roof.bmp");
   texture[8] = LoadTexBMP("land.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}