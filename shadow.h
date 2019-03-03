#ifndef SHADOW_H
#define SHADOW_H

#define S_EPS 0.01
#define S_EPS1 0.0000001
#define S_EPS2 0.0001

#ifndef M_PI
#define M_PI 3.14159265358979323
#endif

#include "polygon.h"

long GetPointPositionPlane(SPoint sPoint_A,SPoint sPoint_Plane,SVector sVector_Normal);
bool GetLineIntersectionPlane(SPoint sPoint_A,SPoint sPoint_B,SPoint sPoint_Plane,SVector sVector_Normal,long mode,SPoint &sPointReturn);
long GetPointPositionInPlaneXZ(SPoint sPoint_A,SPoint sPoint_B,SPoint sPoint);
bool IsPointInPolygonXZ(SPoint sPoint,CPolygon cPolygon);
bool GetPieceIntersectionInPlaneXZ(SPoint A,SPoint B,SPoint C,SPoint D,SPoint &sPointReturn);
bool GetLineIntersectionPieceInPlaneXZ(SPoint A,SPoint B,SPoint C,SPoint D,SPoint &sPointReturn);
bool IsPointInPolygon(SPoint sPoint,CPolygon cPolygon);
bool GenerateShadow(CPolygon cPolygon_ShadowSource,CPolygon cPolygon_ShadowDestination,SPoint sPoint_Lighting,vector<CPolygon> &vector_CPolygon_Fragment);

#endif