#include "glbitmap.h"

float *glLoadBitmap(char *FileName,float Alpha,int *Height,int *Width)
{
 float *Image;
 unsigned char R[256];
 unsigned char G[256];
 unsigned char B[256];
 HANDLE hfbm=CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,
                        OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
 if (hfbm==INVALID_HANDLE_VALUE)
 {
  *Height=0;
  *Width=0;
  return(NULL);
 }
 DWORD dwRead;
 BITMAPFILEHEADER bmfh;
 ReadFile(hfbm, &bmfh,sizeof(BITMAPFILEHEADER),&dwRead,(LPOVERLAPPED)NULL);
 BITMAPINFOHEADER bmih;
 ReadFile(hfbm,&bmih,sizeof(BITMAPINFOHEADER),&dwRead,(LPOVERLAPPED)NULL);
 LPBITMAPINFO lpbmi=(LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER)+((1<<bmih.biBitCount)*sizeof(RGBQUAD))];
 lpbmi->bmiHeader.biSize=bmih.biSize;
 lpbmi->bmiHeader.biWidth=bmih.biWidth;
 lpbmi->bmiHeader.biHeight=bmih.biHeight;
 lpbmi->bmiHeader.biPlanes=bmih.biPlanes;
 lpbmi->bmiHeader.biBitCount=bmih.biBitCount;
 lpbmi->bmiHeader.biCompression=bmih.biCompression;
 lpbmi->bmiHeader.biSizeImage=bmih.biSizeImage;
 lpbmi->bmiHeader.biXPelsPerMeter=bmih.biXPelsPerMeter;
 lpbmi->bmiHeader.biYPelsPerMeter=bmih.biYPelsPerMeter;
 lpbmi->bmiHeader.biClrUsed=bmih.biClrUsed;
 lpbmi->bmiHeader.biClrImportant=bmih.biClrImportant;
 if (lpbmi->bmiHeader.biPlanes!=1 || bmih.biCompression!=0 || (lpbmi->bmiHeader.biBitCount!=24 && lpbmi->bmiHeader.biBitCount!=8) )
 {
  delete(lpbmi);
  *Height=0;
  *Width=0;
  return(NULL);
 }
 if (lpbmi->bmiHeader.biBitCount<24)//существует палитра цветов, нужно загрузить палитру
 {
  ReadFile(hfbm,lpbmi->bmiColors,((1<<bmih.biBitCount)*sizeof(RGBQUAD)),&dwRead,(LPOVERLAPPED)NULL);
  for(int n=0;n<256;n++)
  {
   R[n]=lpbmi->bmiColors[n].rgbRed;
   G[n]=lpbmi->bmiColors[n].rgbGreen;
   B[n]=lpbmi->bmiColors[n].rgbBlue;
  }
 }
 unsigned char *lpvBits=new unsigned char[bmfh.bfSize-bmfh.bfOffBits];
 ReadFile(hfbm,lpvBits,(bmfh.bfSize-bmfh.bfOffBits),&dwRead,(LPOVERLAPPED)NULL);
 CloseHandle(hfbm);
 float *ReturnBits=NULL;
 if (lpbmi->bmiHeader.biBitCount==8)//256 цветов
  {
   Image=new float[4*lpbmi->bmiHeader.biWidth*lpbmi->bmiHeader.biHeight+1];
   for(int y=0;y<lpbmi->bmiHeader.biHeight;y++)
    for(int x=0;x<lpbmi->bmiHeader.biWidth;x++)
     {
      int offset=(x+y*lpbmi->bmiHeader.biWidth)*4;
      int offset1=x+(lpbmi->bmiHeader.biHeight-y-1)*lpbmi->bmiHeader.biWidth;
      Image[offset]=(float)((float)(R[lpvBits[offset1]])/255.0);
      Image[offset+1]=(float)((float)(G[lpvBits[offset1]])/255.0);
      Image[offset+2]=(float)((float)(B[lpvBits[offset1]])/255.0);
      Image[offset+3]=Alpha;
     }
  }
 else
  {
   Image=new float[4*lpbmi->bmiHeader.biWidth*lpbmi->bmiHeader.biHeight+1];
   for(int y=0;y<lpbmi->bmiHeader.biHeight;y++)
    for(int x=0;x<lpbmi->bmiHeader.biWidth;x++)
     {
      int offset=(x+y*lpbmi->bmiHeader.biWidth)*4;
      int offset1=(x+(lpbmi->bmiHeader.biHeight-y-1)*lpbmi->bmiHeader.biWidth)*3;
      Image[offset]=(float)((float)(lpvBits[offset1+2])/255.0);
      Image[offset+1]=(float)((float)(lpvBits[offset1+1])/255.0);
      Image[offset+2]=(float)((float)(lpvBits[offset1])/255.0);
      Image[offset+3]=Alpha;
     }
  }
 *Width=lpbmi->bmiHeader.biWidth;
 *Height=lpbmi->bmiHeader.biHeight;
 delete(lpbmi);
 return(Image);
}
