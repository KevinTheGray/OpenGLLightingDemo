/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Light.h
Purpose: Declaration class for the Light class.  Used for OpenGL lighting.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/

#pragma once

#include "Utilities.h"
#include "Vector4.h"

//Global values

// Light class that encapsulates the OpenGL Lights
class Light
{
public:
	
	enum LightTypes {
		GL_POSITIONAL,
		GL_DIRECTIONAL,
		GL_SPOTLIGHT,
    OFF
	};
  int type;
	//bool for displaying a light
	int display_light;
	
	// Geometric attributes of light
	Vector4  position;
	Vector4  direction;
	
	// spotlight properties
	float	  spotCutoff;
	
	// Color values 
	Vector4  ambient;
	Vector4  diffuse;
	Vector4  specular;

	GLint type_loc;
	GLint ambient_loc;
	GLint diffuse_loc;
	GLint specular_loc;
	GLint position_loc;
	GLint direction_loc;

	
	// Constructor
	Light( LightTypes _t ) : type(_t) {spotCutoff = 0;};
	
	// setter methods for light attributes
	void setPosition( Vector4 pos );
	void setDirection(  Vector4 dir );
	
	void setAmbient(  Vector4 _amb );
	void setDiffuse(  Vector4 _amb );
	void setSpecular( Vector4 _amb );

	void setSpotCutoff( float cutoff );
	
};