#ifndef POLYGON_H
#define POLYGON_H

#include <windows.h>
#include <vector>
using namespace std;

#include "vector.h"

//����
struct SPoint
{
 double X;
 double Y;
 double Z;
};
//�������
struct CPolygon
{
 vector< SPoint> vector_SPoint;//����� ��������
 //������, ����������� �������������
 SVector sVector_Normal;//������ �������
 //��������������� ������
 bool LightingEnable[8];
};
//---------------------------------------------------------------------------
SVector CalculateNormal(SPoint sp1,SPoint sp2,SPoint sp3);
SVector CalculateNormalVector(CPolygon &cPolygon);
//---------------------------------------------------------------------------
#endif