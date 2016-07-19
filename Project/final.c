/* Time Taken = approx 50 hrs */
#include "CSCIx229.h"
#include <time.h>

// Globals
int th     =  15;  // Azimuth of view angle
int ph     =  55;  // Elevation of view angle
int axes   =   0;  // Display axes
int fov    =  80;  // Field of view (for perspective)
int light  =   0;  // Lighting
double asp =   2;  // Aspect ratio
double dim =  10;  // Size of world

// Light Values
int ambient     =  10;  // Ambient intensity (%)
int diffuse     = 100;  // Diffuse intensity (%)
int specular    =  80;  // Specular intensity (%)
int shininess   =   0;  // Shininess (power of two)
int emission    =   0;  // Emission intensity (%)
int moveCueBall =   0;  // To move the Cue Ball
int moveBalls   =   0;  // To move the Balls
float sco       =  90;  // Spot cutoff angle
float Exp       =   5;  // Spot exponent
float shinyvec[1] = {1};// Shininess (value)

// Texture Values
unsigned int ballTexture[17];   // ball textures
unsigned int tableTexture[10];  // table textures

// Stick Paramters
int hit      =    0;  // Switch on hit
int assist   =    0;  // Assit
double Dx    = -0.6;  //  Direction
double Dy    =  -10;  //  Direction
double Dz    =    3;  //  Direction
double roll  = -109;  //  Roll angle
double yaw   =    6;  //  Yaw angle

// Power
int pwr      =  1; // TogglePower
int powValue = 30; // Power Value

// Colors
float white[]  = {1,1,1,1};
float black[]  = {0,0,0,1};
float yellow[] = {1,1,0,1};
float green[]  = {0,1,0,1};
float brown[]  = {0.5,0.35,0.05,1.0};

#define R 0.3     // Cue Ball radius
const int N = 16; // No of Cue Balls
typedef struct
{
   double x;
   double y;
   double z;
   double r;
   float* clr;
   unsigned int ntex;
   char* texName;
   int draw;
   double angle;
   int vec[2];
   double xInc;
   double yInc;
   double angleInc;
   int xSign;
   int ySign;
} CueBalls;

CueBalls cueBalls[] = {
   {   0, -15*R, 1, R, white,  0, "texture/0.bmp" , 1, 90,  {-1,0},      0, -0.01,  0,  1,  1},
   {   0,   0*R, 1, R, white,  1, "texture/1.bmp" , 1, 90,  {-1,0},      0,  0.01,  0,  1,  1},
   {  -R,   2*R, 1, R, white,  2, "texture/2.bmp" , 1, 90, {-1,-1}, -0.005,  0.01,  2,  1,  1}, 
   {   R,   2*R, 1, R, white,  3, "texture/3.bmp" , 1, 90,  {-1,1},  0.005,  0.01,  2,  1,  1},
  
   {-2*R,   4*R, 1, R, white,  4, "texture/4.bmp" , 1, 90, {-1,-1},  -0.01,  0.02,  5,  1,  1},
   {   0,   4*R, 1, R, white,  5, "texture/5.bmp" , 1, 90,  {-1,0},      0,  0.02,  0,  1,  1},
   { 2*R,   4*R, 1, R, white,  6, "texture/6.bmp" , 1, 90,  {-1,1},   0.01,  0.02,  5,  1,  1},
  
   {-3*R,   6*R, 1, R, white,  7, "texture/7.bmp" , 1, 90, {-1,-1},  -0.02,  0.03,  8,  1,  1},
   {  -R,   6*R, 1, R, white,  8, "texture/8.bmp" , 1, 90, {-1,-1}, -0.015,  0.03,  6,  1,  1},
   {   R,   6*R, 1, R, white,  9, "texture/9.bmp" , 1, 90,  {-1,1},  0.015,  0.03,  6,  1,  1},
   { 3*R,   6*R, 1, R, white, 10, "texture/10.bmp", 1, 90,  {-1,1},   0.02,  0.03,  8,  1,  1},
  
   {-4*R,   8*R, 1, R, white, 11, "texture/11.bmp", 1, 90, {-1,-1}, -0.035,  0.08, 10,  1,  1},
   {-2*R,   8*R, 1, R, white, 12, "texture/12.bmp", 1, 90, {-1,-1}, -0.025,  0.03, 15,  1,  1},
   {   0,   8*R, 1, R, white, 13, "texture/13.bmp", 1, 90,  {-1,0},      0,  0.04, 20,  1,  1},
   { 2*R,   8*R, 1, R, white, 14, "texture/14.bmp", 1, 90,  {-1,1},   0.02,  0.04, 15,  1,  1},
   { 4*R,   8*R, 1, R, white, 15, "texture/15.bmp", 1, 90,  {-1,1},  0.035,  0.08, 10,  1,  1},
};

void mul_arr(double m2[4][1], double cueStickPos[4][1], double h)
{
    double m1[4][4] = {
                        {           0.2*Cos(yaw),             0,            h*Sin(yaw), Dx},
                        { 0.2*Sin(roll)*Sin(yaw), 0.2*Cos(roll), -h*Sin(roll)*Cos(yaw), Dy},
                        {-0.2*Cos(roll)*Sin(yaw), 0.2*Sin(roll),  h*Cos(roll)*Cos(yaw), Dz},
                        {                      0,             0,                     0,  1}
                      };
    int i,j,k;
    for(i=0;i<4;i++)
       for(j=0;j<1;j++)
         for (k=0;k<4;k++)
            cueStickPos[i][j] = cueStickPos[i][j] + (m1[i][k] * m2[k][j]);
}

void calcDirection(double cueStickBottomPos[4][1], double cueStickTopPos[4][1], double direction[3][1])
{
   direction[0][0] = cueStickBottomPos[0][0] - cueStickTopPos[0][0];
   direction[1][0] = cueStickBottomPos[1][0] - cueStickTopPos[1][0];
   direction[2][0] = cueStickBottomPos[2][0] - cueStickTopPos[2][0];
}

int checkCollision(int n)
{
   int i;
   for(i=0;i<N;i++)
   {
      if(i == n)
         continue;
      double distance_x = cueBalls[n].x - cueBalls[i].x;
      double distance_y = cueBalls[n].y - cueBalls[i].y; 
      double distance =  sqrt((distance_x * distance_x) + (distance_y * distance_y));
      if(distance <= 2*R)
      {
         return 1;
      }
   }
   return 0;
}

int checkBoundry(int i)
{
   double x = 3.2;
   double y = 7;
   if((cueBalls[i].x >= -x) && (cueBalls[i].x <= x) && (cueBalls[i].y >= -y) && (cueBalls[i].y <= y))
   {
      return 0;
   }
   else
   {
      if(cueBalls[i].x < -x)
      {
         cueBalls[i].x = -x;
         cueBalls[i].xSign *= -1;
      } 
      if(cueBalls[i].x > x)
      {
         cueBalls[i].x = x;
         cueBalls[i].xSign *= -1;
      } 
      if(cueBalls[i].y < -y)
      {
         cueBalls[i].y = -y;
         cueBalls[i].ySign *= -1;
      } 
      if(cueBalls[i].y > y)
      {
         cueBalls[i].y = y;
         cueBalls[i].ySign *= -1;
      }
      return 1;
   }
}

void checkPockets()
{
   int i;
   for(i=0;i<N;i++)
   {
      double x = cueBalls[i].x;
      double y = cueBalls[i].y;

      if((x>3 && y<-7) || (x<-3 && y<-7) || (x>3 && y>7) || (x<-3 && y>7) || (x>3 && y>-1 && y<1) || (x<-3 && y>-1 && y<1))
         cueBalls[i].draw = 0;
   }
}

void cueStick(double r1, double r2, double h, unsigned int ntex)
{
   frustum(Dx, Dy, Dz, roll, yaw, r1, r2, h, ntex); 
   
   // if(moveBalls)
   // {
   //    int i;
   //    for(i=N-1;i >=0;i--)
   //    {
   //       if(cueBalls[i].draw)
   //       {
   //          cueBalls[i].x += (powValue * 0.013 * cueBalls[i].xInc * cueBalls[i].xSign);
   //          cueBalls[i].y += (powValue * 0.013 * cueBalls[i].yInc * cueBalls[i].ySign);
   //          cueBalls[i].angle = (int) (cueBalls[i].angle + (powValue * 0.01 * cueBalls[i].angleInc)) % 360;
   //       }
   //    }
   //    checkPockets();
   //    moveBalls = (moveBalls + 1) % 100;
   // }

   // if(moveCueBall)
   // {
   //    cueBalls[0].x += (powValue * 0.01 * R * (Cos(96 - yaw)));
   //    cueBalls[0].y += (powValue * 0.01 * R);
   //    if(checkCollision(0) || checkBoundry(0) || (cueBalls[0].draw == 0))
   //    {
   //       if(checkCollision(0))
   //       {
   //          moveBalls = 1;
   //       }   
   //       moveCueBall = 0;
   //    }   
   //    checkPockets();    
   // }

   // double direction[3][1];
   // double m2[4][1] = {{0},{0},{-1},{1}};
   // double cueStickTopPos[4][1] ={{0},{0},{0},{0}};
   // double cueStickBottomPos[4][1] ={{0},{0},{0},{0}};
      
   // mul_arr(m2,cueStickBottomPos,h);
   // m2[2][0] = 1;
   // mul_arr(m2,cueStickTopPos,h);

   // if(assist)
   // {
   //    glBegin(GL_LINES);
   //    glVertex3f(cueStickTopPos[0][0],cueStickTopPos[1][0],cueStickTopPos[2][0]);
   //    glVertex3f(cueBalls[0].x,cueBalls[0].y,cueBalls[0].z);
   //    glEnd();
   // }

   // if(hit)
   // {
   //    calcDirection(cueStickBottomPos, cueStickTopPos, direction);
   //    Dx = cueStickTopPos[0][0] + (0.25*direction[0][0]);
   //    Dy = cueStickTopPos[1][0] + (0.25*direction[1][0]);
   //    Dz = cueStickTopPos[2][0] + (0.25*direction[2][0]);
   //    hit = 0;

   //    double stickPos[4][1] ={{0},{0},{0},{0}};
   //    mul_arr(m2,stickPos,h);
      
   //    if( (stickPos[0][0] > (cueBalls[0].x - 2*R) && stickPos[0][0] < (cueBalls[0].x + 2*R)) && (stickPos[1][0] > (cueBalls[0].y - 2*R) && stickPos[1][0] < (cueBalls[0].y + 2*R)) )
   //    {
   //       moveCueBall = 1;
   //       assist = 0;
   //    }
   // }
}

void scoreBoard()
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(-2.5,12.1,5);
  
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   glBindTexture(GL_TEXTURE_2D,tableTexture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(6,0,3);
   glTexCoord2f(1,0); glVertex3f(0,0,3);
   glTexCoord2f(1,1); glVertex3f(0,0,-3);
   glTexCoord2f(0,1); glVertex3f(6,0,-3);
   glEnd();
   glDisable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_TEXTURE_2D);
   
   glColor3f(1,1,1);
   glLineWidth(1.5);
   //S
   glBegin(GL_LINE_STRIP);
   glVertex3f(1.25,0,2.5);
   glVertex3f(0.5,0,2.5);
   glVertex3f(0.5,0,2);
   glVertex3f(1.25,0,2);
   glVertex3f(1.25,0,1.5);
   glVertex3f(0.5,0,1.5);
   glEnd();
   //C
   glBegin(GL_LINE_STRIP);
   glVertex3f(2.25,0,2.5);
   glVertex3f(1.5,0,2.5);
   glVertex3f(1.5,0,1.5);
   glVertex3f(2.25,0,1.5);
   glEnd();
   //O
   glBegin(GL_LINE_LOOP);
   glVertex3f(3.25,0,2.5);
   glVertex3f(2.5,0,2.5);
   glVertex3f(2.5,0,1.5);
   glVertex3f(3.25,0,1.5);
   glEnd();
   //R
   glBegin(GL_LINE_LOOP);
   glVertex3f(4.25,0,2.5);
   glVertex3f(3.5,0,2.5);
   glVertex3f(3.5,0,2);
   glVertex3f(4.25,0,2);
   glEnd();
   glBegin(GL_LINE_STRIP);
   glVertex3f(3.5,0,1.5);
   glVertex3f(3.5,0,2);
   glVertex3f(4.25,0,1.5);
   glEnd();
   //E
   glBegin(GL_LINE_STRIP);
   glVertex3f(5.25,0,2.5);
   glVertex3f(4.5,0,2.5);
   glVertex3f(4.5,0,1.5);
   glVertex3f(5.25,0,1.5);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(4.5,0,2);
   glVertex3f(5.25,0,2);
   glEnd();
   //0
   int i,flag = 0;
   for(i=1;i<N;i++)
      if(cueBalls[i].draw == 0)
         flag = 1;
   glBegin(GL_LINES);
   glVertex3f(3.5,0,0);
   glVertex3f(2.5,0,0);
   if(flag)
   {
      glVertex3f(2.5,0,-0.95);
      glVertex3f(3.5,0,-0.95);
   }  
   else
   {
      glVertex3f(2.5,0,-0.1);
      glVertex3f(2.5,0,-0.95);
      glVertex3f(3.5,0,-1.05);
      glVertex3f(3.5,0,-1.9);
   }
   glVertex3f(2.5,0,-1.05);
   glVertex3f(2.5,0,-1.9);
   glVertex3f(3.5,0,-2);
   glVertex3f(2.5,0,-2);
   glVertex3f(3.5,0,-0.1);
   glVertex3f(3.5,0,-0.95);
   glEnd();
   glPopMatrix();
}

void table(double x, double y, double z, double dx, double dy, double dz)
{  
   int i;
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   cube(1.5,2.25,0.8,1.5,0.25,0.2,green,1,1);
   cube(3.75,6.25,0.8,0.25,2.75,0.2,green,1,1);
   cube(1.75,6,0.3,1.75,4,0.3,green,1,1);
   cube(3.75,6,0.3,0.25,3,0.3,green,1,1);
   cube(1.75,1,0.5,1.75,1,0.5,brown,2,1);
   cube(4.5,6,0.5,0.5,3,0.5,brown,2,1);
   cube(4.75,9.5,0.5,0.25,0.5,0.5,brown,2,1);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,tableTexture[1]);
  
   glColor3f(0,1,0);
   // Cushion Padding start
   glBegin(GL_QUADS);
   glNormal3f(1,1,0);
   glTexCoord2f(0,0); glVertex3f(3,2.5,1);
   glTexCoord2f(1,0); glVertex3f(3,2.5,0.6);
   glTexCoord2f(1,1); glVertex3f(3.5,2,0.6);
   glTexCoord2f(0,1); glVertex3f(3.5,2,1);

   glNormal3f(1,-1,0);
   glTexCoord2f(0,0); glVertex3f(3.5,3.5,1);
   glTexCoord2f(1,0); glVertex3f(3.5,3.5,0.6);
   glTexCoord2f(1,1); glVertex3f(4,3,0.6);
   glTexCoord2f(0,1); glVertex3f(4,3,1);
   glEnd();
   
   glBegin(GL_TRIANGLES);  
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(3.5,2,1);
   glTexCoord2f(0,0); glVertex3f(3,2,1);
   glTexCoord2f(0,0); glVertex3f(3,2.5,1);

   glTexCoord2f(0,0); glVertex3f(3.5,3.5,1);
   glTexCoord2f(1,0); glVertex3f(4,3.5,1);
   glTexCoord2f(0.5,1); glVertex3f(4,3,1);
   glEnd();
   // Cushion Padding stop

   //Rounded Edges Start
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5); glVertex3f(3.5,9,0);
   for(i = -90; i <= 0; i += 15)
   {
      glTexCoord2f(Cos(i), 1 + Sin(i));
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 0);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5); glVertex3f(3.5,9,1);
   for(i = -90; i <= 0; i += 15)
   {
      glTexCoord2f(Cos(i), 1 + Sin(i));
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 1);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for(i = -90; i <= 0; i += 15)
   {
      glNormal3f(-Cos(th),-Sin(th),0);
      glTexCoord2f((i/90.0)+1,(i/90.0)+1);
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 0);
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 1);
   }
   glEnd();

   glColor3f(0.5,0.35,0.05);
   glBindTexture(GL_TEXTURE_2D,tableTexture[2]);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5); glVertex3f(4.5,9,0);
   for(i = 0; i <= 90; i += 15)
   {
      glTexCoord2f(1-Cos(i), Sin(i));
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 0);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5); glVertex3f(4.5,9,1);
   for(i = 0; i <= 90; i += 15)
   {
      glTexCoord2f(1-Cos(i), Sin(i));
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 1);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for(i = 90; i >= 0; i -= 15)
   {
      glNormal3f(-Cos(i),-Sin(i),0);
      glTexCoord2f(1-(i/90.0),1-(i/90.0));
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 0);
      glVertex3f(4 + (0.5*Sin(i)), 10 - (Cos(i)), 1);
   }
   glEnd();
   //Rounded Edges Stop

   //Outer Ring Start
   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(3.5,2,0);
   glTexCoord2f(1,0); glVertex3f(3.75,1.2,0);
   glTexCoord2f(0.5,1); glVertex3f(3.5,0,0);
   
   glTexCoord2f(0,0); glVertex3f(4,3,0);
   glTexCoord2f(1,0); glVertex3f(5,3,0);
   glTexCoord2f(0.5,1); glVertex3f(4.25,2.2,0);

   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(3.5,2,1);
   glTexCoord2f(1,0); glVertex3f(3.75,1.2,1);
   glTexCoord2f(0.5,1); glVertex3f(3.5,0,1);

   glTexCoord2f(0,0); glVertex3f(4,3,1);
   glTexCoord2f(1,0); glVertex3f(5,3,1);
   glTexCoord2f(0.5,1); glVertex3f(4.25,2.2,1);
   glEnd();
   
   glBegin(GL_QUAD_STRIP);
   for(i = 90; i >= 0; i -= 15)
   {
      glNormal3f(-Cos(i),-Sin(i),0);
      glTexCoord2f(1-(i/90.0),1-(i/90.0));
      glVertex3f(3.75 + (0.5*Cos(i)), 2.2 - (Sin(i)), 0);
      glVertex3f(3.75 + (0.5*Cos(i)), 2.2 - (Sin(i)), 1);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP); 
   for(i = 90; i >= 0; i -= 15)
   {
     glNormal3f(Cos(i),Sin(i),0);
     glTexCoord2f(1-(i/90.0),1-(i/90.0));
     glVertex3f(3.5 + (1.5*Cos(i)), 3 - (3*Sin(i)), 0);
     glVertex3f(3.5 + (1.5*Cos(i)), 3 - (3*Sin(i)), 1);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   glNormal3f(0,0,1);
   for(i = 90; i >= 0; i -= 15)
   {
      glTexCoord2f(1-(i/90.0),1-(i/90.0));
      glVertex3f(3.5 + (1.5*Cos(i)), 3 - (3*Sin(i)), 0);
      glVertex3f(3.75 + (0.5*Cos(i)), 2.2 - (Sin(i)), 0);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   glNormal3f(0,0,1);
   for(i = 90; i >= 0; i -= 15)
   {
      glTexCoord2f(1-(i/90.0),1-(i/90.0));
      glVertex3f(3.5 + (1.5*Cos(i)), 3 - (3*Sin(i)), 1);
      glVertex3f(3.75 + (0.5*Cos(i)), 2.2 - (Sin(i)), 1);
   }
   glEnd(); 

   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(3.5,2,0);
   glTexCoord2f(1,0); glVertex3f(3.75,1.2,0);
   glTexCoord2f(1,1); glVertex3f(3.75,1.2,1);
   glTexCoord2f(0,1); glVertex3f(3.5,2,1);

   glTexCoord2f(0,0); glVertex3f(4,3,0);
   glTexCoord2f(1,0); glVertex3f(4.25,2.2,0);
   glTexCoord2f(1,1); glVertex3f(4.25,2.2,1);
   glTexCoord2f(0,1); glVertex3f(4,3,1);
   glEnd();
   //Outer Ring Stop

   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void drawScene(void)
{
   // skybox
   // cube(0,0,1.5*dim,3*dim,3*dim,3*dim,white,0,1);
   // scoreBoard();
   // // Balls
   // int b;
   // for(b=N-1; b>=0; b--)
   // {
   //    if(cueBalls[b].draw == 1)
   //    {
   //       checkBoundry(b);
   //       ball(cueBalls[b].x, cueBalls[b].y, cueBalls[b].z, cueBalls[b].r, cueBalls[b].angle, cueBalls[b].vec, cueBalls[b].clr, cueBalls[b].ntex);
   //    }
   // }
   //One quad of table created and used symmetry to create others
      table(0,-10,0,1,1,1);
   table(0,-10,0,-1,1,1);
   table(0,10,0,-1,-1,1);
   table(0,10,0,1,-1,1);
     // table(0,0,0,1,1,1);
      // table(0,0,0,-1,1,1);
      // table(0,0,0,-1,-1,1);
      // table(0,0,0,1,-1,1);
   // Legs
   // cylinder(4,-9,-3,0.25,3,brown,2);
   // cylinder(-4,-9,-3,0.25,3,brown,2);
   // cylinder(-4,9,-3,0.25,3,brown,2);
   // cylinder(4,9,-3,0.25,3,brown,2);
   // // Stick
   // cueStick(0.5,1,4,3);
   // // Power
   // if(pwr)
   // {
   //    cylinder(5,-5*Cos(th),4+(powValue*0.01),0.5,powValue*0.01,green,5);
   //    cylinder(5,-5*Cos(th),6-(1-(powValue*0.01)),0.5,1-(powValue*0.01),yellow,6);
   //    cylinder(5,-5*Cos(th),6,0.1,0.1,brown,-1);
   // }
}

void display(void)
{
   //  Length of axes
   const double len=5.0;
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);  
   glLoadIdentity();
   //  Set perspective
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
   //  Draw scene
   glPushMatrix();
   glRotated(-90,1,0,0);
   glShadeModel(GL_SMOOTH);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //float color[]  = {0.5,0.01*pwr,0.1,1.0};
      //  Light position
      float Position[]  = {0,-10,10,1};
      float Position1[]  = {0,10,10,1};
      float Direction[] = {0,2,0,0};
      float Direction1[] = {0,-2,0,0};
      //  Draw light position as ball (still no lighting here)
      int vec[2] = {0,1};
      frustum(Position[0],Position[1]-0.25,Position[2]+0.25, 45,0, 2,4,0.5, 4);
      ball(Position[0],Position[1],Position[2], 0.5,0,vec, yellow, 0);
      frustum(Position1[0],Position1[1]+0.25,Position[2]+0.25, -45,0, 2,4,0.5, 4);
      ball(Position1[0],Position1[1],Position1[2], 0.5,0,vec, yellow, 0);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,0);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
      glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
      glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);

      // Enable light 1
      glEnable(GL_LIGHT1);
      glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT1,GL_POSITION,Position1);
      glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,Direction1);
      glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,sco);
      glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,Exp);
   }
   // To draw the scene
   drawScene();
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   glPopMatrix();
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",th,ph,dim,fov,light?"On":"Off");
   glWindowPos2i(5,25);
   Print("Dx=%.1f Dy=%.1f Dz=%.1f roll=%0.1f yaw=%0.f pwr=%d", Dx, Dy, Dz, roll, yaw, pwr);
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
   ErrCheck("display");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

void idle()
{
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
      th            =    15;
      ph            =    55;
      cueBalls[0].x =     0;
      cueBalls[0].y = -15*R;
      Dx            =  -0.6;  
      Dy            =   -10;  
      Dz            =     3; 
      roll          =  -109;
      yaw           =     6;
   }
   // To bring back CueBall
   else if (ch == 'b' || ch == 'B')
   {
      cueBalls[0].draw = 1;
      cueBalls[0].x = 0;
      cueBalls[0].y = -15*R;
   }
   //  Toggle axes
   else if (ch == 'q' || ch == 'Q')
      axes = 1-axes;
   //  Toggle hit
   else if (ch == 'h' || ch == 'H')
      hit = 1-hit;
   //  Toggle Assist
   else if (ch == 'g' || ch == 'G')
      assist = 1-assist;
   //  Change field of view angle
   else if (ch == '-' && fov>1)
      fov--;
   else if (ch == '=' && fov<179)
      fov++;
   //  Cue Stick Parameters
   else if (ch == '1')
      roll -= 0.5;
   else if (ch == '2')
      roll += 0.5;
   else if (ch == '3')
      yaw -= 0.5;
   else if (ch == '4')
      yaw += 0.5;
   else if (ch == 'x' && Dx < 6)
      Dx += 0.1;
   else if (ch == 'X' && Dx > -6)
      Dx -= 0.1;
   else if (ch == 'y'&& Dy < 10 )
      Dy += 0.1;
   else if (ch == 'Y' && Dy > -10)
      Dy -= 0.1;
   else if (ch == 'z' && Dz < 4)
      Dz += 0.1;
   else if (ch == 'Z' && Dz > 1)
      Dz -= 0.1;
   else if (ch == 'p' || ch == 'P')
      pwr = 1 - pwr;
   else if (ch == '>' && powValue < 100)
      powValue += 5;
   else if (ch == '<' && powValue > 0)
      powValue -= 5;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
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
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}

int main(int argc, char *argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 1000 x 1000 pixel window
   glutInitWindowSize(800,600);
   //  Create the window
   glutCreateWindow("Final - Santhanakrishnan Ramani");
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
   // Load texture
   int b;
   for(b=0; b<N; b++)
   {
      ballTexture[b] = LoadTexBMP(cueBalls[b].texName);
   }
   tableTexture[0] = LoadTexBMP("texture/night.bmp");
   tableTexture[1] = LoadTexBMP("texture/table.bmp");
   tableTexture[2] = LoadTexBMP("texture/wood.bmp");
   tableTexture[3] = LoadTexBMP("texture/stick.bmp");
   tableTexture[4] = LoadTexBMP("texture/lamp.bmp");
   tableTexture[5] = LoadTexBMP("texture/green.bmp");
   tableTexture[6] = LoadTexBMP("texture/yellow.bmp");
   tableTexture[7] = LoadTexBMP("texture/score.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}