#include "CSCIx229.h"

void cube(double x,double y,double z, double dx, double dy, double dz, float clr[], unsigned int ntex, int rep)
{
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   if(ntex >= 0)
      glBindTexture(GL_TEXTURE_2D,tableTexture[ntex]);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glBegin(GL_QUADS);
   glColor3f(clr[0],clr[1],clr[2]);
   //  Front
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(rep,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(rep,rep); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,rep); glVertex3f(-1,+1,+1);
   //  Back
   glNormal3f(0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,rep); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,rep); glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(rep,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,rep); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,rep); glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(rep,rep); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,rep); glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f(0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(rep,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(rep,rep); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,rep); glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f(0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,rep); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,rep); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix(); 
   glDisable(GL_TEXTURE_2D);
}

void Vertex(double th,double ph)
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

void ball(double x,double y,double z,double r, double angle, int vec[2], float clr[], unsigned int ntex)
{
   int th,ph;
   int inc = 5;
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,ballTexture[ntex]);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glRotated(angle,vec[0],vec[1],0);
   //  White ball
   glColor3f(clr[0], clr[1], clr[2]);
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

void cylinder(double x, double y, double z, double r, double h, float clr[], int ntex)
{
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,tableTexture[ntex]);
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glScaled(r,r,h);
   glColor3f(clr[0],clr[1],clr[2]);
   // Cylinder Top & Bottom
   int i,k;
   for (i=1;i>=-1;i-=2)
   {
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0,0,i);
      glTexCoord2f(0.5,0.5); glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5); glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   // Cylinder "Cover"
   glBegin(GL_QUAD_STRIP);
   for(i = 0; i <=360; i += 15) 
   {
      glNormal3f(Cos(i), Sin(i), 0);
      glTexCoord2f(1,0.5*i); glVertex3f(Cos(i), Sin(i), -1);
      glTexCoord2f(0,0.5*i); glVertex3f(Cos(i), Sin(i), 1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void frustum(double x, double y, double z, double th, double ph, double r1, double r2, double h, unsigned int ntex)
{
   int k;
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,tableTexture[ntex]);
   //  Save transformation
   glPushMatrix();
   //
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glRotated(ph,0,1,0);
   glScaled(0.2,0.2,h);
   glColor3f(0.5,0.35,0.05);
   // Cylinder Top & Bottom
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glTexCoord2f(0.5,0.5); glVertex3f(0,0,1);
   for (k=0;k<=360;k+=10)
   {
      glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5); 
      glVertex3f(r1*Cos(k),r1*Sin(k),1);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.5,0.5); glVertex3f(0, 0, -1);
   for (k=0;k<=360;k+=10)
   {
      glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5); 
      glVertex3f(r2*Cos(k), r2*Sin(k), -1);
   }
   glEnd();

   // Cylinder "Cover"
   glBegin(GL_QUAD_STRIP);
   for(k = 0; k <=360; k += 15) 
   {
      glNormal3f(Cos(k), Sin(k), 0);
      glTexCoord2f(1,0.5*k); glVertex3f(r2*Cos(k), r2*Sin(k), -1);
      glTexCoord2f(0,0.5*k); glVertex3f(r1*Cos(k), r1*Sin(k), 1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}