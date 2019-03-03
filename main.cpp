#include <windows.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "copengl.h"

//------------------------------------------------------------------------------
extern HINSTANCE hProjectInstance;

#include "cwnd_main.h"
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevstance,LPSTR lpstrCmdLine,int nCmdShow)
{
 hProjectInstance=hInstance;
 MSG msg;
 CWnd_Main_Register();
 CreateWindow("CWnd_Main","тест модуля фрагментной обработки теней (OpenGL)",WS_POPUP|WS_VISIBLE,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,hProjectInstance,NULL);
 //поменяем видеорежим
 DEVMODE dev;
 memset(&dev,sizeof(dev),0);
 dev.dmSize=sizeof(dev);
 dev.dmBitsPerPel=32;
 dev.dmPelsWidth=SCREEN_WIDTH;
 dev.dmPelsHeight=SCREEN_HEIGHT;
 dev.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
 ChangeDisplaySettings(&dev,0);
 while(GetMessage(&msg,NULL,0,0))
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 ChangeDisplaySettings(NULL,0);
 return(msg.wParam);
}

