#pragma once

#include "Utilities.h"
#define VERTEX_SHADER 1
#define FRAGMENT_SHADER 0

//

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
  
  typedef std::map<std::string, GLuint> ShaderPrograms;
  typedef std::map<std::string, GLuint>::iterator SPIterator;
  ShaderPrograms shader_programs;
  std::vector<GLuint> compiled_vertex_shaders;
  std::vector<GLuint> compiled_fragment_shaders;

  
  void LoadShaderFiles(std::vector<std::string> &program_names);
  void RemoveShaders(std::vector<std::string> &program_names);
  void StartProgram(std::string name);
  void StartProgram(GLuint program_id);
  void EndProgram();
  GLuint GetProgram(std::string name);
	GLint GetVariableLocation(GLint program, const char  *variable_name);
  GLint GetAttribLocation(GLint porgram, const char *attrib_name);
  void EditVariable1i(const char *name, GLint program, int value);
  void EditVariable4fv(const char *name, GLint program, float *value);
  void EditVariable1f(const char *name, GLint program, float value);


private:
  std::string project_mode_string;

	unsigned getFileLength(std::ifstream& file);
  int loadFile(int type, const char* filename, char** source, int *len);
  int checkErrorStatus(GLuint shaderObject, GLuint type);
  void getShaderLog(GLuint shaderObject);
  int compileShaders(GLuint type, std::string &directory);
  int createPrograms();
};