/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Point4.cpp
Purpose: Implementation for the Point class, which is a 4-D point.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#include "Point4.h"
#include "Vector4.h"

Point4::Point4(void)
{
  x = 0; 
  y = 0;
  z = 0; 
  w = 1;
}
  
    // Copy constructor, copies every component from the other Point4
Point4::Point4(const Point4& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
}
  
    // Non-Default constructor, self-explanatory
Point4::Point4(f32 xx, f32 yy, f32 zz, f32 ww)
{
  x = xx;
  y = yy;
  z = zz;
  w = ww;
}
  
    // Assignment operator, copies every component from the other Point4
Point4& Point4::operator=(const Point4& rhs)
{
  if(&rhs == this)
    return *this;

  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  
  return *this;
}
  
    // Unary negation operator, negates every component and returns a copy
Point4 Point4::operator-(void) const
{
  Point4 p;
  p.x = -x;
  p.y = -y;
  p.z = -z;
  p.w = -w;
  return p;
}
  
    // Binary subtraction operator, Subtract two Point4s and you get a Vector4
Vector4 Point4::operator-(const Point4&  rhs) const
{
  Vector4 v;
  v.x = x - rhs.x;
  v.y = y - rhs.y;
  v.z = z - rhs.z;
  v.w = w - rhs.w;
  return v;
}
  
    // Basic vector math operations with points, you can add a Vector4 to a Point4, or subtract
    // a Vector4 from a Point4
Point4  Point4::operator+ (const Vector4& rhs) const
{
  Point4 p;
  p.x = x + rhs.x;
  p.y = y + rhs.y;
  p.z = z + rhs.z;
  p.w = w + rhs.w;
  return p;
}
Point4  Point4::operator- (const Vector4& rhs) const
{
  Point4 p;
  p.x = x - rhs.x;
  p.y = y - rhs.y;
  p.z = z - rhs.z;
  p.w = w - rhs.w;
  return p;
}
  
    // Same as previous two operators, just modifies the original instead of returning a copy
Point4& Point4::operator+=(const Vector4& rhs)
{
  x = x + rhs.x;
  y = y + rhs.y;
  z = z + rhs.z;
  w = w + rhs.w;
  return *this;
}

Point4& Point4::operator-=(const Vector4& rhs)
{
  x = x - rhs.x;
  y = y - rhs.y;
  z = z - rhs.z;
  w = w - rhs.w;
  return *this;
}
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is within a certain range
    // in which case we say they are equivalent.
bool Point4::operator==(const Point4& rhs) const
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

bool Point4::operator!=(const Point4& rhs) const
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
  
    // Sets x,y,z to zeroes, w to defined value
void Point4::Zero(void)
{
  x = 0;
  y = 0;
  z = 0;
  w = 1;
}

void Point4::Print(void) const
{
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n",x,y,z,w);
}
