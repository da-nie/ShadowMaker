#ifndef WND_MAIN_H
#define WND_MAIN_H

#include <windows.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "copengl.h"

#include <vector>
using namespace std;

#include "polygon.h"
#include "glbitmap.h"
#include "vector.h"
#include "shadow.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

class CWnd_Main
{
 public:
  //-----------------------------------------------------------
  HWND hWnd;
  COpenGL cOpenGL;
  vector<CPolygon> vector_CPolygon_Stage;//список полигонов сцены
  float Angle;
  float Angle1;
  UINT OldTimer;
  GLuint TextureNameArray[2];
  //-----------------------------------------------------------
  //-----------------------------------------------------------
  CWnd_Main(void);
  ~CWnd_Main(void);
  //-----------------------------------------------------------
  void Create(HWND hWnds,WPARAM wParam,LPARAM lParam);
  void Destroy(HWND hWnds,WPARAM wParam,LPARAM lParam);
  void Paint(HWND hWnds,WPARAM wParam,LPARAM lParam);
  void Timer(HWND hWnds,WPARAM wParam,LPARAM lParam);
  //-----------------------------------------------------------
  void ExecuteOpenGL(void);//выполнить оцифровку сцены
  void AddPoint(CPolygon &cPolygon,double x,double y,double z);//добавить точку к полигону
  //-----------------------------------------------------------
};
//------------------------------------------------------------------------------
void CWnd_Main_Register(void);
LONG WINAPI CWnd_Main_wndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

#endif