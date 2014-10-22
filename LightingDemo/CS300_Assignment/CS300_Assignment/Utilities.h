/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Utilities.h
Purpose: Utility file, holds some headers and dclarations used throughout the project.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#pragma once

#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib, "glew32d.lib")

#include "glew.h"
#include "wglew.h"
#include "glui.h"
#include "Parser.h"
#include "MathFunctions.h"
//#include "glext.h"

// Typedefs for consistency
typedef signed short		   s16;
typedef unsigned short		 u16;
typedef unsigned char      u8;
typedef char               s8;
typedef unsigned int       u32;
typedef int                s32;
typedef unsigned long long u64;
typedef long long          s64;
typedef float              f32;
typedef double             f64;

// Do not change
static const f32 EPSILON = 0.0001f;
static const f32 PI = 3.1415926f;
static const f32 Z_FAR = 50.0f;
static const f32 Z_NEAR = 0.1f;
#define NUM_SHADER_PROGRAMS 2