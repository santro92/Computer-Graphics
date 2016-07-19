#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

extern int emission;  // Emission intensity (%)
extern float shinyvec[1];    // Shininess (value)

//Colors
extern float white[];
extern float black[];

// Texture Values
extern unsigned int ballTexture[17];   // ball textures
extern unsigned int tableTexture[10];  // table textures

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
void cube(double x,double y,double z, double dx, double dy, double dz, float clr[], unsigned int ntex, int rep);
void Vertex(double th,double ph);
void ball(double x,double y,double z,double r, double angle, int vec[2], float clr[], unsigned int ntex);
void cylinder(double x, double y, double z, double r, double h, float clr[], int ntex);
void frustum(double x, double y, double z, double th, double ph, double r1, double r2, double h, unsigned int ntex);

#ifdef __cplusplus
}
#endif

#endif
