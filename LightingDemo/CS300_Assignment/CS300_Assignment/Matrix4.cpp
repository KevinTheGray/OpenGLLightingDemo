/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Matrix.cpp
Purpose: Implementation for the Matrix class, which is a 4x4 matrix.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#include "Matrix4.h"
#include "Vector4.h"
#include "Point4.h"

Matrix4::Matrix4()
{
  for(int i = 0; i < 16; i++)
  {
    v[i] = 0;
  }
}

Matrix4::Matrix4(const Matrix4& rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] = rhs.v[i];
  }
}

Matrix4::Matrix4(f32 mm00, f32 mm01, f32 mm02, f32 mm03,
          f32 mm10, f32 mm11, f32 mm12, f32 mm13,
          f32 mm20, f32 mm21, f32 mm22, f32 mm23,
          f32 mm30, f32 mm31, f32 mm32, f32 mm33)
{
  m[0][0] = mm00;
  m[0][1] = mm01;
  m[0][2] = mm02;
  m[0][3] = mm03;
  m[1][0] = mm10;
  m[1][1] = mm11;
  m[1][2] = mm12;
  m[1][3] = mm13;
  m[2][0] = mm20;
  m[2][1] = mm21;
  m[2][2] = mm22;
  m[2][3] = mm23;
  m[3][0] = mm30;
  m[3][1] = mm31;
  m[3][2] = mm32;
  m[3][3] = mm33;
}

Matrix4::Matrix4(float *values)
{
  for(unsigned i = 0; i < 16; i++)
	{
    v[i] = values[i];
	}
}

void Matrix4::Print(void) const
{
  printf("--------------------------\n");
  printf("%5.3f %5.3f %5.3f %5.3f\n", m00, m01, m02, m03 );
  printf("%5.3f %5.3f %5.3f %5.3f\n", m10, m11, m12, m13 );
  printf("%5.3f %5.3f %5.3f %5.3f\n", m20, m21, m22, m23 );
  printf("%5.3f %5.3f %5.3f %5.3f\n", m30, m31, m32, m33 );
  printf("--------------------------\n");
}

Matrix4& Matrix4::operator=(const Matrix4& rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] = rhs.v[i];
  }
  return *this;
}

Vector4 Matrix4::operator*(const Vector4& rhs) const
{
  Vector4 v(rhs);
  for(int i = 0; i < 4; i++)
  {
    v.v[i] = 0;
    for(int j = 0; j < 4; j++)
	{
	  v.v[i] += m[j][i] * rhs.v[j];
	  //printf("v[%d] += m[%d][%d] * rhs.v[%d]\n", i, i, j, j);
	  //printf("%f = %f * %f\n", v.v[i], m[i][j], rhs.v[j]);
	}
  }
  return v;
}
Point4 Matrix4::operator*(const Point4& rhs) const
{
  Point4 v;
  v.w = 0;
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
	{
	  v.v[i] += m[i][j] * rhs.v[j];
	  //printf("v[%d] += m[%d][%d] * rhs.v[%d]\n", i, i, j, j);
	  //printf("%f = %f * %f\n", v.v[i], m[i][j], rhs.v[j]);
	}
  }
  return v;
}
  
    // Basic Matrix arithmetic operations
Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
  Matrix4 newMatrix;
  for(int i = 0; i < 16; i++)
  {
    newMatrix.v[i] = v[i] + rhs.v[i];
  }
  
  return newMatrix;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
  Matrix4 newMatrix;
  for(int i = 0; i < 16; i++)
  {
    newMatrix.v[i] = v[i] - rhs.v[i];
  }
  
  return newMatrix;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const
{
  Matrix4 newMatrix;
  int row = 0;
  int column = 0;
  
  for(int i = 0; i < 16; i++)
  {
    float dp = 0;
    for(int j = 0; j < 4; j++)
	{
      dp += m[i % 4][j] * rhs.m[j][i / 4]; 
	}
	newMatrix.m[row % 4][i / 4] = dp;
	//printf("%d, %d - %f\n", row%4, i/4, dp);
	row++;
  }
  
  return newMatrix;
}

// Similar to the three above except they modify
  // the original 
Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] += rhs.v[i];
  }
  return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] -= rhs.v[i];
  }
  return *this;}

Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
  int row = 0;
  int column = 0;
  
  Matrix4 temp = *this;
  *this = temp * rhs; 
  return *this;
}
  
   // Scale/Divide the entire matrix by a float
Matrix4 Matrix4::operator*(const f32 rhs) const
{
  Matrix4 m;
  for(int i = 0; i < 16; i++)
  {
    m.v[i] = v[i] * rhs;
  }
  return m;
}
Matrix4 Matrix4::operator/(const f32 rhs) const
{
  Matrix4 m;
  for(int i = 0; i < 16; i++)
  {
    m.v[i] = v[i] / rhs;
  }
  return m;
}
  
    // Same as previous 
Matrix4& Matrix4::operator*=(const f32 rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] = v[i] * rhs;
  }
  return *this;
}

Matrix4& Matrix4::operator/=(const f32 rhs)
{
  for(int i = 0; i < 16; i++)
  {
    v[i] = v[i] / rhs;
  }
  return *this;
}
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is within a certain range
    // in which case we say they are equivalent.
bool Matrix4::operator==(const Matrix4& rhs) const
{
  for(int i = 0; i < 16; i++)
  {
    if(abs(v[i] - rhs.v[i]) >= EPSILON)
	  return false;
  }
  
  return true;
}

bool Matrix4::operator!=(const Matrix4& rhs) const
{
  for(int i = 0; i < 16; i++)
  {
    if(abs(v[i] - rhs.v[i]) >= EPSILON)
	  return true;
  }
  
  return false;

}
  
    // Zeroes out the entire matrix
void Matrix4::Zero(void)
{
  for(int i = 0; i < 16; i++)
    v[i] = 0;
}
  
    // Builds the identity matrix
void Matrix4::Identity(void)
{
  Zero();
  for(int i = 0; i < 4; i ++)
    m[i][i] = 1;
}