#ifndef POLYGON_H
#define POLYGON_H

#include <windows.h>
#include <vector>
using namespace std;

#include "vector.h"

//тока
struct SPoint
{
 double X;
 double Y;
 double Z;
};
//полигон
struct CPolygon
{
 vector< SPoint> vector_SPoint;//точки полигона
 //данные, вычисляемые автоматически
 SVector sVector_Normal;//вектор нормали
 //вспомогательные данные
 bool LightingEnable[8];
};
//---------------------------------------------------------------------------
SVector CalculateNormal(SPoint sp1,SPoint sp2,SPoint sp3);
SVector CalculateNormalVector(CPolygon &cPolygon);
//---------------------------------------------------------------------------
#endif