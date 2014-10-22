/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Vector4.cpp
Purpose: Implementation for the Vector class, which is a 4D Vector.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#include "Vector4.h"

Vector4::Vector4(void)
{
  v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 0;
}
  
    // Copy constructor, copies every component from the other Vector4
Vector4::Vector4(const Vector4& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
}
  
    // Non-Default constructor, self explanatory
Vector4::Vector4(f32 xx, f32 yy, f32 zz, f32 ww)
{
  x = xx; y= yy; z = zz; w = ww;
}
  
    // Assignment operator, does not need to handle self assignment
Vector4& Vector4::operator=(const Vector4& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  
  return *this;
}
  
    // Unary negation operator, negates all components and returns a copy
Vector4 Vector4::operator-(void) const
{
  Vector4 v;
  v.x = -x;
  v.y = -y;
  v.z = -z;
  v.w = -w;
  return v;
}
    
    // Basic Vector math operations. Add Vector to Vector B, or Subtract Vector A 
    // from Vector B, or multiply a vector with a scalar, or divide a vector by 
    // that scalar
Vector4 Vector4::operator+(const Vector4& rhs) const
{
  Vector4 v;
  v.x = x + rhs.x;
  v.y = y + rhs.y;
  v.z = z + rhs.z;
  v.w = w + rhs.w;
  return v;
}

Vector4 Vector4::operator-(const Vector4& rhs) const
{
  Vector4 v;
  v.x = x - rhs.x;
  v.y = y - rhs.y;
  v.z = z - rhs.z;
  v.w = w - rhs.w;
  return v;
}

Vector4 Vector4::operator*(const f32 rhs) const
{
  Vector4 v;
  v.x = x * rhs;
  v.y = y * rhs;
  v.z = z * rhs;
  v.w = w * rhs;
  return v;
}

Vector4 Vector4::operator/(const f32 rhs) const
{
  Vector4 v;
  v.x = x / rhs;
  v.y = y / rhs;
  v.z = z / rhs;
  v.w = w / rhs;
  return v;
}
  
    // Same as above, just stores the result in the original vector
Vector4& Vector4::operator+=(const Vector4& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
  return *this;
}

Vector4& Vector4::operator-=(const Vector4& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
  return *this;;
}

Vector4& Vector4::operator*=(const f32 rhs)
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  w *= rhs;
  return *this;
}

Vector4& Vector4::operator/=(const f32 rhs)
{
  x /= rhs;
  y /= rhs;
  z /= rhs;
  w /= rhs;
  return *this;
}
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is within a certain range
    // in which case we say they are equivalent.
bool Vector4::operator==(const Vector4& rhs) const
{
  if(abs(x - rhs.x) <= EPSILON &&
     abs(y - rhs.y) <= EPSILON &&
	 abs(z - rhs.z) <= EPSILON &&
	 abs(w - rhs.w) <= EPSILON   )
  {
     return true;
  }
 
   return false;
}

bool Vector4::operator!=(const Vector4& rhs) const
{
  if(abs(x - rhs.x) <= EPSILON &&
     abs(y - rhs.y) <= EPSILON &&
	 abs(z - rhs.z) <= EPSILON &&
	 abs(w - rhs.w) <= EPSILON   )
  {
     return false;
  }
  
  return true;
}
  
    // Computes the dot product with the other vector. Treat it as 3D vector.
f32 Vector4::Dot(const Vector4& rhs) const
{
  float total = (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  return total;
}
  
    // Computes the cross product with the other vector. Treat it as a 3D vector.
Vector4 Vector4::Cross(const Vector4& rhs) const
{
  Vector4 v;
  v.x = ((y * rhs.z) - (z * rhs.y));
  v.y = ((z * rhs.x) - (x * rhs.z));
  v.z = ((x * rhs.y) - (y * rhs.x));
  return v;
}
  
    // Computes the true length of the vector 
f32 Vector4::Length(void) const
{
  return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}
  
    // Computes the squared length of the vector
f32 Vector4::LengthSq(void) const
{
  return ((x * x) + (y * y) + (z * z) + (w * w));
}
  
    // Normalizes the vector to make the final vector be of length 1. If the length is zero
    // then this function should not modify anything.
void Vector4::Normalize(void)
{
  float length = this->Length();
  if(length <= EPSILON)
    return;

  x = x/length;
  y = y/length;
  z = z/length;
  w = w/length;  

	if(abs(x) < EPSILON)
		x = 0.0f;

	if(abs(y) < EPSILON)
		y = 0.0f;

	if(abs(z) < EPSILON)
		z = 0.0f;

	if(abs(w) < EPSILON)
		w = 0.0f;
}
  
    // Sets x,y,z to zeroes, w to defined value
void Vector4::Zero(void)
{
  x = 0;
  y = 0;
  z = 0;
  w = 0;
}

void Vector4::Print(void) const
{
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n",x,y,z,w);
}
