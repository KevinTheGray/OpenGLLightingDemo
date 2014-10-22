/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Light.cpp
Purpose: Implementation for the Light class.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/

#include "Light.h"

void Light::setPosition( Vector4 pos )
{
	position = pos;
	return;
}

void Light::setDirection( Vector4 dir )
{
	direction = dir;
	return;
}

void Light::setAmbient( Vector4 _amb )
{
	ambient = _amb;
	return;
}

void Light::setDiffuse( Vector4 _diff )
{
	diffuse = _diff;
	return;
}

void Light::setSpecular( Vector4 _spec )
{
	specular = _spec;
	return;
}


void Light::setSpotCutoff( float cutoff )
{
	spotCutoff = cutoff;
	return;
}