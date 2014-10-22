/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Mesh.h
Purpose: Declaration for the Mesh class, which is a triangular mesh.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/

#pragma once
#include "Utilities.h"
#include "Vector4.h"
#include "Extents.h"
#include "Transform.h"

class Mesh
{
public:
	struct Face
	{
		Vector4 p1;
		Vector4 p2;
		Vector4 p3;
		Vector4 face_normal;
		Vector4 center;
	};

	//VBO ids
	GLuint vertex_ID;
	GLuint vertex_normals_ID;
	GLuint face_normals_ID;
	bool uses_vbo;

	//Face information
	Face *faces;            //List of faces
	unsigned face_count;    //Number of faces.
	unsigned *indices;      //Indices of faces
	unsigned *tex_indices;  //Indices of tex_coords
  unsigned *norm_indices;

	//Vertex information
	unsigned vertex_count;  //Number of vertices in the mesh
	unsigned normal_count;
	unsigned tex_coord_count;
	Vertex *vertices;       //List of the vertices
	Vertex *normals;
	Vertex *tex_coords;
	float *vertices_3f;     //List of the vertices to pass to a VBO
	Vertex *vertex_normals; //List of all the vertex normals;
	float *face_normals_3f;
	float *vertex_normals_3f;
	float *vertices_3f_a;
	float *normals_3f_a;
	float *tex_coords_2f_a;
  float *tangents_3f_a;
  float *binormals_3f_a;
	Extents x_extents;
	Extents y_extents;
	Extents z_extents;
	Vertex center; 

	//Lines representing vertex normals and face normals
	float *face_normal_lines_3f;
	float *vertex_normal_lines_3f;

	const char *file_name; 
	Mesh(const char *filename, bool use_vbo = false);  //Have to use this to create a mesh.
	void DebugMesh();
	Vector4 CalculateFaceNormal(Vector4 p1, Vector4 p2, Vector4 p3);
	Vector4 CalculateCenter(Vector4 p1, Vector4 p2, Vector4 p3);
	void CalculateVertexNormals(std::vector<std::vector<unsigned>> &face_membership);
  void CalculateTangentsAndBinormals();
	void CheckExtents(float x, float y, float z);
	void SetCenter();
	~Mesh();

private:
	Mesh()
	{
	}

};