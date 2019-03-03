#ifndef COPENGL_H
#define COPENGL_H

#include <windows.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

class COpenGL
{
 public:
  HGLRC hrc;
  int SetDCPixelFormat(HDC hdc);
  void CreateContext(HDC hdc);
  void DeleteContext(void);
  void EnableMakeCurrent(HDC hdc);
  void DisableMakeCurrent(void);
  void SetProjectMatrix(int dx,int dy,float angle);
};
#endif
