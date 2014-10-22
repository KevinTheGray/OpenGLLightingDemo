#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
#ifdef _DEBUG
  project_mode_string = "..\\";
#else
  project_mode_string = ".\\";
#endif
}

void ShaderManager::LoadShaderFiles(std::vector<std::string> &program_names)
{
  std::string directory;

  //Load and compile the shaders
  directory = project_mode_string + "vertex_shaders.txt";
  compileShaders(GL_VERTEX_SHADER, directory);
  directory = project_mode_string + "fragment_shaders.txt";
  compileShaders(GL_FRAGMENT_SHADER, directory);
  //Link the programs
  createPrograms();

  //Get shader names to send back
  SPIterator it = shader_programs.begin();
  for(; it!= shader_programs.end(); it++)
  {
    program_names.push_back((*it).first);
  }
}

int ShaderManager::compileShaders(GLuint type, std::string &directory)
{
  //Open the file
  std::ifstream file;
  file.open(directory.c_str(), std::ios::in);
  if(!file)
  {
    std::cout << directory + " could not be opened, no shaders loaded!" << std::endl;
    return -1;
  }

  std::string filename;
  GLuint shaderObject;
  GLchar *source;
  int len;
  //Opened, now read in the type and file name
  while(file.good())
  {
    //Load files into memory
    file >> filename;
    directory = project_mode_string + "Shaders\\" + filename;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << filename << ":" << std::endl;
    int err = loadFile(type, directory.c_str(), &source, &len);
    if(err)
      continue;

    //create shader objects
    shaderObject = glCreateShader(type); //Could be a vertex or fragment shader, it uses the TYPE
    //CONST STUFF so annoying
    const char *csource = source;
    const GLchar **p_source = &csource;
    glShaderSource(shaderObject, 1, p_source, &len);

    //Compile shader file
    glCompileShader(shaderObject);
    //Check if it compiled correctly
    err = checkErrorStatus(shaderObject, GL_COMPILE_STATUS);
    if(err)
      continue;
    std::cout << "Shader of type " << type << " compiled correctly: " << filename << std::endl;

    //Add it to the vector
    if(type == GL_VERTEX_SHADER)
      compiled_vertex_shaders.push_back(shaderObject);
    else
      compiled_fragment_shaders.push_back(shaderObject);
  }
  std::cout << "--------------------------------------------" << std::endl;
  file.close();
  return 0;
}

void ShaderManager::RemoveShaders(std::vector<std::string> &program_names)
{
  SPIterator it;
  for(it = shader_programs.begin(); it != shader_programs.end(); it++)
  {
    glDeleteProgram((*it).second);
    program_names.push_back((*it).first);
  }
  shader_programs.clear();
}

int ShaderManager::checkErrorStatus(GLuint shaderObject, GLuint type)
{
  GLint correct;

  glGetShaderiv(shaderObject, type, &correct);
  if (correct)
  {
    return 0;
  }    
  else
  {
    getShaderLog(shaderObject);
    return -1;
  }
}

int ShaderManager::createPrograms()
{
  std::string directory = project_mode_string + "programs.txt";
  std::ifstream file;
  file.open(directory.c_str(), std::ios::in);
  if(!file)
  {
    std::cout << directory + " could not be opened, no shaders created!" << std::endl;
    return -1;
  }

  std::string prog_name;
  int v_shader, f_shader;
  while(file.good()  && !file.eof())
  {
    file >> prog_name;
    file >> v_shader;
    file >> f_shader;

    if(v_shader >= (int)(compiled_vertex_shaders.size()) || f_shader >= ((int)compiled_fragment_shaders.size()))
    {
      std::cout << "Could not link a program with values " << v_shader << ", " << f_shader << std::endl;
      continue;
    }
    GLuint program = glCreateProgram();
    glAttachShader(program, compiled_vertex_shaders[v_shader]);
    glAttachShader(program, compiled_fragment_shaders[f_shader]);
    glLinkProgram(program);
    
    int err = checkErrorStatus(program, GL_LINK_STATUS);
    if(err)
      continue;
    std::cout << prog_name << " - " << v_shader << ", " << f_shader << " linked correctly" << std::endl;
    shader_programs[prog_name] = program;
  }
  for(unsigned i = 0; i < compiled_vertex_shaders.size(); i++)
  {
    glDeleteShader(compiled_vertex_shaders[i]);
  }
  for(unsigned i = 0; i < compiled_fragment_shaders.size(); i++)
  {
    glDeleteShader(compiled_fragment_shaders[i]);
  }
  compiled_vertex_shaders.clear();
  compiled_fragment_shaders.clear();
  return 0;
}

void ShaderManager::getShaderLog(GLuint shaderObject)
{
  GLint log_len;
  GLsizei s_len = 0;
  //Get the info log length
  glGetShaderiv(shaderObject, GL_SHADER_SOURCE_LENGTH, &log_len);
  //Allocate space for a log, and get the log.
  char *log = new char[log_len];
  glGetInfoLogARB(shaderObject, log_len, &s_len, log);
  std::cout << "Compiler Error: \n" << log << std::endl;
  free(log);
}


ShaderManager::~ShaderManager()
{
 
}


int ShaderManager::loadFile(int type, const char *filename, char** source, int *len)
{
	std::ifstream file;
	//char* source;
	//unsigned len;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file)
	{
		std::cout << "Shader of type " << type << " could not be opened: " << filename << std::endl;
		return -1;
	}

	*len = getFileLength(file);

	if (*len == 0)
	{
		// Error: Empty File 
		std::cout << "Shader of type " << type << " is empty: " << filename << std::endl;
		return -2;
	}

	(*source) = new char[*len+1];
	if ((*source) == 0)
	{
		std::cout << "New failed." << std::endl;
		return -3;   // can't reserve memory
	}

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
	(*source)[*len] = 0; 

	unsigned int i=0;
	while (file.good())
	{
		(*source)[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	(*source)[i] = 0;  // 0-terminate it at the correct position

	file.close();

	std::cout << "Shader of type " << type << " opened and read successfully: " << filename << std::endl;

	//std::pair<char*, unsigned long> ShaderAttributes(*source, *len);
	//Shaders[type][filename] = ShaderAttributes;
	return 0; // No Error
}

unsigned ShaderManager::getFileLength(std::ifstream &file)
{
	if(!file.good()) return 0;

	unsigned long pos=file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

void ShaderManager::StartProgram(std::string name)
{
  SPIterator it = shader_programs.find(name);
  if(it == shader_programs.end())
  {
    std::cout << "No such program name: " << name << std::endl;
    glUseProgram(0);
    return;
  }
  glUseProgram(it->second);
}

void ShaderManager::StartProgram(GLuint program_id)
{
  glUseProgram(program_id);
}

void ShaderManager::EndProgram()
{
  glUseProgram(0);
}

GLuint ShaderManager::GetProgram(std::string name)
{
  SPIterator it = shader_programs.find(name);
  if(it == shader_programs.end())
  {
    //std::cout << "No such program name: " << name << std::endl;
    return 0;
  }
  return (it->second);
}

GLint ShaderManager::GetVariableLocation(GLint program, const char *variable_name)
{
  GLint loc = glGetUniformLocation(program, variable_name);
	if(loc == -1)
		std::cout << "Can not locate variable named " << variable_name << "..." << std::endl;
	return loc;
}

GLint ShaderManager::GetAttribLocation(GLint program, const char *attrib_name)
{
  GLint loc = glGetAttribLocation(program, attrib_name);
	if(loc == -1)
		std::cout << "Can not locate attribute named " << attrib_name << "..." << std::endl;
	return loc;
}

void ShaderManager::EditVariable1i(const char *name, GLint program, int value)
{
  GLint loc = GetVariableLocation(program, name);
}
void ShaderManager::EditVariable1f(const char *name, GLint program, float value)
{
  GLint loc = GetVariableLocation(program, name);
}
void ShaderManager::EditVariable4fv(const char *name, GLint program, float *value)
{
  GLint loc = GetVariableLocation(program, name);
}