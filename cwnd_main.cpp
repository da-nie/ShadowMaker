#include "cwnd_main.h"

HINSTANCE hProjectInstance;

CWnd_Main Wnd_Main;


//------------------------------------------------------------------------------
CWnd_Main::CWnd_Main(void)
{
 //строим сцену
 CPolygon cPolygon; 
 //плоскость под пирамидой
 AddPoint(cPolygon,-5,0,5);
 AddPoint(cPolygon,-5,0,-5);
 AddPoint(cPolygon,5,0,-5);
 AddPoint(cPolygon,5,0,5);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
 //боковая грань пирамиды
 AddPoint(cPolygon,-2,1,-2);
 AddPoint(cPolygon,0,5,0);
 AddPoint(cPolygon,-2,1,2);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
 //боковая грань пирамиды
 AddPoint(cPolygon,2,1,2);
 AddPoint(cPolygon,0,5,0);
 AddPoint(cPolygon,2,1,-2);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
 //боковая грань пирамиды
 AddPoint(cPolygon,-2,1,2);
 AddPoint(cPolygon,0,5,0);
 AddPoint(cPolygon,2,1,2);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
 //боковая грань пирамиды
 AddPoint(cPolygon,2,1,-2);
 AddPoint(cPolygon,0,5,0);
 AddPoint(cPolygon,-2,1,-2);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
 //основание пирамиды
 AddPoint(cPolygon,-2,1,-2);
 AddPoint(cPolygon,-2,1,2);
 AddPoint(cPolygon,2,1,2);
 AddPoint(cPolygon,2,1,-2);
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 vector_CPolygon_Stage.push_back(cPolygon);
 cPolygon.vector_SPoint.clear();
}
CWnd_Main::~CWnd_Main(void)
{
}
//------------------------------------------------------------------------------
void CWnd_Main::Create(HWND hWnds,WPARAM wParam,LPARAM lParam)
{
 hWnd=hWnds;
 HDC hdc=GetDC(hWnd);
 if (cOpenGL.SetDCPixelFormat(hdc)==1)
 {
  MessageBox(hWnd,"Не удалось активировать cOpenGL.","Ошибка",MB_OK);
  ReleaseDC(hWnd,hdc);
  DestroyWindow(hWnd);
  return;
 }
 cOpenGL.CreateContext(hdc);
 cOpenGL.EnableMakeCurrent(hdc);
 cOpenGL.SetProjectMatrix(SCREEN_WIDTH,SCREEN_HEIGHT,60);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);
 glShadeModel(GL_FLAT);
 glDisable(GL_DITHER);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_FRONT);
 glShadeModel(GL_SMOOTH);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 float *Texture;
 int height,width;
 glGenTextures(2,TextureNameArray);
 glBindTexture(GL_TEXTURE_2D,TextureNameArray[0]);
 Texture=glLoadBitmap("511.bmp",1,&height,&width);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
 gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,Texture);
 delete[](Texture);
 glBindTexture(GL_TEXTURE_2D,TextureNameArray[1]);
 Texture=glLoadBitmap("538.bmp",1,&height,&width);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
 gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_FLOAT,Texture);
 delete[](Texture);
 cOpenGL.DisableMakeCurrent();
 ReleaseDC(hWnd,hdc);
 Angle=28;
 Angle1=0;
 OldTimer=SetTimer(hWnd,100,1,NULL);
}
void CWnd_Main::Destroy(HWND hWnds,WPARAM wParam,LPARAM lParam)
{
 KillTimer(hWnd,100);
 cOpenGL.DeleteContext();
}
//------------------------------------------------------------------------------
void CWnd_Main_Register(void);
LONG WINAPI CWnd_Main_wndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
//------------------------------------------------------------------------------
void CWnd_Main_Register(void)
{
 WNDCLASS wc;
 wc.style=CS_HREDRAW|CS_VREDRAW;
 wc.cbClsExtra=0;
 wc.cbWndExtra=0;
 wc.hInstance=hProjectInstance;
 wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 wc.hCursor=LoadCursor(NULL,IDC_ARROW);
 wc.hbrBackground=(HBRUSH)(COLOR_WINDOW);
 wc.lpszMenuName=NULL;
 wc.lpszClassName="CWnd_Main";
 wc.lpfnWndProc=(WNDPROC)CWnd_Main_wndProc;
 RegisterClass(&wc);
}
LONG WINAPI CWnd_Main_wndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
 switch(msg)
 {
  case WM_CREATE:
  {
   Wnd_Main.Create(hWnd,wParam,lParam);
   return(0);
  }
  case WM_DESTROY:
  {
   Wnd_Main.Destroy(hWnd,wParam,lParam);
   PostQuitMessage(0);
   return(0);
  }
  case WM_PAINT:
  {
   Wnd_Main.Paint(hWnd,wParam,lParam);
   return(0);
  }
  case WM_TIMER:
  {
   Wnd_Main.Timer(hWnd,wParam,lParam);
   return(0);
  }
 }
 return(DefWindowProc(hWnd,msg,wParam,lParam));
}
//------------------------------------------------------------------------------
void CWnd_Main::Paint(HWND hWnds,WPARAM wParam,LPARAM lParam)
{
 PAINTSTRUCT ps;
 HDC hdc=BeginPaint(hWnd,&ps);
 cOpenGL.EnableMakeCurrent(hdc);
 ExecuteOpenGL();
 SwapBuffers(hdc);
 cOpenGL.DisableMakeCurrent();
 EndPaint(hWnd,&ps);
}
void CWnd_Main::Timer(HWND hWnds,WPARAM wParam,LPARAM lParam)
{
 if (GetAsyncKeyState(VK_ESCAPE)&32768)
 {
  DestroyWindow(hWnd);
  return;
 }

 //if (GetAsyncKeyState(VK_LEFT)&32768)
 {
  Angle+=1;
  if (Angle>=360) Angle-=360;
 }
 if (GetAsyncKeyState(VK_RIGHT)&32768)
 {
  Angle-=1;
  if (Angle<0) Angle+=360;
 }
 Angle1+=0.2f;
 if (Angle1>=360) Angle1-=360;
 InvalidateRect(hWnd,NULL,FALSE);
}

void CWnd_Main::ExecuteOpenGL(void)
{
 int n,m,k,l,v;

 glClearColor(0,0,0,0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(0,0,-20);
 glRotatef(45,1,0,0);
 glRotatef(25,0,1,0);
 glRotatef(Angle1,0,1,0);

 //построим тень
 int LightingAmount=2;
 SPoint sPoint_Lighting[2];

 sPoint_Lighting[0].X=10*sin(3.14159/180.0*Angle);
 sPoint_Lighting[0].Y=8;
 sPoint_Lighting[0].Z=10*cos(3.14159/180.0*Angle);

 sPoint_Lighting[1].X=10*cos(3.14159/180.0*(Angle+35));
 sPoint_Lighting[1].Y=10*cos(3.14159/180.0*(Angle-30));
 sPoint_Lighting[1].Z=10*sin(3.14159/180.0*Angle);

 glEnable(GL_LIGHTING);
 float l0_ambient[]={0,0,0,1};
 float l0_diffuse[]={0.3f,0.3f,1,1};
 float l0_specular[]={0,0,0,1};
 float l0_position[]={sPoint_Lighting[0].X,sPoint_Lighting[0].Y,sPoint_Lighting[0].Z,1};
 glLightfv(GL_LIGHT0,GL_AMBIENT,l0_ambient);
 glLightfv(GL_LIGHT0,GL_DIFFUSE,l0_diffuse);
 glLightfv(GL_LIGHT0,GL_SPECULAR,l0_specular);
 glLightfv(GL_LIGHT0,GL_POSITION,l0_position);

 float l1_ambient[]={0,0,0,1};
 float l1_diffuse[]={1,0.3f,0.3f,1};
 float l1_specular[]={0,0,0,1};
 float l1_position[]={sPoint_Lighting[1].X,sPoint_Lighting[1].Y,sPoint_Lighting[1].Z,1};
 glLightfv(GL_LIGHT1,GL_AMBIENT,l1_ambient);
 glLightfv(GL_LIGHT1,GL_DIFFUSE,l1_diffuse);
 glLightfv(GL_LIGHT1,GL_SPECULAR,l1_specular);
 glLightfv(GL_LIGHT1,GL_POSITION,l1_position);

 float m0_ambient[]={1,1,1,1};
 float m0_diffuse[]={1,1,1,1};
 float m0_specular[]={1,1,1,1};
 glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,m0_ambient);
 glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m0_diffuse);
 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,m0_specular);

 glEnable(GL_LIGHT0);
 glDisable(GL_LIGHT1);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
 GLUquadricObj *gluSphereObj;
 glPushMatrix();
 gluSphereObj=gluNewQuadric();
 glTranslatef(sPoint_Lighting[0].X,sPoint_Lighting[0].Y,sPoint_Lighting[0].Z);
 gluSphere(gluSphereObj,0.5,20,20);
 gluDeleteQuadric(gluSphereObj);
 glPopMatrix();

 glEnable(GL_LIGHT1);
 glDisable(GL_LIGHT0);
 glPushMatrix();
 gluSphereObj=gluNewQuadric();
 glTranslatef(sPoint_Lighting[1].X,sPoint_Lighting[1].Y,sPoint_Lighting[1].Z);
 gluSphere(gluSphereObj,0.5,20,20);
 gluDeleteQuadric(gluSphereObj);
 glPopMatrix();
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
 glEnable(GL_CULL_FACE);

 glEnable(GL_TEXTURE_2D);

 //выводим сцену
 for(n=0;n<vector_CPolygon_Stage.size();n++)//для каждого полигона сцены
 {
  vector<CPolygon> vector_CPolygon_Fragment;//фрагменты, на которые разбивается полигон сцены при выводе
  CPolygon cPolygon_Basic=vector_CPolygon_Stage[n];
  vector_CPolygon_Fragment.push_back(cPolygon_Basic);//добавили самого себя
  bool LightingEnable[8];
  LightingEnable[0]=true;//источники света все включены
  LightingEnable[1]=true;
  LightingEnable[2]=true;
  LightingEnable[3]=true;
  LightingEnable[4]=true;
  LightingEnable[5]=true;
  LightingEnable[6]=true;
  LightingEnable[7]=true;
  for(long u=0;u<8;u++) vector_CPolygon_Fragment[0].LightingEnable[u]=LightingEnable[u];
  for(m=0;m<vector_CPolygon_Stage.size();m++)//полигон, создающий тень
  {
   CPolygon cPolygon_ShadowSource=vector_CPolygon_Stage[m];
   if (m==n) continue;//сами на себя тень не отбрасываем
   for(l=0;l<LightingAmount;l++)//источник, освещающий полигон
   {
    vector<CPolygon> vector_CPolygon_NewFragment;
    for(k=0;k<vector_CPolygon_Fragment.size();k++)//для каждого фрагмента считаем тень и создаём суб-фрагменты
	{
	 vector<CPolygon> vector_CPolygon_Temporary;
     bool EnableShadow=GenerateShadow(cPolygon_ShadowSource,vector_CPolygon_Fragment[k],sPoint_Lighting[l],vector_CPolygon_Temporary);
	 //подключим источники света
     for(v=0;v<vector_CPolygon_Temporary.size();v++)
	 {
      CPolygon cPolygon=vector_CPolygon_Fragment[k];
	  for(long u=0;u<8;u++) vector_CPolygon_Temporary[v].LightingEnable[u]=cPolygon.LightingEnable[u];
	  if (v==0 && EnableShadow==true)//если тень была
	  {
       cPolygon.LightingEnable[l]=false;
       for(long u=0;u<8;u++) vector_CPolygon_Temporary[v].LightingEnable[u]=cPolygon.LightingEnable[u];
	  }
	 }
     for(v=0;v<vector_CPolygon_Temporary.size();v++)
	 {
      CPolygon cPolygon=vector_CPolygon_Temporary[v];
	  cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
      vector_CPolygon_NewFragment.push_back(cPolygon);
	 }
	 vector_CPolygon_Temporary.clear();
	}
    vector_CPolygon_Fragment.clear();
	for(k=0;k<vector_CPolygon_NewFragment.size();k++)
	{
     CPolygon cPolygon=vector_CPolygon_NewFragment[k];
     cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
     vector_CPolygon_Fragment.push_back(cPolygon);
	}
    vector_CPolygon_NewFragment.clear();
   }
  }
  //выведем все созданные фрагменты
  double lx,ly;
  if (n==0)//выводим плоскость
  {
   glBindTexture(GL_TEXTURE_2D,TextureNameArray[0]);
   lx=fabs(cPolygon_Basic.vector_SPoint[2].X-cPolygon_Basic.vector_SPoint[0].X);
   ly=fabs(cPolygon_Basic.vector_SPoint[2].Z-cPolygon_Basic.vector_SPoint[0].Z);
  }
  else//выводим пирамиду
  {
   glBindTexture(GL_TEXTURE_2D,TextureNameArray[1]);
   double dx=cPolygon_Basic.vector_SPoint[1].X-cPolygon_Basic.vector_SPoint[0].X;
   double dy=cPolygon_Basic.vector_SPoint[1].Y-cPolygon_Basic.vector_SPoint[0].Y;
   double dz=cPolygon_Basic.vector_SPoint[1].Z-cPolygon_Basic.vector_SPoint[0].Z;
   lx=sqrt(dx*dx+dz*dz);
   ly=fabs(dy);
  }
  for(m=0;m<vector_CPolygon_Fragment.size();m++)
  {
   CPolygon cPolygon=vector_CPolygon_Fragment[m];
   for(l=0;l<LightingAmount;l++)
   {
    if (cPolygon.LightingEnable[l]==true) glEnable(GL_LIGHT0+l);
                                     else glDisable(GL_LIGHT0+l);									 
   }
   glNormal3d(cPolygon.sVector_Normal.X,cPolygon.sVector_Normal.Y,cPolygon.sVector_Normal.Z);
   glBegin(GL_POLYGON);
   for(k=0;k<cPolygon.vector_SPoint.size();k++) 
   {
    double dx=cPolygon.vector_SPoint[k].X-cPolygon_Basic.vector_SPoint[0].X;
	double dy=cPolygon.vector_SPoint[k].Y-cPolygon_Basic.vector_SPoint[0].Y;
	double dz=cPolygon.vector_SPoint[k].Z-cPolygon_Basic.vector_SPoint[0].Z;
	double tx,ty;
	if (n==0)
	{
     tx=dx/lx;
	 ty=dz/ly;
	}
	else
	{
	 tx=sqrt(dx*dx+dz*dz)/lx;
	 ty=dy/ly;
	}
    glTexCoord2f(tx,ty);
    glVertex3d(cPolygon.vector_SPoint[k].X,cPolygon.vector_SPoint[k].Y,cPolygon.vector_SPoint[k].Z);
   }
   glEnd();
  }
  vector_CPolygon_Fragment.clear();
 }

 glDisable(GL_TEXTURE_2D);
 glFlush();
}
//добавить точку к полигону
void CWnd_Main::AddPoint(CPolygon &cPolygon,double x,double y,double z)
{
 SPoint sPoint;
 sPoint.X=x;
 sPoint.Y=y;
 sPoint.Z=z;
 cPolygon.vector_SPoint.push_back(sPoint);
}
