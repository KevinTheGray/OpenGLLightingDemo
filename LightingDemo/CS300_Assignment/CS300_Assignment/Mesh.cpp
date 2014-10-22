/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Mesh.h
Purpose: Implementation for the Mesh class, which is a triangular mesh.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/

#include "Mesh.h"
#include "Parser.h"

Mesh::Mesh(const char *filename, bool use_vbo)
{
  std::ifstream obj_file(filename);
  std::vector<std::string> vertex_lines;
  std::vector<std::string> face_lines;
  std::vector<std::string> vertex_normal_lines;
  std::vector<std::string> tex_coord_lines;
  uses_vbo = false;
  while(obj_file.good())
  {
    std::string line;
    std::getline(obj_file, line);
    if(line.size())
    {
      if(line[0] == 'v')
      {
        if(line[1] == ' ')
          vertex_lines.push_back(line);
        if(line[1] == 'n')
          vertex_normal_lines.push_back(line);
        if(line[1] == 't')
          tex_coord_lines.push_back(line);
      }
      if(line[0] == 'f')
      face_lines.push_back(line);
    }
  }
  obj_file.close();
  
  vertex_count = vertex_lines.size();
  normal_count = vertex_normal_lines.size();
  tex_coord_count = tex_coord_lines.size();
  face_count = face_lines.size();

  //dynamically allocated things
  vertices = new Vertex[vertex_count];
  normals = new Vertex[normal_count];
  tex_coords = new Vertex[tex_coord_count];
  vertices_3f = new float[vertex_count * 3];
  vertex_normals = new Vertex[vertex_count];
  face_normals_3f = new float[face_count * 3];
  faces = new Face[face_count];
  indices = new unsigned int[face_count * 3];
  tex_indices = new unsigned int[face_count * 3];
  norm_indices = new unsigned int[face_count * 3];
  vertex_normal_lines_3f = new float[vertex_count * 3 * 2];
  face_normal_lines_3f = new float[face_count * 3 * 2];
  vertex_normals_3f = new float[vertex_count * 3];

  //Drawarray style
  vertices_3f_a = new float[face_count * 3 * 3]; //will hold all the vertices of the model, even repeating ones, 3 vertices for each face, 3 values for each vertex
  normals_3f_a  = new float[face_count * 3 * 3];
  tex_coords_2f_a = new float[face_count * 3 * 2]; //Only two values for each coordinate 
  tangents_3f_a = new float[face_count *3 * 3];
  binormals_3f_a = new float[face_count *3 * 3];

  //Will hold which faces the ith vertex is a member of;
  std::vector<std::vector<unsigned>> face_membership(vertex_count);

  //Read in the vertices
  for(unsigned i = 0; i < vertex_count; i++)
  {
    std::vector<std::string> tokens;
	Parser::ParseString(vertex_lines[i], tokens);
	//std::cout << "done" << std::endl;
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	float z = (float)atof(tokens[3].c_str());
	vertices[i].x = vertices_3f[3 * i] = x;
	vertices[i].y = vertices_3f[3 * i + 1] = y;
	vertices[i].z = vertices_3f[3 * i + 2] = z;
	CheckExtents(x, y, z);		
  }
  //Read in the normals
  for(unsigned i = 0; i < normal_count; i++)
  {
    std::vector<std::string> tokens;
    Parser::ParseString(vertex_normal_lines[i], tokens);
    //std::cout << "done" << std::endl;
    float x = (float)atof(tokens[1].c_str());
    float y = (float)atof(tokens[2].c_str());
    float z = (float)atof(tokens[3].c_str());
    normals[i].x  = x;
    normals[i].y  = y;
    normals[i].z  = z;		
  }
  //Read in the texture_coordinates
  for(unsigned i = 0; i < tex_coord_count; i++)
  {
    std::vector<std::string> tokens;
    Parser::ParseString(tex_coord_lines[i], tokens);
    //std::cout << "done" << std::endl;
    float x = (float)atof(tokens[1].c_str());
    float y = (float)atof(tokens[2].c_str());
    tex_coords[i].x = x;
    tex_coords[i].y = y;
  }
	SetCenter();
  //Read in the faces
  for(unsigned i = 0; i < face_count; i++)
  {
    std::vector<std::string> tokens;
    Parser::ParseString(face_lines[i], tokens);
		//Set up indices
    if(tokens.size() == 4)
    {
      indices[3 * i]     = atoi(tokens[1].c_str()) - 1;
      indices[3 * i + 1] = atoi(tokens[2].c_str()) - 1;
      indices[3 * i + 2] = atoi(tokens[3].c_str()) - 1;
    }
    else if(tokens.size() == 7)
    {
      unsigned multi = tokens.size()/3;
      indices[3 * i]     = atoi(tokens[0 * multi + 1].c_str()) - 1;
      indices[3 * i + 1] = atoi(tokens[1 * multi + 1].c_str()) - 1;
      indices[3 * i + 2] = atoi(tokens[2 * multi + 1].c_str()) - 1;

      tex_indices[3 * i]     = atoi(tokens[0 * multi + 2].c_str()) - 1;
      tex_indices[3 * i + 1] = atoi(tokens[1 * multi + 2].c_str()) - 1;
      tex_indices[3 * i + 2] = atoi(tokens[2 * multi + 2].c_str()) - 1;
    }
    else 
    {
      unsigned multi = tokens.size()/3;
      indices[3 * i]     = atoi(tokens[0 * multi + 1].c_str()) - 1;
      indices[3 * i + 1] = atoi(tokens[1 * multi + 1].c_str()) - 1;
      indices[3 * i + 2] = atoi(tokens[2 * multi + 1].c_str()) - 1;

      tex_indices[3 * i]     = atoi(tokens[0 * multi + 2].c_str()) - 1;
      tex_indices[3 * i + 1] = atoi(tokens[1 * multi + 2].c_str()) - 1;
      tex_indices[3 * i + 2] = atoi(tokens[2 * multi + 2].c_str()) - 1;

      norm_indices[3 * i]     = atoi(tokens[0 * multi + 3].c_str()) - 1;
      norm_indices[3 * i + 1] = atoi(tokens[1 * multi + 3].c_str()) - 1;
      norm_indices[3 * i + 2] = atoi(tokens[2 * multi + 3].c_str()) - 1;
    }
	//Set up faces
	faces[i].p1 = vertices[indices[3 * i]];
	faces[i].p2 = vertices[indices[3 * i + 1]];
	faces[i].p3 = vertices[indices[3 * i + 2]];
	//Update face member ship
	face_membership[indices[3 * i]].push_back(i);
	face_membership[indices[3 * i + 1]].push_back(i);
	face_membership[indices[3 * i + 2]].push_back(i);
	//Calculate Face normal
	faces[i].face_normal = CalculateFaceNormal(faces[i].p1, faces[i].p2, faces[i].p3);
    face_normals_3f[i * 3] = faces[i].face_normal.x;
    face_normals_3f[i * 3 + 1] = faces[i].face_normal.y;
    face_normals_3f[i * 3 + 2] = faces[i].face_normal.z;
		faces[i].center      = CalculateCenter(faces[i].p1, faces[i].p2, faces[i].p3);

    //Get the face normal lines to draw
    face_normal_lines_3f[i * 6 + 0] = faces[i].center.x;
    face_normal_lines_3f[i * 6 + 1] = faces[i].center.y;
    face_normal_lines_3f[i * 6 + 2] = faces[i].center.z;
    face_normal_lines_3f[i * 6 + 3] = faces[i].center.x + faces[i].face_normal.x;
    face_normal_lines_3f[i * 6 + 4] = faces[i].center.y + faces[i].face_normal.y;
    face_normal_lines_3f[i * 6 + 5] = faces[i].center.z + faces[i].face_normal.z;
  }
	CalculateVertexNormals(face_membership);
  //Get the vertex normal lines to draw
  for(unsigned i = 0; i < vertex_count; i++)
  {
    vertex_normal_lines_3f[i * 6 + 0] = vertices[i].x;
    vertex_normal_lines_3f[i * 6 + 1] = vertices[i].y;
    vertex_normal_lines_3f[i * 6 + 2] = vertices[i].z;
    vertex_normal_lines_3f[i * 6 + 3] = vertices[i].x + vertex_normals[i].x;
    vertex_normal_lines_3f[i * 6 + 4] = vertices[i].y + vertex_normals[i].y;
    vertex_normal_lines_3f[i * 6 + 5] = vertices[i].z + vertex_normals[i].z;
    //store the vertex_normals_3f
    vertex_normals_3f[i * 3 + 0] = vertex_normals[i].x;
    vertex_normals_3f[i * 3 + 1] = vertex_normals[i].y;
    vertex_normals_3f[i * 3 + 2] = vertex_normals[i].z;
  }

  //Ok generate a list of vertices, normals and txture coords for the vbos
  for(unsigned i = 0; i < face_count * 3; i++)
  {
    //vertices_3f_a
    unsigned vert_index = indices[i];
    vertices_3f_a[3 * i + 0] = vertices[vert_index].x;
    vertices_3f_a[3 * i + 1] = vertices[vert_index].y;
    vertices_3f_a[3 * i + 2] = vertices[vert_index].z;
    //normals_3f_a
    if(vertex_normal_lines.size())
    {
      unsigned norm_index  = norm_indices[i];
      normals_3f_a[3 * i + 0] = normals[norm_index].x;
      normals_3f_a[3 * i + 1] = normals[norm_index].y;
      normals_3f_a[3 * i + 2] = normals[norm_index].z;
    }
    else
    {
      normals_3f_a[3 * i + 0] = vertex_normals[vert_index].x;
      normals_3f_a[3 * i + 1] = vertex_normals[vert_index].y;
      normals_3f_a[3 * i + 2] = vertex_normals[vert_index].z;
    }
    //tex_coords_2f_a;
    if(tex_coord_lines.size())
    {
      unsigned tex_index  = tex_indices[i];
      tex_coords_2f_a[2 * i + 0] = tex_coords[tex_index].x;
      tex_coords_2f_a[2 * i + 1] = tex_coords[tex_index].y;
    }
    else
    {
      if(i % 6 == 0)
      {
        tex_coords_2f_a[2 * i + 0] = 0.0;
        tex_coords_2f_a[2 * i + 1] = 0.0;
      }
      if(i % 6 == 1)
      {
        tex_coords_2f_a[2 * i + 0] = 1.0;
        tex_coords_2f_a[2 * i + 1] = 0.0;
      }
      if(i % 6 == 2)
      {
        tex_coords_2f_a[2 * i + 0] = 1.0;
        tex_coords_2f_a[2 * i + 1] = 1.0;
      }
      if(i % 6 == 3)
      {
        tex_coords_2f_a[2 * i + 0] = 0.0;
        tex_coords_2f_a[2 * i + 1] = 0.0;
      }
      if(i % 6 == 4)
      {
        tex_coords_2f_a[2 * i + 0] = 1.0;
        tex_coords_2f_a[2 * i + 1] = 1.0;
      }
      if(i % 6 == 5)
      {
        tex_coords_2f_a[2 * i + 0] = 0.0;
        tex_coords_2f_a[2 * i + 1] = 1.0;
      }
    }
  }

  CalculateTangentsAndBinormals();

  if(use_vbo)
  {
    glGenBuffers(1, &vertex_ID);
    std::cout << "Vertex ID: " << vertex_ID << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vertex_ID);
    //need room for vertices, indices, and normals

    /*glBufferData(GL_ARRAY_BUFFER,    //Target
                 (sizeof(float) * 3 * vertex_count) + (sizeof(float) * 3 * face_count), //Size of the buffer
                 0,                  //Data
                 GL_STATIC_DRAW);    //Usage*/

    glBufferData(GL_ARRAY_BUFFER,    //Target
                 4 * (sizeof(float) * 3 * 3 * face_count) + (sizeof(float) * 3 * 2 * face_count),//size of buffer
                 0,                  //Data
                 GL_STATIC_DRAW);    //Usage

    //Store vertices
    glBufferSubData(GL_ARRAY_BUFFER, //Target
                    0,               //Offset
                   (sizeof(float) * 3 * 3 * face_count),  //Size of Data
                    vertices_3f_a);    //The data

    //Store normals
    glBufferSubData(GL_ARRAY_BUFFER, //Target
                   (sizeof(float) * 3 * 3 * face_count),                   //Offset
                   (sizeof(float) * 3 * 3 * face_count),  //Size of Data
                    normals_3f_a);           //The data

    //Store tangents
    glBufferSubData(GL_ARRAY_BUFFER,                         //Target
                   2 * (sizeof(float) * 3 * 3 * face_count), //Offset
                   (sizeof(float) * 3 * 3 * face_count),     //Size of Data
                    tangents_3f_a);                           //The data

    //Store binormals
    glBufferSubData(GL_ARRAY_BUFFER,                         //Target
                   3 * (sizeof(float) * 3 * 3 * face_count), //Offset
                   (sizeof(float) * 3 * 3 * face_count),     //Size of Data
                    binormals_3f_a);                           //The data

    //Store texture coordinates
    glBufferSubData(GL_ARRAY_BUFFER, //Target
                    4 * (sizeof(float) * 3 * 3 * face_count),                   //Offset
                   (sizeof(float) * 3 * 2 * face_count),  //Size of Data
                    tex_coords_2f_a);           //The data

    //Now do face normal lines;
    glGenBuffers(1, &face_normals_ID);
    glBindBuffer(GL_ARRAY_BUFFER, face_normals_ID);

    glBufferData(GL_ARRAY_BUFFER,    //Target
                 (sizeof(float) * 3 * 2 * face_count), //Size of the buffer
                 face_normal_lines_3f,                  //Data
                 GL_STATIC_DRAW);    //Usage

    //Now do vertex norms
    glGenBuffers(1, &vertex_normals_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_normals_ID);

    glBufferData(GL_ARRAY_BUFFER,    //Target
                 (sizeof(float) * 3 * 2 * vertex_count), //Size of the buffer
                 vertex_normal_lines_3f,                  //Data
                 GL_STATIC_DRAW);    //Usage

    uses_vbo = true;
  }

	//DebugMesh();
}

Mesh::~Mesh()
{
	delete[] vertex_normals;
	delete[] face_normals_3f;
	delete[] vertices;
  delete[] faces;
	delete[] indices;
	delete[] vertices_3f;
  delete[] face_normal_lines_3f;
  delete[] vertex_normal_lines_3f;

  //delete vbo info
  glDeleteBuffers(1, &vertex_ID);
  glDeleteBuffers(1, &vertex_normals_ID);
  glDeleteBuffers(1, &face_normals_ID);
}

void Mesh::DebugMesh()
{
  //Debug Print
  for(unsigned i = 0; i < vertex_count; i++)
  {
    std::cout << i << ": " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
  }
  //for(unsigned i = 0; i < face_count; i++)
  //{
    //std::cout << i << ": " << faces[i].p1 << " " << faces[i].p2 << " " << faces[i].p3 << std::endl;
  //}

	for(unsigned i = 0; i < face_count * 3; i++)
  {
		std::cout << indices[i] << std::endl;
  }
}

Vector4 Mesh::CalculateFaceNormal(Vector4 p1, Vector4 p2, Vector4 p3)
{
	Vector4 a = p2 - p1;
	Vector4 b = p3 - p1;
	Vector4 cp = a.Cross(b);
	cp.Normalize();
	return cp;
}

Vector4 Mesh::CalculateCenter(Vector4 p1, Vector4 p2, Vector4 p3)
{
	Vector4 center((p1.x + p2.x + p3.x)/3, (p1.y + p2.y + p3.y)/3, (p1.z + p2.z + p3.z)/3);
	return center;
}

void Mesh::CalculateVertexNormals(std::vector<std::vector<unsigned>> &face_membership)
{
	for(unsigned i = 0; i < face_membership.size(); i++)
	{
		Vector4 total;
		for(unsigned j = 0; j < face_membership[i].size(); j++)
		{
		  total += faces[face_membership[i][j]].face_normal;	
		}
		total = total/((float)face_membership[i].size());
		total.Normalize();
		vertex_normals[i] = total;
	}
}

void Mesh::CheckExtents(float x, float y, float z)
{
	//Check x extents
  if(x < x_extents.min)
		x_extents.min = x;
	if(x > x_extents.max)
		x_extents.max = x;
	//Check y extents
	if(y < y_extents.min)
		y_extents.min = y;
	if(y > y_extents.max)
		y_extents.max = y;
	//Check z extents
	if(z < z_extents.min)
		z_extents.min = z;
	if(z > z_extents.max)
		z_extents.max = z;
}

void Mesh::SetCenter()
{
	center.x = (x_extents.max + x_extents.min)/2;
	center.y = (y_extents.max + y_extents.min)/2;
	center.z = (z_extents.max + z_extents.min)/2;
}

void Mesh::CalculateTangentsAndBinormals()
{
  for(unsigned a = 0,a_tex = 0; a < (face_count * 3 * 3); a+=3, a_tex+=2)
  {
    unsigned b = (a + 3)%9 + (a/9)*9;
    unsigned b_tex = (a_tex + 2)%6 + (a_tex/6)*6;
    unsigned c = (a + 6)%9 + (a/9)*9; 
    unsigned c_tex = (a_tex + 4)%6 + (a_tex/6)*6;
    Vector4 A(vertices_3f_a[a], vertices_3f_a[a + 1], vertices_3f_a[a + 2], 0.0f);
    Vector4 B(vertices_3f_a[b], vertices_3f_a[b + 1], vertices_3f_a[b + 2], 0.0f); 
    Vector4 C(vertices_3f_a[c], vertices_3f_a[c + 1], vertices_3f_a[c + 2], 0.0f); 
    Vector4 A_Tex(tex_coords_2f_a[a_tex], tex_coords_2f_a[a_tex + 1], 0.0f, 0.0f);
    Vector4 B_Tex(tex_coords_2f_a[b_tex], tex_coords_2f_a[b_tex + 1], 0.0f, 0.0f);
    Vector4 C_Tex(tex_coords_2f_a[c_tex], tex_coords_2f_a[c_tex + 1], 0.0f, 0.0f);
    
    Vector4 P(B - A);
    Vector4 Q(C - A);
    Vector4 ST1(B_Tex - A_Tex);
    Vector4 ST2(C_Tex - A_Tex);

	//Gauss Jordan solve x, y z
	//[ST1.x ST1.y P.x]
	//[ST2.x ST2.y Q.x] 
	Vector4 tangent;
	Vector4 binormal;
	float factor = (1.0f/((ST1.x*ST2.y) - (ST2.x*ST1.y)));
	tangent.x  = (ST2.y * P.x + (-ST1.y) * Q.x)*factor;
	binormal.x = ((-ST2.x) * P.x + (ST1.x) * Q.x)*factor;
	tangent.y  = (ST2.y * P.y + (-ST1.y) * Q.y)*factor;
	binormal.y = ((-ST2.x) * P.y + (ST1.x) * Q.y)*factor;
	tangent.z  = (ST2.y * P.z + (-ST1.y) * Q.z)*factor;
	binormal.z = ((-ST2.x) * P.z + (ST1.x) * Q.z)*factor;

	Vector4 normal_check = tangent.Cross(binormal);

  tangents_3f_a[a + 0] = tangent.x;
  tangents_3f_a[a + 1] = tangent.y;
  tangents_3f_a[a + 2] = tangent.z;
  binormals_3f_a[a + 0] = binormal.x;
  binormals_3f_a[a + 1] = binormal.y;
  binormals_3f_a[a + 2] = binormal.z;
  //tangents_3f_a[a + 0] = binormal.x;
  //tangents_3f_a[a + 1] = binormal.y;
  //tangents_3f_a[a + 2] = binormal.z;
  //binormals_3f_a[a + 0] = tangent.x;
  //binormals_3f_a[a + 1] = tangent.y;
  //binormals_3f_a[a + 2] = tangent.z;

	if(P != ((tangent)*ST1.x + (binormal)*ST1.y))
	  std::cout << "Error calculating tangents and binormals" << std::endl;
	if(Q != ((tangent)*ST2.x + (binormal)*ST2.y))
		std::cout << "Error calculating tangents and binormals" << std::endl;
  }
}
