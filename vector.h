#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

//*************************************************************************************************************************
//������ ��� ������ � ���������
//*************************************************************************************************************************
struct SVector//��������� �������
{
 double X;//���������� �������
 double Y;
 double Z;
};
//*************************************************************************************************************************
//���������� �������:�������� �������
//���������:x,y,z-���������� �������
//������������ ��������:��������� ������
//��������������� ��������:���
//*************************************************************************************************************************
SVector CreateVector(double x,double y,double z);
//*************************************************************************************************************************
//���������� �������:���������� �������
//���������:v-������, ���������� ����������
//������������ ��������:������������� ������
//��������������� ��������:���
//*************************************************************************************************************************
SVector NormalizeVector(SVector v);
//*************************************************************************************************************************
//���������� �������:����������� ������� � ����������� ���
//���������:v-������, ���������� �����������
//������������ ��������:����� ������
//��������������� ��������:���
//*************************************************************************************************************************
SVector VectorToOrt(SVector v);
//*************************************************************************************************************************
//���������� �������:����������� ����� �������
//���������:v-������
//������������ ��������:����� �������
//��������������� ��������:���
//*************************************************************************************************************************
double GetNormaVector(SVector v);
//*************************************************************************************************************************
//���������� �������:���������� ���������� ������������
//���������:u,v-�������
//������������ ��������:��������� ������������
//��������������� ��������:���
//*************************************************************************************************************************
SVector VectorProduct(SVector u,SVector v);
//*************************************************************************************************************************
//���������� �������:���������� ���������� ������������ ���� ��������
//���������:u,v-�������
//������������ ��������:��������� ������������
//��������������� ��������:���
//*************************************************************************************************************************
double ScalarProduct(SVector u,SVector v);

#endif