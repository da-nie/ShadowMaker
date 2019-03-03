#include "polygon.h"

//---------------------------------------------------------------------------
SVector CalculateNormal(SPoint sp1,SPoint sp2,SPoint sp3)
{
 SVector sv1=CreateVector(sp3.X-sp1.X,sp3.Y-sp1.Y,sp3.Z-sp1.Z);
 SVector sv2=CreateVector(sp2.X-sp1.X,sp2.Y-sp1.Y,sp2.Z-sp1.Z);
 SVector Normal=NormalizeVector(VectorProduct(sv1,sv2));
 return(Normal);
}
SVector CalculateNormalVector(CPolygon &cPolygon)
{
 SVector sVector;
 sVector.X=0;
 sVector.Y=0;
 sVector.Z=0;
 if (cPolygon.vector_SPoint.size()<3) return(sVector);
 SPoint sPoint[3];
 sPoint[0]=cPolygon.vector_SPoint[0];
 sPoint[1]=cPolygon.vector_SPoint[1];
 sPoint[2]=cPolygon.vector_SPoint[2];
 sVector=CalculateNormal(sPoint[0],sPoint[1],sPoint[2]);
 return(sVector);
}