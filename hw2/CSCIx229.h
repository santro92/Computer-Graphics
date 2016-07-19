#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

extern int th;       // Azimuth of view angle
extern int ph;       // Elevation of view angle
extern double dim;   //  Size of world
extern int mode;     // To change projection mode
extern double asp;     //  Aspect ratio
extern int fov;       //  Field of view (for perspective)

void Print(const char* format , ...);
void ErrCheck(const char* where);
void special(int key,int x,int y);
void reshape(int width,int height);
void drawAxes(const double len);
void Project();

#ifdef __cplusplus
}
#endif

#endif
