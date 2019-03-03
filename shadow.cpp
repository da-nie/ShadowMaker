#include "shadow.h"

//----------------------------------------------------------------------------------------------------
//получить положение точки sPoint_A относительно плоскости, заданой точкой sPoint_Plane и вектором нормали sVector_Normal
//----------------------------------------------------------------------------------------------------
long GetPointPositionPlane(SPoint sPoint_A,SPoint sPoint_Plane,SVector sVector_Normal)
{
 double p=sVector_Normal.X*(sPoint_A.X-sPoint_Plane.X)+sVector_Normal.Y*(sPoint_A.Y-sPoint_Plane.Y)+sVector_Normal.Z*(sPoint_A.Z-sPoint_Plane.Z);
 if (p<=-S_EPS2) return(-1);
 if (p>=S_EPS2) return(1);
 return(0);
}
//----------------------------------------------------------------------------------------------------
//получить точку пересечения прямой (sPoint_A,sPoint_B) с плоскостью (sPoint_Plane,sVector_Normal), 
//где mode=0 определяет луч с одной стороны, mode=2 задаёт луч с другой стороны, mode=1 определяет прямую
//----------------------------------------------------------------------------------------------------
bool GetLineIntersectionPlane(SPoint sPoint_A,SPoint sPoint_B,SPoint sPoint_Plane,SVector sVector_Normal,long mode,SPoint &sPointReturn)
{
 double A=sVector_Normal.X;
 double B=sVector_Normal.Y;
 double C=sVector_Normal.Z;
 double lx=sPoint_B.X-sPoint_A.X;
 double ly=sPoint_B.Y-sPoint_A.Y;
 double lz=sPoint_B.Z-sPoint_A.Z;
 double E=(A*lx+B*ly+C*lz);
 if (fabs(E)<=S_EPS1) return(false);//нет пересечения
 double D=-(sPoint_Plane.X*A+sPoint_Plane.Y*B+sPoint_Plane.Z*C);
 double t=-(D+A*sPoint_A.X+B*sPoint_A.Y+C*sPoint_A.Z);
 //уменьшим погрешности вычислений
 if (E<0)
 {
  t=-t;
  E=-E;
 }
 if (t<E+S_EPS1 && mode==0) return(false);
 if ((t>E-S_EPS1 || t<-S_EPS1) && mode==2) return(false);
 sPointReturn.X=sPoint_A.X+(t*lx)/E;
 sPointReturn.Y=sPoint_A.Y+(t*ly)/E;
 sPointReturn.Z=sPoint_A.Z+(t*lz)/E;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить положение точки sPoint в плоскости XZ (без учёта координаты Y), относительно прямой, заданной точками sPoint_A и sPoint_B
//----------------------------------------------------------------------------------------------------
long GetPointPositionInPlaneXZ(SPoint sPoint_A,SPoint sPoint_B,SPoint sPoint)
{
 double p=sPoint.X*(sPoint_A.Z-sPoint_B.Z)-sPoint.Z*(sPoint_A.X-sPoint_B.X)+sPoint_A.X*sPoint_B.Z-sPoint_A.Z*sPoint_B.X;
 if (p<=-S_EPS2) return(-1);
 if (p>=S_EPS2) return(1);
 return(0);
}
//----------------------------------------------------------------------------------------------------
//попадает ли точка sPoint внутрь полигона cPolygon в плоскости XZ (без учёта координаты Y)
//----------------------------------------------------------------------------------------------------
bool IsPointInPolygonXZ(SPoint sPoint,CPolygon cPolygon)
{
 long OldPosition=0;
 for(long v=0;v<cPolygon.vector_SPoint.size();v++)
 {
  long v1=v;//текущая точка
  long v2=v+1;//следующая точка
  if (v2>=cPolygon.vector_SPoint.size()) v2-=cPolygon.vector_SPoint.size();
  long Position=GetPointPositionInPlaneXZ(cPolygon.vector_SPoint[v1],cPolygon.vector_SPoint[v2],sPoint);
  if (Position==0) continue;
  if (OldPosition==0) OldPosition=Position;
  if (OldPosition!=Position) return(false);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить точку пересечения двух отрезков в плоскости XZ (без учёта координаты Y)
//----------------------------------------------------------------------------------------------------
bool GetPieceIntersectionInPlaneXZ(SPoint A,SPoint B,SPoint C,SPoint D,SPoint &sPointReturn)
{
 double z=(B.X-A.X)*(D.Z-C.Z)-(B.Z-A.Z)*(D.X-C.X);
 if (fabs(z)<=S_EPS1) return(false);
 double r=(A.Z-C.Z)*(D.X-C.X)-(A.X-C.X)*(D.Z-C.Z);
 double s=(A.Z-C.Z)*(B.X-A.X)-(A.X-C.X)*(B.Z-A.Z);
 //уменьшим погрешности вычислений
 if (z<0)
 {
  r=-r;
  s=-s;
  z=-z;
 }
 if (r<0 || r>z) return(false);
 if (s<0 || s>z) return(false);
 r/=z;
 s/=z;
 sPointReturn.X=(double)(A.X+r*(B.X-A.X));
 sPointReturn.Z=(double)(A.Z+r*(B.Z-A.Z));
 sPointReturn.Y=0;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить точку пересечения двух прямых в плоскости XZ
//----------------------------------------------------------------------------------------------------
bool GetLineIntersectionPieceInPlaneXZ(SPoint A,SPoint B,SPoint C,SPoint D,SPoint &sPointReturn)
{
 double z=(B.X-A.X)*(D.Z-C.Z)-(B.Z-A.Z)*(D.X-C.X);
 if (fabs(z)<=S_EPS1) return(false);
 double r=(A.Z-C.Z)*(D.X-C.X)-(A.X-C.X)*(D.Z-C.Z);
 double s=(A.Z-C.Z)*(B.X-A.X)-(A.X-C.X)*(B.Z-A.Z);
 //уменьшим погрешности вычислений
 if (z<0)
 {
  r=-r;
  s=-s;
  z=-z;
 }
 if (s<0 || s>z) return(false);
 r/=z;
 s/=z;
 sPointReturn.X=(double)(A.X+r*(B.X-A.X));
 sPointReturn.Z=(double)(A.Z+r*(B.Z-A.Z));
 sPointReturn.Y=0;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//определить, находится ли точка внутри полигона
//----------------------------------------------------------------------------------------------------
bool IsPointInPolygon(SPoint sPoint,CPolygon cPolygon)
{
 for(long n=0;n<cPolygon.vector_SPoint.size();n++)
 {
  long i1,i2,i3;
  i1=n;
  i2=n+1;
  i3=n+2;
  if (i2>=cPolygon.vector_SPoint.size()) i2-=cPolygon.vector_SPoint.size();
  if (i3>=cPolygon.vector_SPoint.size()) i3-=cPolygon.vector_SPoint.size();
  double v1x=cPolygon.vector_SPoint[i1].X;
  double v1y=cPolygon.vector_SPoint[i1].Y;
  double v1z=cPolygon.vector_SPoint[i1].Z;
   
  double v2x=cPolygon.vector_SPoint[i2].X;
  double v2y=cPolygon.vector_SPoint[i2].Y;
  double v2z=cPolygon.vector_SPoint[i2].Z;
   
  double v3x=cPolygon.vector_SPoint[i3].X;
  double v3y=cPolygon.vector_SPoint[i3].Y;
  double v3z=cPolygon.vector_SPoint[i3].Z;
   
  SVector V1=CreateVector(v2x-v1x,v2y-v1y,v2z-v1z);
  SVector V2=CreateVector(v3x-v1x,v3y-v1y,v3z-v1z);
  SVector V3=CreateVector(sPoint.X-v1x,sPoint.Y-v1y,sPoint.Z-v1z);
   
  SVector V4=VectorProduct(V1,V2);
  SVector V5=VectorProduct(V1,V3);
  V4=NormalizeVector(V4);
  V5=NormalizeVector(V5);
  long double scalar=ScalarProduct(V4,V5);
  if (scalar<-S_EPS1) return(false);
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//проверка корретности входных полигонов (число точек должно быть не менее трёх)
//----------------------------------------------------------------------------------------------------
bool CheckCorrectnessOfPolygons(const CPolygon &cPolygon,const CPolygon &cPolygon_ShadowDestination,vector<CPolygon> &vector_CPolygon_Fragment)
{
 if (cPolygon_ShadowDestination.vector_SPoint.size()<3) return(false);
 if (cPolygon.vector_SPoint.size()<3)
 {
  if (cPolygon_ShadowDestination.vector_SPoint.size()>=3) vector_CPolygon_Fragment.push_back(cPolygon_ShadowDestination);//добавим сам полигон, на которой отбрасывается тень
  return(false);//тень отсутствует 
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//проверка, может ли источник света вообще освещать многоугольник
//----------------------------------------------------------------------------------------------------
bool CheckIlluminationPossibility(const CPolygon &cPolygon_ShadowDestination,const SPoint &sPoint_Lighting,vector<CPolygon> &vector_CPolygon_Fragment)
{
 if (GetPointPositionPlane(sPoint_Lighting,cPolygon_ShadowDestination.vector_SPoint[0],cPolygon_ShadowDestination.sVector_Normal)!=1)//источник света не может освещать полигон
 {
  vector_CPolygon_Fragment.push_back(cPolygon_ShadowDestination);//добавим сам полигон
  return(false);
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//корректировка полигона, отбрасывающего тень и проверка может ли новый полигон быть источником тени
//----------------------------------------------------------------------------------------------------
bool CorrectionOfShadowSourcePolygon(CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,vector<CPolygon> &vector_CPolygon_Fragment)
{
 CPolygon cPolygon_CorrectShadowSource;//новый, скорректированный полигон, отбрасывающий тень
 long size=cPolygon_ShadowSource.vector_SPoint.size();
 for(long u=0;u<size;u++)
 {
  long u1=u;
  long u2=u+1;
  if (u2>=cPolygon_ShadowSource.vector_SPoint.size()) u2-=cPolygon_ShadowSource.vector_SPoint.size();//вышли за последнюю точку
  SPoint sPoint_A=cPolygon_ShadowSource.vector_SPoint[u1];
  SPoint sPoint_B=cPolygon_ShadowSource.vector_SPoint[u2];
  long PointAPosition=GetPointPositionPlane(sPoint_A,cPolygon_ShadowDestination.vector_SPoint[0],cPolygon_ShadowDestination.sVector_Normal);
  long PointBPosition=GetPointPositionPlane(sPoint_B,cPolygon_ShadowDestination.vector_SPoint[0],cPolygon_ShadowDestination.sVector_Normal);
  SPoint sPoint_Intersection;
  if (PointAPosition>=0)//точка A расположена правильно 
  {
   cPolygon_CorrectShadowSource.vector_SPoint.push_back(sPoint_A);
  }
  //проверим пересечение 
  if (PointAPosition*PointBPosition<0)//есть пересечение (точки по разные стороны от плоскости)
  {
   //найдём его
   if (GetLineIntersectionPlane(sPoint_A,sPoint_B,cPolygon_ShadowDestination.vector_SPoint[0],cPolygon_ShadowDestination.sVector_Normal,1,sPoint_Intersection)==true)
   {
    cPolygon_CorrectShadowSource.vector_SPoint.push_back(sPoint_Intersection); 
   }
  }
 }
 if (cPolygon_CorrectShadowSource.vector_SPoint.size()<3)//тени нет
 {
  cPolygon_CorrectShadowSource.vector_SPoint.clear();
  if (cPolygon_ShadowDestination.vector_SPoint.size()>=3) vector_CPolygon_Fragment.push_back(cPolygon_ShadowDestination);//добавим сам полигон
  return(false);
 }
 cPolygon_CorrectShadowSource.sVector_Normal=CalculateNormalVector(cPolygon_CorrectShadowSource);
 cPolygon_ShadowSource=cPolygon_CorrectShadowSource;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//проверка, что полигон, на который ложится тень и источник света не находятся по одноу сторону от полигона, отрасывающего тень
//----------------------------------------------------------------------------------------------------
bool CheckSideToShadowSourcePolygon(const CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,const SPoint &sPoint_Lighting,vector<CPolygon> &vector_CPolygon_Fragment)
{
 long lightpos=GetPointPositionPlane(sPoint_Lighting,cPolygon_ShadowSource.vector_SPoint[0],cPolygon_ShadowSource.sVector_Normal);
 bool ShadowOn=false;
 for(long n=0;n<cPolygon_ShadowDestination.vector_SPoint.size();n++)
 {
  long pos=GetPointPositionPlane(cPolygon_ShadowDestination.vector_SPoint[n],cPolygon_ShadowSource.vector_SPoint[0],cPolygon_ShadowSource.sVector_Normal);
  if (pos==0) continue;
  if (pos!=lightpos)//тень есть
  {
   ShadowOn=true;
   break;
  }
 }
 if (ShadowOn==false)
 {
  if (cPolygon_ShadowDestination.vector_SPoint.size()>=3) vector_CPolygon_Fragment.push_back(cPolygon_ShadowDestination);//добавим сам полигон
  return(false);
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//создание темнового объёма
//----------------------------------------------------------------------------------------------------
vector<CPolygon> CreateShadowVolume(const CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,const SPoint &sPoint_Lighting)
{
 SVector sVector_Temporary;//резервный вектор
 vector<CPolygon> vector_CPolygon_ShadowBorder;//набор полигонов, ограничивающий теневой объём
 if (cPolygon_ShadowSource.vector_SPoint.size()>=3) vector_CPolygon_ShadowBorder.push_back(cPolygon_ShadowSource);//добавляем себя
 for(long n=0;n<cPolygon_ShadowSource.vector_SPoint.size();n++)//добавляем полигоны для каждой стороны
 {
  CPolygon cPolygon_Temporary;//резервный полигон, в котором будет собираться сторона теневого объёма
  long i=n;
  long j=n+1;
  if (j>=cPolygon_ShadowSource.vector_SPoint.size()) j-=cPolygon_ShadowSource.vector_SPoint.size();//вышли за последнюю точку
  sVector_Temporary=CreateVector(cPolygon_ShadowSource.vector_SPoint[i].X-sPoint_Lighting.X,cPolygon_ShadowSource.vector_SPoint[i].Y-sPoint_Lighting.Y,cPolygon_ShadowSource.vector_SPoint[i].Z-sPoint_Lighting.Z);
  sVector_Temporary=NormalizeVector(sVector_Temporary);
  SPoint sPoint;
  sPoint.X=cPolygon_ShadowSource.vector_SPoint[i].X+sVector_Temporary.X*10000.0;
  sPoint.Y=cPolygon_ShadowSource.vector_SPoint[i].Y+sVector_Temporary.Y*10000.0;
  sPoint.Z=cPolygon_ShadowSource.vector_SPoint[i].Z+sVector_Temporary.Z*10000.0;
  cPolygon_Temporary.vector_SPoint.push_back(sPoint);

  cPolygon_Temporary.vector_SPoint.push_back(cPolygon_ShadowSource.vector_SPoint[i]);
  cPolygon_Temporary.vector_SPoint.push_back(cPolygon_ShadowSource.vector_SPoint[j]);
  sVector_Temporary=CreateVector(cPolygon_ShadowSource.vector_SPoint[j].X-sPoint_Lighting.X,cPolygon_ShadowSource.vector_SPoint[j].Y-sPoint_Lighting.Y,cPolygon_ShadowSource.vector_SPoint[j].Z-sPoint_Lighting.Z);
  sVector_Temporary=NormalizeVector(sVector_Temporary);

  sPoint.X=cPolygon_ShadowSource.vector_SPoint[j].X+sVector_Temporary.X*10000.0;
  sPoint.Y=cPolygon_ShadowSource.vector_SPoint[j].Y+sVector_Temporary.Y*10000.0;
  sPoint.Z=cPolygon_ShadowSource.vector_SPoint[j].Z+sVector_Temporary.Z*10000.0;
  cPolygon_Temporary.vector_SPoint.push_back(sPoint);

  cPolygon_Temporary.sVector_Normal=CalculateNormalVector(cPolygon_Temporary);//вычисляем нормаль к грани теневого объёма
  vector_CPolygon_ShadowBorder.push_back(cPolygon_Temporary);//добавляем грань
 }
 return(vector_CPolygon_ShadowBorder);
}
//----------------------------------------------------------------------------------------------------
//коррекция нормалей граней теневого объёма, чтобы они смотрели наружу из теневого объёма
//----------------------------------------------------------------------------------------------------
void CorrectionShadowVolume(const CPolygon &cPolygon_ShadowSource,vector<CPolygon> &vector_CPolygon_ShadowBorder,const SPoint &sPoint_Lighting)
{
 long n;
 SVector sVector_Temporary;//резервный вектор
 SPoint sPoint_Temporary;//резервная точка

 SPoint sPoint_ShadowSourceCenter;//точка в центре грани
 sPoint_ShadowSourceCenter.X=0;
 sPoint_ShadowSourceCenter.Y=0;
 sPoint_ShadowSourceCenter.Z=0;
 long size=cPolygon_ShadowSource.vector_SPoint.size();
 for(n=0;n<size;n++)
 {
  sPoint_ShadowSourceCenter.X+=cPolygon_ShadowSource.vector_SPoint[n].X;
  sPoint_ShadowSourceCenter.Y+=cPolygon_ShadowSource.vector_SPoint[n].Y;
  sPoint_ShadowSourceCenter.Z+=cPolygon_ShadowSource.vector_SPoint[n].Z;
 } 
 sPoint_ShadowSourceCenter.X/=static_cast<double>(cPolygon_ShadowSource.vector_SPoint.size());
 sPoint_ShadowSourceCenter.Y/=static_cast<double>(cPolygon_ShadowSource.vector_SPoint.size());
 sPoint_ShadowSourceCenter.Z/=static_cast<double>(cPolygon_ShadowSource.vector_SPoint.size());
 //скорректируем нормали нашего набора (нормаль должна быть наружу)
 sVector_Temporary=CreateVector(sPoint_ShadowSourceCenter.X-sPoint_Lighting.X,sPoint_ShadowSourceCenter.Y-sPoint_Lighting.Y,sPoint_ShadowSourceCenter.Z-sPoint_Lighting.Z);
 sVector_Temporary=NormalizeVector(sVector_Temporary);
 //эта точка точно лежит внутри объёма (точка внутри грани+вектор наружу грани)
 sPoint_Temporary.X=sPoint_ShadowSourceCenter.X+sVector_Temporary.X*100.0;
 sPoint_Temporary.Y=sPoint_ShadowSourceCenter.Y+sVector_Temporary.Y*100.0;
 sPoint_Temporary.Z=sPoint_ShadowSourceCenter.Z+sVector_Temporary.Z*100.0;
 size=vector_CPolygon_ShadowBorder.size();
 for(n=0;n<size;n++)
 {
  CPolygon cPolygon_Temporary=vector_CPolygon_ShadowBorder[n];
  long pos=GetPointPositionPlane(sPoint_Temporary,cPolygon_Temporary.vector_SPoint[0],cPolygon_Temporary.sVector_Normal);
  if (pos>0)//изменим направление нормали
  {
   cPolygon_Temporary.sVector_Normal.X=-cPolygon_Temporary.sVector_Normal.X;
   cPolygon_Temporary.sVector_Normal.Y=-cPolygon_Temporary.sVector_Normal.Y;
   cPolygon_Temporary.sVector_Normal.Z=-cPolygon_Temporary.sVector_Normal.Z;
   vector_CPolygon_ShadowBorder[n].sVector_Normal=cPolygon_Temporary.sVector_Normal;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//создание полигона тени
//----------------------------------------------------------------------------------------------------
CPolygon CreateShadowPolygon(const CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,const vector<CPolygon> &vector_CPolygon_ShadowBorder)
{
 long n;
 CPolygon cPolygon_ShadowPoint;//полигон, содержащий в себе точки тени
 //добавим все точки полигона, лежащие внутри объёма
 long size_sd=cPolygon_ShadowDestination.vector_SPoint.size();
 long size_sb=vector_CPolygon_ShadowBorder.size();
 for(n=0;n<size_sd;n++)
 {
  SPoint sPoint_Temporary=cPolygon_ShadowDestination.vector_SPoint[n];
  //проверим, лежит ли эта точка внутри теневого объёма
  bool error=false;
  for(long m=0;m<size_sb;m++)
  {  
   CPolygon cPolygon_Temporary=vector_CPolygon_ShadowBorder[m];
   long pos=GetPointPositionPlane(sPoint_Temporary,cPolygon_Temporary.vector_SPoint[0],cPolygon_Temporary.sVector_Normal);
   if (pos>0) 
   {
    error=true;
    break;
   }
  }
  if (error==false)//точка лежит внутри объёма
  {
   //посмотрим, может быть уже есть такая точка 
   bool enable=true;
   long size_sp=cPolygon_ShadowPoint.vector_SPoint.size();
   for(long v=0;v<size_sp;v++)
   {
    SPoint sPoint_D=cPolygon_ShadowPoint.vector_SPoint[v];
    if ((fabs(sPoint_D.X-sPoint_Temporary.X))<=S_EPS && (fabs(sPoint_D.Z-sPoint_Temporary.Z))<=S_EPS && (fabs(sPoint_D.Y-sPoint_Temporary.Y))<=S_EPS) 
    { 
     enable=false;
     break;
    }
   }
   if (enable==true)//точка внутри теневого объёма
   {
    cPolygon_ShadowPoint.vector_SPoint.push_back(sPoint_Temporary);
   }
  }
 }
 //добавим точки пересечения с теневым объёмом
 for(n=0;n<size_sb;n++)
 {
  CPolygon cPolygon_Temporary=vector_CPolygon_ShadowBorder[n];
  for(long m=0;m<size_sd;m++)
  {
   long i=m;
   long j=m+1;
   if (j>=size_sd) j-=size_sd;
   SPoint sPoint_A=cPolygon_ShadowDestination.vector_SPoint[i];
   SPoint sPoint_B=cPolygon_ShadowDestination.vector_SPoint[j];
   SPoint sPoint_Return;
   //есть пересечение с плоскостью грани теневого объёма
   if (GetLineIntersectionPlane(sPoint_A,sPoint_B,cPolygon_Temporary.vector_SPoint[0],cPolygon_Temporary.sVector_Normal,2,sPoint_Return)==true)
   {
    //убедимся, что точка пересечения попадает в грань теневого объёма
    if (IsPointInPolygon(sPoint_Return,cPolygon_Temporary)==true)
    {
     //посмотрим, может быть уже есть такая точка 
     bool enable=true;
	 long size_sp=cPolygon_ShadowPoint.vector_SPoint.size();
     for(long v=0;v<size_sp;v++)
     {
      SPoint sPoint_D=cPolygon_ShadowPoint.vector_SPoint[v];
      if ((fabs(sPoint_D.X-sPoint_Return.X))<=S_EPS && (fabs(sPoint_D.Z-sPoint_Return.Z))<=S_EPS && (fabs(sPoint_D.Y-sPoint_Return.Y))<=S_EPS) 
      { 
       enable=false;
       break;
      }
     }
     if (enable==true)
     {
      cPolygon_ShadowPoint.vector_SPoint.push_back(sPoint_Return);
     }
    }
   }
  }
 }
 return(cPolygon_ShadowPoint);
}
//----------------------------------------------------------------------------------------------------
//создание ортов системы координат, связанной с полигоном
//----------------------------------------------------------------------------------------------------
void CreateOrtsXYZ(const CPolygon &cPolygon,SPoint &sPoint_CenterOfCoordSystem,SVector &sVector_OrtsX,SVector &sVector_OrtsY,SVector &sVector_OrtsZ)
{
 sPoint_CenterOfCoordSystem=cPolygon.vector_SPoint[0];
 sVector_OrtsY=cPolygon.sVector_Normal;
 sVector_OrtsX.X=cPolygon.vector_SPoint[1].X-cPolygon.vector_SPoint[0].X;
 sVector_OrtsX.Y=cPolygon.vector_SPoint[1].Y-cPolygon.vector_SPoint[0].Y;
 sVector_OrtsX.Z=cPolygon.vector_SPoint[1].Z-cPolygon.vector_SPoint[0].Z;
 sVector_OrtsX=NormalizeVector(sVector_OrtsX);
 sVector_OrtsZ=VectorProduct(sVector_OrtsX,sVector_OrtsY);
 sVector_OrtsZ=NormalizeVector(sVector_OrtsZ);
}
//----------------------------------------------------------------------------------------------------
//перевод точек полигона в заданную систему координат
//----------------------------------------------------------------------------------------------------
void ConvertPolygonToOrtsXYZ(const SPoint &sPoint_CenterOfCoordSystem,const SVector &sVector_OrtsX,const SVector &sVector_OrtsY,const SVector &sVector_OrtsZ,CPolygon &cPolygon)
{
 long n;
 long size=cPolygon.vector_SPoint.size();
 for(n=0;n<size;n++)
 {
  SPoint sPoint_Temporary=cPolygon.vector_SPoint[n];
  SVector sVector_Radius=CreateVector(sPoint_Temporary.X-sPoint_CenterOfCoordSystem.X,sPoint_Temporary.Y-sPoint_CenterOfCoordSystem.Y,sPoint_Temporary.Z-sPoint_CenterOfCoordSystem.Z);
  cPolygon.vector_SPoint[n].X=ScalarProduct(sVector_Radius,sVector_OrtsX);
  cPolygon.vector_SPoint[n].Z=ScalarProduct(sVector_Radius,sVector_OrtsZ);
  cPolygon.vector_SPoint[n].Y=0;//т.к. на нормаль проекция отсутствует всегда
 }
}
//----------------------------------------------------------------------------------------------------
//найти и добавить к полигону тени все точки пересечения объёма с плоскостью полигона, на который падает тень и попадающие внутрь этого полигона, переведённые в систему координат
//----------------------------------------------------------------------------------------------------
void FindAndConvertIntersectionPointOfShadowSourceForShadowDestination(const SPoint &sPoint_CenterOfCoordSystem,const SVector &sVector_OrtsX,const SVector &sVector_OrtsY,const SVector &sVector_OrtsZ,const CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,const CPolygon &cPolygon_ShadowDestinationProject,CPolygon &cPolygon_ShadowPoint,const SPoint &sPoint_Lighting)
{ 
 long size_ss=cPolygon_ShadowSource.vector_SPoint.size();
 for(long n=0;n<size_ss;n++)
 {
  SPoint sPoint_Temporary=cPolygon_ShadowSource.vector_SPoint[n];
  SPoint sPoint_Return;
  if (GetLineIntersectionPlane(sPoint_Lighting,sPoint_Temporary,cPolygon_ShadowDestination.vector_SPoint[0],cPolygon_ShadowDestination.sVector_Normal,0,sPoint_Return)==true)//есть точка пересечения
  {
   //переведём в новую систему координат
   SVector sVector_Radius=CreateVector(sPoint_Return.X-sPoint_CenterOfCoordSystem.X,sPoint_Return.Y-sPoint_CenterOfCoordSystem.Y,sPoint_Return.Z-sPoint_CenterOfCoordSystem.Z);
   sPoint_Temporary.X=ScalarProduct(sVector_Radius,sVector_OrtsX);
   sPoint_Temporary.Z=ScalarProduct(sVector_Radius,sVector_OrtsZ);
   sPoint_Temporary.Y=0;//т.к. на нормаль проекция отсутствует всегда
   if (IsPointInPolygonXZ(sPoint_Temporary,cPolygon_ShadowDestinationProject)==true)//точка внутри полигона
   {
    //посмотрим, может быть уже есть такая точка 
    bool enable=true;
	long size_sp=cPolygon_ShadowPoint.vector_SPoint.size();
    for(long v=0;v<size_sp;v++)
    {
     SPoint sPoint_D=cPolygon_ShadowPoint.vector_SPoint[v];
     if ((fabs(sPoint_D.X-sPoint_Temporary.X))<=S_EPS && (fabs(sPoint_D.Z-sPoint_Temporary.Z))<=S_EPS)  
     { 
      enable=false;
      break;
     }
    }
    if (enable==true)
    {
     cPolygon_ShadowPoint.vector_SPoint.push_back(sPoint_Temporary);
    }
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//сортировка точек полигона вокруг центра в плоскости XZ
//----------------------------------------------------------------------------------------------------
void SortPointsOfPolygonInPlaneXZ(CPolygon &cPolygon)
{
 long n;
 long v;
 //теперь сортируем точки
 CPolygon cPolygon_Output;//точки, отсортированные вокруг центра
 //найдём центральную точку тени
 SPoint sPoint_Center;
 sPoint_Center.X=0;
 sPoint_Center.Y=0;
 sPoint_Center.Z=0;
 long size=cPolygon.vector_SPoint.size();
 for(n=0;n<size;n++)
 {
  sPoint_Center.X+=cPolygon.vector_SPoint[n].X;
  sPoint_Center.Z+=cPolygon.vector_SPoint[n].Z;
  cPolygon_Output.vector_SPoint.push_back(cPolygon.vector_SPoint[n]);
 }
 sPoint_Center.X/=static_cast<double>(cPolygon.vector_SPoint.size());
 sPoint_Center.Z/=static_cast<double>(cPolygon.vector_SPoint.size());
 //узнаем параметры точек
 vector<double> vector_Angle;
 vector<double> vector_Length;
 vector_Angle.resize(cPolygon_Output.vector_SPoint.size());
 vector_Length.resize(cPolygon_Output.vector_SPoint.size());
 size=cPolygon_Output.vector_SPoint.size();
 for(v=0;v<size;v++)
 {
  SPoint sPoint=cPolygon_Output.vector_SPoint[v];
  double dx=sPoint.X-sPoint_Center.X;
  double dz=sPoint.Z-sPoint_Center.Z;
  vector_Length[v]=sqrt(dx*dx+dz*dz);
  vector_Angle[v]=0;
  vector_Angle[v]=atan2(dz,dx)*180.0/M_PI;
 }
 //теперь сортируем точки
 size=cPolygon_Output.vector_SPoint.size();
 for(v=0;v<size;v++)
 {
  for(long u=0;u<size-1;u++)
  {
   double angle1=vector_Angle[u];
   double angle2=vector_Angle[u+1];
   double length1=vector_Length[u];
   double length2=vector_Length[u+1];
   if (angle1>angle2)
   {
    SPoint sPoint_1=cPolygon_Output.vector_SPoint[u];
    SPoint sPoint_2=cPolygon_Output.vector_SPoint[u+1];
    cPolygon_Output.vector_SPoint[u]=sPoint_2;
    cPolygon_Output.vector_SPoint[u+1]=sPoint_1;
    vector_Angle[u]=angle2;
    vector_Angle[u+1]=angle1;
    vector_Length[u]=length2;
    vector_Length[u+1]=length1;
    continue;
   }
   if (angle1==angle2)
   {
    if (length1>length2)
    {
     SPoint sPoint_1=cPolygon_Output.vector_SPoint[u];
     SPoint sPoint_2=cPolygon_Output.vector_SPoint[u+1];
     cPolygon_Output.vector_SPoint[u]=sPoint_2;
     cPolygon_Output.vector_SPoint[u+1]=sPoint_1;
     vector_Angle[u]=angle2;
     vector_Angle[u+1]=angle1;
     vector_Length[u]=length2;
     vector_Length[u+1]=length1;
     continue;
    }
   }
  }
 }
 cPolygon_Output.sVector_Normal=CalculateNormalVector(cPolygon_Output);
 cPolygon=cPolygon_Output;
}
//----------------------------------------------------------------------------------------------------
//разбивка полигона на фрагменты по полигону тени в плоскости XZ
//----------------------------------------------------------------------------------------------------
bool CreatePolygonFragmentInPlaneXZ(const CPolygon &cPolygon_ShadowSource,const CPolygon &cPolygon_ShadowDestination,CPolygon &cPolygon_ShadowDestinationProject,CPolygon &cPolygon_Shadow,vector<CPolygon> &vector_CPolygon_Fragment)
{ 
 long u;
 //разобъём полигон на фрагменты
 vector_CPolygon_Fragment.push_back(cPolygon_Shadow);//добавим саму тень в список
 long size_s=cPolygon_Shadow.vector_SPoint.size();
 //найдём центральную точку тени
 SPoint sPoint_CenterShadow;
 sPoint_CenterShadow.X=0;
 sPoint_CenterShadow.Y=0;
 sPoint_CenterShadow.Z=0;
 for(long n=0;n<size_s;n++)
 {
  sPoint_CenterShadow.X+=cPolygon_Shadow.vector_SPoint[n].X;
  sPoint_CenterShadow.Z+=cPolygon_Shadow.vector_SPoint[n].Z;
 }
 sPoint_CenterShadow.X/=static_cast<double>(cPolygon_Shadow.vector_SPoint.size());
 sPoint_CenterShadow.Z/=static_cast<double>(cPolygon_Shadow.vector_SPoint.size());

 for(long v=0;v<size_s;v++)
 {
  long v1=v;
  long v2=v+1;
  if (v2>=size_s) v2-=size_s;
  CPolygon cPolygon_Fragment;//фрагмент
  SPoint sPoint_A=cPolygon_Shadow.vector_SPoint[v1];
  SPoint sPoint_B=cPolygon_Shadow.vector_SPoint[v2];
  long CenterShadowPosition=GetPointPositionInPlaneXZ(sPoint_A,sPoint_B,sPoint_CenterShadow);
  if (CenterShadowPosition==0)//тени нет, так как полигон тени стянулся в линию
  {
   vector_CPolygon_Fragment.clear();//удалим всё что добавили
   vector_CPolygon_Fragment.push_back(cPolygon_ShadowDestination);//добавим сам полигон
   return(false);
  }
  //создаём фрагмент
  //добавляем точки, лежащие снаружи 
  long size_sdp=cPolygon_ShadowDestinationProject.vector_SPoint.size();
  for(u=0;u<size_sdp;u++)
  {
   SPoint sPoint_C=cPolygon_ShadowDestinationProject.vector_SPoint[u];
   long PointCPosition=GetPointPositionInPlaneXZ(sPoint_A,sPoint_B,sPoint_C);
   if (PointCPosition*CenterShadowPosition<=0)//по разные стороны
   {
    //посмотрим, может быть уже есть такая точка 
    bool enable=true;
	long size_f=cPolygon_Fragment.vector_SPoint.size();
    for(long n=0;n<size_f;n++)
    {
     SPoint sPoint_D=cPolygon_Fragment.vector_SPoint[n];
     if ((fabs(sPoint_D.X-sPoint_C.X))<=S_EPS && (fabs(sPoint_D.Z-sPoint_C.Z))<=S_EPS)
     { 
      enable=false;
      break;
     }
    }
    if (enable==true)
    {
     cPolygon_Fragment.vector_SPoint.push_back(sPoint_C);
    }
   }
  }
  //добавляем точки пересечения с прямой выбранной стороны полигона тени
  for(u=0;u<size_sdp;u++)
  {
   long u1=u;
   long u2=u+1;
   if (u2>=size_sdp) u2-=size_sdp;
   SPoint sPoint_C=cPolygon_ShadowDestinationProject.vector_SPoint[u1];
   SPoint sPoint_D=cPolygon_ShadowDestinationProject.vector_SPoint[u2];
   SPoint sPoint_Intersection;
   if (GetLineIntersectionPieceInPlaneXZ(sPoint_A,sPoint_B,sPoint_C,sPoint_D,sPoint_Intersection)==true)//есть пересечение
   {
    //посмотрим, может быть уже есть такая точка 
    bool enable=true;
	long size_f=cPolygon_Fragment.vector_SPoint.size();
    for(long n=0;n<size_f;n++)
    {
     SPoint sPoint_E=cPolygon_Fragment.vector_SPoint[n];
     if ((fabs(sPoint_E.X-sPoint_Intersection.X))<=S_EPS && (fabs(sPoint_E.Z-sPoint_Intersection.Z))<=S_EPS)
     { 
      enable=false;
      break;
     }
    }
    if (enable==true)
    {
     cPolygon_Fragment.vector_SPoint.push_back(sPoint_Intersection);
    }
   }
  }
  if (cPolygon_Fragment.vector_SPoint.size()==0) continue;//нет фрагмента
  //модифицируем наш полигон
  CPolygon cPolygon_NewBasic;
  for(long n=0;n<size_sdp;n++)
  {
   long u1=n;
   long u2=n+1;
   if (u2>=size_sdp) u2-=size_sdp;
   SPoint sPoint_C=cPolygon_ShadowDestinationProject.vector_SPoint[u1];
   SPoint sPoint_D=cPolygon_ShadowDestinationProject.vector_SPoint[u2];
   long PointCPosition=GetPointPositionInPlaneXZ(sPoint_A,sPoint_B,sPoint_C);
   SPoint sPoint_Intersection;
   if (GetLineIntersectionPieceInPlaneXZ(sPoint_A,sPoint_B,sPoint_C,sPoint_D,sPoint_Intersection)==true)//есть пересечение
   {
    if (PointCPosition*CenterShadowPosition>=0)//точка C внутри
    {
     cPolygon_NewBasic.vector_SPoint.push_back(sPoint_C);
    }
    if (PointCPosition==0) continue;
    cPolygon_NewBasic.vector_SPoint.push_back(sPoint_Intersection);
   }
   else
   {
    if (PointCPosition*CenterShadowPosition>=0)//точка C внутри
    {
     cPolygon_NewBasic.vector_SPoint.push_back(sPoint_C);
    }
   }
  }
  cPolygon_ShadowDestinationProject=cPolygon_NewBasic;
  //сортируем точки полигона фрагмента
  SortPointsOfPolygonInPlaneXZ(cPolygon_Fragment);
  if (cPolygon_Fragment.vector_SPoint.size()>=3) vector_CPolygon_Fragment.push_back(cPolygon_Fragment);//добавим фрагмент в список
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//обратное преобразование для всех точек полигонов
//----------------------------------------------------------------------------------------------------
void UnconvertPolygonTo3D(const SPoint &sPoint_CenterOfCoordSystem,const SVector &sVector_OrtsX,const SVector &sVector_OrtsY,const SVector &sVector_OrtsZ,CPolygon &cPolygon,const CPolygon &cPolygon_ShadowDestination)
{
 long size=cPolygon.vector_SPoint.size();
 for(long v=0;v<size;v++)
 {
  SPoint sPoint=cPolygon.vector_SPoint[v];
  cPolygon.vector_SPoint[v].X=sPoint_CenterOfCoordSystem.X+sVector_OrtsX.X*sPoint.X+sVector_OrtsY.X*sPoint.Y+sVector_OrtsZ.X*sPoint.Z;
  cPolygon.vector_SPoint[v].Y=sPoint_CenterOfCoordSystem.Y+sVector_OrtsX.Y*sPoint.X+sVector_OrtsY.Y*sPoint.Y+sVector_OrtsZ.Y*sPoint.Z;
  cPolygon.vector_SPoint[v].Z=sPoint_CenterOfCoordSystem.Z+sVector_OrtsX.Z*sPoint.X+sVector_OrtsY.Z*sPoint.Y+sVector_OrtsZ.Z*sPoint.Z;
 }
 cPolygon.sVector_Normal=CalculateNormalVector(cPolygon);
 if (ScalarProduct(cPolygon_ShadowDestination.sVector_Normal,cPolygon.sVector_Normal)<0)//нужно поменять направление обхода вершин
 {  
  for(long v=0;v<size/2;v++)
  {
   SPoint sPoint1=cPolygon.vector_SPoint[size-v-1];
   SPoint sPoint2=cPolygon.vector_SPoint[v];
   cPolygon.vector_SPoint[size-v-1]=sPoint2;
   cPolygon.vector_SPoint[v]=sPoint1;
  }
 }
 cPolygon.sVector_Normal=cPolygon_ShadowDestination.sVector_Normal;
}

//----------------------------------------------------------------------------------------------------
//сформировать фрагменты тени
//----------------------------------------------------------------------------------------------------
bool GenerateShadow(CPolygon cPolygon_ShadowSource,CPolygon cPolygon_ShadowDestination,SPoint sPoint_Lighting,vector<CPolygon> &vector_CPolygon_Fragment)
{
 //очистим выходной список фрагментов
 vector_CPolygon_Fragment.clear();
 //вычислим нормали к полигонам
 cPolygon_ShadowDestination.sVector_Normal=CalculateNormalVector(cPolygon_ShadowDestination);
 cPolygon_ShadowSource.sVector_Normal=CalculateNormalVector(cPolygon_ShadowSource);
 //проверим, что оба полигона имеют не менее трёх точек
 if (CheckCorrectnessOfPolygons(cPolygon_ShadowSource,cPolygon_ShadowDestination,vector_CPolygon_Fragment)==false) return(false);
 //проверим, может ли источник света освещать полигон (нормаль должна смотреть в сторону источника света)
 if (CheckIlluminationPossibility(cPolygon_ShadowDestination,sPoint_Lighting,vector_CPolygon_Fragment)==false) return(false);
 //скорректируем полигон, отбрасывающий тень (он ведь может пересекать плоскость тени) и проверим, может ли новый полигон быть источником тени
 if (CorrectionOfShadowSourcePolygon(cPolygon_ShadowSource,cPolygon_ShadowDestination,vector_CPolygon_Fragment)==false) return(false);
 //проверим, не находятся ли полигон на который отбрасывается тень и источник света с одной стороны от полигона, отбрасывающего тень (тогда тени нет)
 if (CheckSideToShadowSourcePolygon(cPolygon_ShadowSource,cPolygon_ShadowDestination,sPoint_Lighting,vector_CPolygon_Fragment)==false) return(false);
 //формируем теневой объём
 vector<CPolygon> vector_CPolygon_ShadowBorder=CreateShadowVolume(cPolygon_ShadowSource,cPolygon_ShadowDestination,sPoint_Lighting);
 //корректируем нормали граней теневого объёма, чтобы они смотрени наружу из теневого объёма
 CorrectionShadowVolume(cPolygon_ShadowSource,vector_CPolygon_ShadowBorder,sPoint_Lighting);
 //найдём полигон тени
 CPolygon cPolygon_ShadowPoint=CreateShadowPolygon(cPolygon_ShadowSource,cPolygon_ShadowDestination,vector_CPolygon_ShadowBorder);
 //переведём все созданные точки тени в двумерную систему координат, связанную с плоскостью полигона, на который падает тень
 //создадим орты системы координат, связанной с полигоном, на который падает тень
 SPoint sPoint_CenterOfCoordSystem;
 SVector sVector_OrtsX;
 SVector sVector_OrtsY;
 SVector sVector_OrtsZ;
 CreateOrtsXYZ(cPolygon_ShadowDestination,sPoint_CenterOfCoordSystem,sVector_OrtsX,sVector_OrtsY,sVector_OrtsZ);
 //переведём полигон тени в новую систему координат
 ConvertPolygonToOrtsXYZ(sPoint_CenterOfCoordSystem,sVector_OrtsX,sVector_OrtsY,sVector_OrtsZ,cPolygon_ShadowPoint);
 //переведём полигон, на который падает тень в новую систему координат
 CPolygon cPolygon_ShadowDestinationProject=cPolygon_ShadowDestination;//полигон, на который отбрасывается тень в своей собственной двухмерной системе координат
 ConvertPolygonToOrtsXYZ(sPoint_CenterOfCoordSystem,sVector_OrtsX,sVector_OrtsY,sVector_OrtsZ,cPolygon_ShadowDestinationProject);
 //найдём и переведём в новую систему координат все точки пересечения теневого объёма с плоскостью полигона, на который падает тень и попадающие внутрь этого полигона ()
 FindAndConvertIntersectionPointOfShadowSourceForShadowDestination(sPoint_CenterOfCoordSystem,sVector_OrtsX,sVector_OrtsY,sVector_OrtsZ,cPolygon_ShadowSource,cPolygon_ShadowDestination,cPolygon_ShadowDestinationProject,cPolygon_ShadowPoint,sPoint_Lighting);
 //проверим, что полигон с точками тени имеет не менее трёх точек
 if (CheckCorrectnessOfPolygons(cPolygon_ShadowPoint,cPolygon_ShadowDestination,vector_CPolygon_Fragment)==false) return(false);

 //теперь у нас есть все точки тени в полигоне cPolygon_ShadowPoint, однако, они идут в произвольном порядке
 
 //сортируем точки тени вокруг геометрического центра в плоскости XZ
 SortPointsOfPolygonInPlaneXZ(cPolygon_ShadowPoint);
 //разбиваем полигон, на который падает тень, на фрагменты
 if (CreatePolygonFragmentInPlaneXZ(cPolygon_ShadowSource,cPolygon_ShadowDestination,cPolygon_ShadowDestinationProject,cPolygon_ShadowPoint,vector_CPolygon_Fragment)==false) return(false);
 //выполняем обратное преобразование координат в трёхмерное пространство
 for(long n=0;n<vector_CPolygon_Fragment.size();n++) UnconvertPolygonTo3D(sPoint_CenterOfCoordSystem,sVector_OrtsX,sVector_OrtsY,sVector_OrtsZ,vector_CPolygon_Fragment[n],cPolygon_ShadowDestination);
 return(true); 
}


