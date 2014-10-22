/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Renderer.h
Purpose: Declaration for the Renderer class, which is responsible for rendering.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/

#pragma once
#include "Light.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "Matrix4.h"
#include "Object.h"
#include "tgaimage.h"
#include "NormalMap.h"

#define DELTA_SIN .01f;

//#define NUM_SHADER_PROGRAMS
//static const char *shader_names[NUM_SHADER_PROGRAMS] = {"test", "blinn"};
static const char *shader_name = "test";
//static const char *shader_name = "blinn";

void CBFunction(int ID);

class Renderer
{
public:
  //  pointer to a GLUI window
  GLUI *glui_window;

  ~Renderer();

  //Public members
  void InitializeGluiWindow(int main_window_id);
  void RenderScene();
  void FloatToString(std::stringstream & ss, std::string &temp, float value);
  void LoadInitialFiles();
  void LoadNewMesh();
  void MouseClicked(int x, int y);
  void MouseReleased(int x, int y);
  void MouseMoved(int x, int y);
  //Member functions used by CBFunction
  Mesh* load_obj_file(const char *file_name);
  void modify_central_object(Mesh * mesh);
  void recompile_shaders();
  void light_choice_change();
  void cd_change();
  void scale_change();

  //members

  ////GLUI Stuff/////
  //Background Color
    float bg_red_value;
    float bg_blue_value;
    float bg_green_value;

  //File Loader
    GLUI_EditText   *file_directory;

  //Camera 
    int cam_mode_value;
    enum cam_modes
    {
      MANUAL,
      RC,
			CIRCULAR
    };
    float cam_distance_value;

   //Shaders
    GLUI_Listbox *shaders_lb;

  enum cbids
  {
    LOAD_FILE_BUTON,
    RECOMPILE_BUTTON,
    LIGHT_CHANGE,
    CD_CHANGE,
    CAM_MODE_CHANGE,
    NM_SCALE_CHANGE
  };
  ////GLUI Stuff//////

  static Renderer* GetSingleton();

  float cam_rot_angle;
  float light_rot_angle;
  //Global light values
  Vector4 global_ambient;
  Vector4 fog_color;
  Vector4 att_coef;
  Vector4 eye_position;
	Vector4 coc_eye_space;
  float inner_angle;
  float outer_angle;
  float spot_falloff;
  float fog_near;
  float fog_far;

	GLint global_ambient_loc;
  GLint fog_color_loc;
  GLint att_coef_loc;
  GLint inner_angle_loc;
  GLint outer_angle_loc;
  GLint spot_falloff_loc;
  GLint fog_near_loc;
  GLint fog_far_loc;
	GLint cam_matrix_loc;
  GLint eye_position_loc;
  GLint use_textures_loc;
  GLint use_normal_map_loc;
  GLint use_spec_map_loc;
  GLint use_tan_color_loc;
  GLint use_bin_color_loc;
  GLint use_ndotl_loc;
  GLint use_normal_mapping_loc;
	GLint view_matrix_loc;
  GLint tex_diff_loc;
  GLint tex_spec_loc;
  GLint normal_map_loc;
  GLint tangent_loc;
  GLint binormal_loc;
  
  Vector4 cam_eye;
  Vector4 cam_center;
  Vector4 mouse_click_origin;
  Vector4 cam_eye_origin;
	float z_distance;
	float delta_sin;
	Vector4 camera_center;
	Vector4 camera_up;
  //singleton stuff cause I need a callback for glui
  bool draw_object;
private:
  static bool instanceFlag;
  static Renderer *instance;

  //Private member functions
  void initialize_variables();
  void draw_axes();
  void render_object(Object *object);
  void compute_eye_pos();
  void edit_shader_lb(std::vector<std::string> &program_names, bool add);
  void set_uniforms();
  void get_uniform_locs();
  void setup_lights();
  void set_modified_lights();
  void load_textures();
  void load_normal_map();
  void use_textures();

  int vertex_norm_value;
  int face_norm_value;
  int draw_axes_value;
  int wireframe_value;
  int shader_value;
  int use_textures_value;
  int use_normal_map_value;
  int use_spec_map_value;
  int use_tan_color_value;
  int use_bin_color_value;
  int use_ndotl_value;
  int use_normal_mapping_value;
  void render_lights();
  float radius_value;
  float light_height_value;
  float light_rot_value;
  float cam_height_value;
  float cam_rot_value;
  int light_choice_value;
  int light_type_value;
  Vector4 light_amb_value;
  Vector4 light_diff_value;
  Vector4 light_spec_value;
  GLuint metal_roof_diff;
  GLuint metal_roof_spec;
  GLuint normal_map_texture;
  NormalMap *normal_map;

  float normal_map_scale_value;

  bool look_forward;
  bool look_center;
  bool has_looked_forward;
  bool has_looked_center;

  typedef std::map<std::string, Mesh*> MeshMap;
  typedef std::map<std::string, Mesh*>::iterator MMIt;
  Mesh *mesh;


  //Private members
  MeshMap meshes;
  std::vector<Light*> lights;
  Object central_object;
  Object co_plane;
  //std::map<std::string, *Mesh> meshes;  //List of all the meshes

  //Declaring the default constructor as private;
  Renderer()
  {
  }
  ShaderManager shader_manager;
public:
};