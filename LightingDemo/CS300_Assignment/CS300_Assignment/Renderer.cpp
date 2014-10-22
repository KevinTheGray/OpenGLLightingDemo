/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Renderer.cpp
Purpose: Implementation for the Renderer class, which is responsible for rendering.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#include "Renderer.h"

Renderer* Renderer::instance = NULL;

Renderer* Renderer::GetSingleton()
{
  if(instance == NULL)
  {
    instance = new Renderer();
    instance->initialize_variables();
	//Load default OBJ.
  }
  return instance;
}

Renderer::~Renderer()
{
  if(instance)
  {
    delete instance;
    //Delete the meshes
  }
}

void Renderer::initialize_variables()
{
  instance->cam_rot_angle = 0.0f;
  instance->light_rot_angle = 0.0f;
  draw_axes_value = 1;
  wireframe_value = 0;
  vertex_norm_value = 0;
  face_norm_value = 0;
  use_textures_value = 1;
  use_spec_map_value = 0;
  use_normal_map_value = 0;
  use_tan_color_value = 0;
  use_bin_color_value = 0;
  use_ndotl_value = 0;
  use_normal_mapping_value = 1;

  bg_red_value = 0.5f;
  bg_green_value = 0.5f;
  bg_blue_value = 0.5f;

  cam_mode_value = 0;
  cam_distance_value = 10.0f;

  cam_eye = Vector4(0, 0, cam_distance_value, 0);
  cam_center = Vector4(0,0,0,0);
  draw_object = true;

  light_height_value = 3.0f;
  light_rot_value = 0.5;
  light_rot_angle = 0.0f;
  radius_value = 3.0f;

  //Initial Global Light values
  global_ambient = Vector4(.2,.2,.2,1);
  inner_angle = 15; //degrees
  outer_angle = 30; //degrees
  spot_falloff = 1;
  att_coef = Vector4(1, .1, 0, 0);
  fog_color = Vector4(1, 1, 1, 1);
  fog_near = .5 * Z_FAR;
  fog_far = Z_FAR;

  light_choice_value = 0;
  light_type_value = 0;
  cam_height_value = 3.0f;
  cam_rot_value = 1.0f;
  cam_rot_angle = 0.0f;
  delta_sin = 0.0f;

  normal_map_scale_value = 0.01f;

  look_forward = true;
  look_center = false;
  has_looked_forward = false;
  has_looked_center = false;
}

void Renderer::load_textures()
{
  std::string proj_string;
  std::string directory;
#ifdef _DEBUG
  proj_string = "..\\";
#else
  proj_string = ".\\";
#endif
  Image *metal_roof_d_image;
  Image *metal_roof_s_image;
  directory = proj_string;
  directory += "TGAS\\metal_roof_diff.tga";
  metal_roof_d_image = Image::Load(directory.c_str());
  directory = proj_string;
  directory += "TGAS\\metal_roof_spec.tga";
  metal_roof_s_image = Image::Load(directory.c_str());

  //Generate
  glGenTextures(1, &metal_roof_diff);
  glGenTextures(1, &metal_roof_spec);
  //Bind diffuse
  glBindTexture(GL_TEXTURE_2D, metal_roof_diff);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               metal_roof_d_image->SizeX(),
               metal_roof_d_image->SizeY(),
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               metal_roof_d_image->Data());
  //Bind specular
  glBindTexture(GL_TEXTURE_2D, metal_roof_spec);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               metal_roof_s_image->SizeX(),
               metal_roof_s_image->SizeY(),
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               metal_roof_s_image->Data());  

  Image::Free(metal_roof_s_image);
  Image::Free(metal_roof_d_image);
}


void Renderer::load_normal_map()
{
  std::string proj_string;
  std::string directory;
#ifdef _DEBUG
  proj_string = "..\\";
#else
  proj_string = ".\\";
#endif

  Image *height_map_image;
  directory = proj_string;
  //directory += "TGAS\\heightmap2.tga";
  //directory += "TGAS\\heightmap.tga";
  directory += "TGAS\\metal_roof_spec.tga";
  
  height_map_image = Image::Load(directory.c_str());

  //unsigned height_map_size = height_map_image->SizeX() * height_map_image->SizeX() * 3;
  normal_map = NormalMap::Generate(height_map_image, normal_map_scale_value);
  //Generate normal map
  /*for(unsigned i = 0; i < normal_map->SizeX(); i++)
  {
    for(unsigned j = 0; j < (normal_map->SizeY() * 3); j+=3)
    {
      //Access texel (i,j/3)
      unsigned r = (i * normal_map->SizeX() * 3) + j + 0;
      unsigned g = (i * normal_map->SizeX() * 3) + j + 1;
      unsigned b = (i * normal_map->SizeX() * 3) + j + 2;
      //std::cout << (int)normal_map->NormalData()[r] << std::endl;
      //std::cout << (int)normal_map->NormalData()[g] << std::endl;
      //std::cout << (int)normal_map->NormalData()[b] << std::endl;
      //std::cout << "---" << std::endl;
      //r == red_value of (i,j/3) texel, its grayscale, so g and b are the same
      //if(height_map_image->Data()[r] != height_map_image->Data()[g] || height_map_image->Data()[r] != height_map_image->Data()[b])
        //std::cout << "ERROR READING IN HEIGHT MAP" << std::endl;
    }
  }*/
  glGenTextures(1, &normal_map_texture);
  glBindTexture(GL_TEXTURE_2D, normal_map_texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               normal_map->SizeX(),
               normal_map->SizeY(),
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               normal_map->NormalData());
               //normal_map->ImageData());
               //height_map_image->Data());
  Image::Free(height_map_image);


}
void Renderer::use_textures()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, metal_roof_diff);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, metal_roof_spec);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  //Set uniforms
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, normal_map_texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

  glUniform1i(tex_diff_loc ,0);
  glUniform1i(tex_spec_loc,1);
  glUniform1i(normal_map_loc,2);

}

void Renderer::InitializeGluiWindow(int main_window_id)
{
  float version = GLUI_Master.get_version();
  glui_window = GLUI_Master.create_glui("CS300 Interactive GUI");

  //GLUI Background Color
  GLUI_Rollout *background_color_rollout = glui_window->add_rollout("Background Color [0,1]");
  GLUI_EditText *temp_et = glui_window->add_edittext_to_panel(background_color_rollout, "Red", GLUI_EDITTEXT_FLOAT, &bg_red_value);
  temp_et->set_float_limits(0.0f, 1.0f);
  temp_et = glui_window->add_edittext_to_panel(background_color_rollout, "Green", GLUI_EDITTEXT_FLOAT, &bg_green_value);
  temp_et->set_float_limits(0.0f, 1.0f);
  temp_et = glui_window->add_edittext_to_panel(background_color_rollout, "Blue", GLUI_EDITTEXT_FLOAT, &bg_blue_value);
  temp_et->set_float_limits(0.0f, 1.0f);
  //GLUI File Loader
  glui_window->add_separator();
  GLUI_Rollout *file_loader_rollout = glui_window->add_rollout("File Loader");
  file_directory = glui_window->add_edittext_to_panel(file_loader_rollout, "Directory:",GLUI_EDITTEXT_TEXT, 0, LOAD_FILE_BUTON, CBFunction);
  file_directory->set_text("..\\OBJS\\");
  //glui_window->add_button_to_panel(file_loader_rollout, "Load Mesh", LOAD_FILE_BUTON, CBFunction);

  //GLUI Camera options
  glui_window->add_separator();
  GLUI_Rollout *camera_options_rollout = glui_window->add_rollout("Camera Options");
  temp_et = glui_window->add_edittext_to_panel(camera_options_rollout, "Distance", GLUI_EDITTEXT_FLOAT, &cam_distance_value, CD_CHANGE, CBFunction);
  temp_et->set_float_limits(0, 10000);
  GLUI_Listbox *temp_lb = glui_window->add_listbox_to_panel(camera_options_rollout, "Mode", &cam_mode_value,CAM_MODE_CHANGE, CBFunction);
  temp_lb->add_item(0, "Manual");
  temp_lb->add_item(1, "Roller Coaster");
	temp_lb->add_item(2, "Circular");
  temp_et = glui_window->add_edittext_to_panel(camera_options_rollout, "Cam Height", GLUI_EDITTEXT_FLOAT, &cam_height_value);
  temp_et = glui_window->add_edittext_to_panel(camera_options_rollout, "Cam Rotation", GLUI_EDITTEXT_FLOAT, &cam_rot_value);

  //GLUI Shader options
  glui_window->add_separator();
  GLUI_Rollout *shader_options_rollout = glui_window->add_rollout("Shader Options");
  shaders_lb = glui_window->add_listbox_to_panel(shader_options_rollout, "Program", &shader_value);
  
  //GLUI Normal Map OPtions
  glui_window->add_separator();
  GLUI_Rollout *temp_rollout1 = glui_window->add_rollout("Normal Map Options");
  glui_window->add_checkbox_to_panel(temp_rollout1, "On", &use_normal_mapping_value);
  glui_window->add_edittext_to_panel(temp_rollout1, "Scale", GLUI_EDITTEXT_FLOAT, &normal_map_scale_value, NM_SCALE_CHANGE, CBFunction);
  //GLUI debug options
  glui_window->add_separator();
  GLUI_Rollout *temp_rollout = glui_window->add_rollout("Debug Options");
  glui_window->add_checkbox_to_panel(temp_rollout, "Draw Axes", &draw_axes_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Wireframe", &wireframe_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Face Normals", &face_norm_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Vertex Normals", &vertex_norm_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Textures", &use_textures_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Normal Map", &use_normal_map_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Specular Map", &use_spec_map_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Tangent", &use_tan_color_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "Binormal", &use_bin_color_value);
  glui_window->add_checkbox_to_panel(temp_rollout, "NdotL", &use_ndotl_value);  

  //GLUI global light values
  glui_window->add_column();
  temp_rollout = glui_window->add_rollout("Global Light Values", false);
  //Global Ambient
  GLUI_Panel *temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Global Ambient");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "R", GLUI_EDITTEXT_FLOAT, &global_ambient.r);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "G", GLUI_EDITTEXT_FLOAT, &global_ambient.g);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "B", GLUI_EDITTEXT_FLOAT, &global_ambient.b);
	temp_et->set_float_limits(0.0f, 1.0f);
  //Fog color
  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Fog Color");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "R", GLUI_EDITTEXT_FLOAT, &fog_color.r);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "G", GLUI_EDITTEXT_FLOAT, &fog_color.g);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "B", GLUI_EDITTEXT_FLOAT, &fog_color.b);
	temp_et->set_float_limits(0.0f, 1.0f);
  //Att Coef
  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Attenuation");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "c0", GLUI_EDITTEXT_FLOAT, &att_coef.r);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "c1", GLUI_EDITTEXT_FLOAT, &att_coef.g);
	temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "c2", GLUI_EDITTEXT_FLOAT, &att_coef.b);
	temp_et->set_float_limits(0.0f, 1.0f);
  //The rest
  glui_window->add_column_to_panel(temp_rollout,false);
  temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Spot Cutoff", GLUI_EDITTEXT_FLOAT, &spot_falloff);
	temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Inner Angle", GLUI_EDITTEXT_FLOAT, &inner_angle);
	temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Outer Angle", GLUI_EDITTEXT_FLOAT, &outer_angle);
  temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Light Rot.", GLUI_EDITTEXT_FLOAT, &light_rot_value);
	temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Light Hgt", GLUI_EDITTEXT_FLOAT, &light_height_value);
	temp_et = glui_window->add_edittext_to_panel(temp_rollout, "Radius", GLUI_EDITTEXT_FLOAT, &radius_value);

  Mesh *central_mesh = load_obj_file("..\\OBJS\\sphere.obj");
  modify_central_object(central_mesh);
  Mesh *plane = load_obj_file("..\\OBJS\\xz_plane.obj");
  co_plane.mesh = plane;
  co_plane.trans.translation = Vector4(0,-.8,0,1);
  co_plane.trans.scale = Vector4(10,10,10,1);

  std::vector<std::string> program_names;
  shader_manager.LoadShaderFiles(program_names);
  edit_shader_lb(program_names, true); 
  shader_value = shader_manager.GetProgram(program_names[0]);
  std::cout << shader_value << std::endl;

  //Setup Lights
  //Get Uniform Locations
  setup_lights();
  load_textures();
  load_normal_map();
  get_uniform_locs();

  //Setup lights
  temp_rollout = glui_window->add_rollout("Lights", false);
  temp_lb = glui_window->add_listbox_to_panel(temp_rollout, "Light", &light_choice_value, LIGHT_CHANGE, CBFunction);
  std::string name = "Light x";
  for(unsigned i = 0; i < 8; i++)
  {
    name[6] = '0' + i;
    temp_lb->add_item(i, name.c_str());
  }

  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Values");

  temp_lb = glui_window->add_listbox_to_panel(temp_panel, "Type", &light_type_value);
  //temp_lb->live
  temp_lb->add_item(0, "Point Light");
  temp_lb->add_item(1, "Directional");
  temp_lb->add_item(2, "Spot  Light");
  temp_lb->add_item(3, "Off");

  //Ambient
  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Ambient");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "R", GLUI_EDITTEXT_FLOAT, &light_amb_value.r);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "G", GLUI_EDITTEXT_FLOAT, &light_amb_value.g);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "B", GLUI_EDITTEXT_FLOAT, &light_amb_value.b);
  temp_et->set_float_limits(0.0f, 1.0f);
  //Diffuse
  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Diffuse");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "R", GLUI_EDITTEXT_FLOAT, &light_diff_value.r);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "G", GLUI_EDITTEXT_FLOAT, &light_diff_value.g);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "B", GLUI_EDITTEXT_FLOAT, &light_diff_value.b);
  temp_et->set_float_limits(0.0f, 1.0f);
  //Specular
  temp_panel = glui_window->add_panel_to_panel(temp_rollout, "Specular");
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "R", GLUI_EDITTEXT_FLOAT, &light_spec_value.r);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "G", GLUI_EDITTEXT_FLOAT, &light_spec_value.g);
  temp_et->set_float_limits(0.0f, 1.0f);
  glui_window->add_column_to_panel(temp_panel,false);
  temp_et = glui_window->add_edittext_to_panel(temp_panel, "B", GLUI_EDITTEXT_FLOAT, &light_spec_value.b);
  temp_et->set_float_limits(0.0f, 1.0f);
  
}

void Renderer::setup_lights()
{
  for(unsigned i = 0; i < 8; i++)
  {
    Light  *light = new Light(Light::OFF);
    light->setPosition(Vector4( 5, 5, 5, 0 ) );
    light->setAmbient(Vector4( 0.0, 0.0, 0.0, 0.0 ));
    light->setDiffuse(Vector4( 1.0, 0.0, 0.0, 0.0 ));
    light->setSpecular(Vector4( 1.0, 1.0, 0.0, 0.0));
    lights.push_back(light);
  }

  lights[0]->type = light_type_value = Light::GL_POSITIONAL;
  lights[0]->ambient = light_amb_value = Vector4(0, 0, 0, 0);
  lights[0]->diffuse = light_diff_value = Vector4(.8,.8,.8,1);
  lights[0]->specular = light_spec_value =Vector4(1, 1, 1, 1);
  //Set initial Global Values
}


void Renderer::edit_shader_lb(std::vector<std::string> &program_names, bool add)
{
  for(unsigned i = 0; i < program_names.size(); i++)
  {
    if(add)
     shaders_lb->add_item(shader_manager.GetProgram(program_names[i]), program_names[i].c_str());
    else
     shaders_lb->delete_item(program_names[i].c_str());
  }
  if(program_names.size())
    shader_value = shader_manager.GetProgram(program_names[0]);
}
void Renderer::LoadInitialFiles()
{
  //load_obj_file("..\\OBJS\\cubet.obj");
}

void Renderer::recompile_shaders()
{
  std::vector<std::string> program_names;
  shader_manager.RemoveShaders(program_names);
  edit_shader_lb(program_names, false);
  program_names.clear();
  shader_manager.LoadShaderFiles(program_names);
  edit_shader_lb(program_names, true);
}
 
void Renderer::LoadNewMesh()
{
  Mesh *central_mesh = load_obj_file(file_directory->get_text());
  if(central_mesh)
    modify_central_object(central_mesh);
}

void Renderer::RenderScene()
{
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(bg_red_value, bg_green_value, bg_blue_value, 1.0);

  glColor3f(1.0,1.0,1.0);
  if(wireframe_value != 0)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Compute the values for the camera eye
  glLoadIdentity();
  compute_eye_pos();
 
  gluLookAt(cam_eye.x, cam_eye.y, cam_eye.z, 
    cam_center.x, cam_center.y, cam_center.z,
    0, 1, 0);

  glLineWidth(.1); 
  if(draw_axes_value)
    draw_axes();


  //std::cout << lights[0]->type << std::endl;
  render_lights();
  light_rot_angle += light_rot_value;
  cam_rot_angle += cam_rot_value;
  delta_sin += DELTA_SIN;
  shader_manager.StartProgram(shader_value);
  set_modified_lights();
  set_uniforms();
  if(use_textures_value)
    use_textures();
  render_object(&central_object);
  render_object(&co_plane);
  shader_manager.EndProgram();
  //std::cout << light_choice_value << std::endl;
  glutSwapBuffers();
}

void Renderer::set_modified_lights()
{
  lights[light_choice_value]->type = light_type_value;
  lights[light_choice_value]->ambient = light_amb_value;
  lights[light_choice_value]->diffuse = light_diff_value;
  lights[light_choice_value]->specular = light_spec_value;
}

void Renderer::render_lights()
{
  //float diag_x = central_object.mesh->x_extents.max - central_object->x_extents.min;
  //float diag_z = central_object->z_extents.max - central_object->z_extents.min;
  //Vector4 xz_diag(diag_x, 0, diag_z, 0);
  float radius = radius_value;
  float angle = 0;
  for(unsigned i = 0; i < lights.size(); i++)
  {
    if(lights[i]->type == Light::OFF)
    {
      angle += 2*PI/lights.size();
      continue;
    }
    glPushMatrix();
    float x = 0 + radius * cos(angle);
    float z = 0 + radius * sin(angle);
		Vector4 sin_vector(x,0,z,1);
    glRotatef(light_rot_angle, 0.0f, 1.0f, 0.0f);
    //Find x_value of rotation first so we can get the y value
		GLfloat matrix[16]; 
		float y = light_height_value * sin(angle + delta_sin);
    //std::cout << y << std::endl;
		//

    //glTranslatef(x, central_object->y_extents.max - central_object->center.y, z);
    glTranslatef(x, 1 + y, z);
    glutSolidSphere(.25, 10, 10);
    glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
		Matrix4 my_mat(matrix);
		Vector4 light_position(0, 0, 0,1);
		light_position = my_mat * light_position;
		Vector4 zero(0,0,0,1);
		//Vector4 light_direction(0,0,0,0);
    glPopMatrix();

		//Get direction
		glPushMatrix();
    glScalef(central_object.trans.scale.x, central_object.trans.scale.y, central_object.trans.scale.z);
    glTranslatef(central_object.trans.translation.x, central_object.trans.translation.y, central_object.trans.translation.z);
    glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
		Matrix4 my_mat2(matrix);
		Vector4 mesh_center(0,0,0,1);
    mesh_center = my_mat2 * mesh_center;
		glPopMatrix();

    angle += 2*PI/lights.size();
		Vector4 light_direction = mesh_center - light_position;
		lights[i]->setPosition(light_position);
		lights[i]->setDirection(light_direction);
  }
}

void Renderer::set_uniforms()
{
  glUniform4fv(global_ambient_loc, 1, global_ambient.v);
	glUniform4fv(fog_color_loc, 1, fog_color.v);
  glUniform4fv(att_coef_loc, 1, att_coef.v);
  glUniform4fv(eye_position_loc, 1, eye_position.v);
	glUniform1f(inner_angle_loc, inner_angle);
	glUniform1f(outer_angle_loc, outer_angle);
	glUniform1f(spot_falloff_loc, spot_falloff);
	glUniform1f(fog_near_loc, fog_near);
	glUniform1f(fog_far_loc, fog_far);
  glUniform1i(use_textures_loc, use_textures_value);
  glUniform1i(use_normal_map_loc, use_normal_map_value);
  glUniform1i(use_spec_map_loc, use_spec_map_value);
  glUniform1i(use_tan_color_loc, use_tan_color_value);
  glUniform1i(use_bin_color_loc, use_bin_color_value);
  glUniform1i(use_ndotl_loc, use_ndotl_value);
  glUniform1i(use_normal_mapping_loc, use_normal_mapping_value);
  
  for(unsigned i = 0; i < lights.size(); i++)
  {
    glUniform1i(lights[i]->type_loc, lights[i]->type);
		glUniform4fv(lights[i]->ambient_loc, 1, lights[i]->ambient.v);
    glUniform4fv(lights[i]->diffuse_loc, 1, lights[i]->diffuse.v);
		glUniform4fv(lights[i]->specular_loc, 1, lights[i]->specular.v);
		glUniform4fv(lights[i]->position_loc, 1, lights[i]->position.v);
		glUniform4fv(lights[i]->direction_loc, 1, lights[i]->direction.v);
  }
}

void Renderer::get_uniform_locs()
{
  global_ambient_loc = shader_manager.GetVariableLocation(shader_value, "global_ambient");
	fog_color_loc = shader_manager.GetVariableLocation(shader_value, "fog_color");
	att_coef_loc = shader_manager.GetVariableLocation(shader_value, "att_coef");
  inner_angle_loc = shader_manager.GetVariableLocation(shader_value, "inner_angle");
  outer_angle_loc = shader_manager.GetVariableLocation(shader_value, "outer_angle");
  spot_falloff_loc = shader_manager.GetVariableLocation(shader_value, "spot_falloff");
  fog_near_loc = shader_manager.GetVariableLocation(shader_value, "fog_near");
  fog_far_loc = shader_manager.GetVariableLocation(shader_value, "fog_far");
	cam_matrix_loc = shader_manager.GetVariableLocation(shader_value, "cam_matrix");
  eye_position_loc = shader_manager.GetVariableLocation(shader_value, "eye_position");
  use_textures_loc = shader_manager.GetVariableLocation(shader_value, "use_textures");
  use_normal_map_loc = shader_manager.GetVariableLocation(shader_value, "use_normal_map");
  use_spec_map_loc = shader_manager.GetVariableLocation(shader_value, "use_spec_map");
  use_tan_color_loc = shader_manager.GetVariableLocation(shader_value, "use_tan_color");
  use_bin_color_loc = shader_manager.GetVariableLocation(shader_value, "use_bin_color");
  use_ndotl_loc = shader_manager.GetVariableLocation(shader_value, "use_ndotl");
  use_normal_mapping_loc = shader_manager.GetVariableLocation(shader_value, "use_normal_mapping");
	view_matrix_loc = shader_manager.GetVariableLocation(shader_value, "view_matrix");
  tex_diff_loc = shader_manager.GetVariableLocation(shader_value, "tex_diff");
  tex_spec_loc = shader_manager.GetVariableLocation(shader_value, "tex_spec");
  normal_map_loc = shader_manager.GetVariableLocation(shader_value, "normal_map");
  tangent_loc = shader_manager.GetAttribLocation(shader_value, "tangent");
  binormal_loc = shader_manager.GetAttribLocation(shader_value, "binormal");

  std::string name = "lights[x]";
  for(unsigned i = 0; i < 8; i++)
  {
		char num = '0' + i;
		name[7] = num;

    std::string u_name = name + ".type";
		lights[i]->type_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
		u_name = name + ".ambient";
    lights[i]->ambient_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
		u_name = name + ".diffuse";
    lights[i]->diffuse_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
		u_name = name + ".specular";
    lights[i]->specular_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
		u_name = name + ".position";
    lights[i]->position_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
		u_name = name + ".direction";
		lights[i]->direction_loc = shader_manager.GetVariableLocation(shader_value, u_name.c_str());
  }
}

void Renderer::MouseClicked(int x, int y)
{
  mouse_click_origin = Vector4(x, y, 0, 0);
  cam_eye_origin = cam_eye;
  //std::cout << "Down" << std::endl;
}

void Renderer::MouseReleased(int x, int y)
{
  //std::cout << "Up" << std::endl;
  //cam_eye = cam_eye_origin;
}

void Renderer::MouseMoved(int x, int y)
{
  //std::cout << x  << ", " << y << std::endl;
  float delta_x = (float)(x) - mouse_click_origin.x;
  float delta_y = (float)(y) - mouse_click_origin.y;
  //std::cout << "d_x: " << delta_x << std::endl;
  float rot_value_ay = -(2*PI * (delta_x)/cam_distance_value);
  float rot_value_ax = -(2*PI * (delta_y)/cam_distance_value);
  //std::cout << rot_value_ay << std::endl;
  float temp_mat[16];
  glPushMatrix();
  glLoadIdentity();
  //glRotatef(rot_value_ay, 0.0f, 1.0f, 0.0f);
  glRotatef(rot_value_ax, 1.0f, 0.0f, 0.0f);
  glRotatef(rot_value_ay, 0.0f, 1.0f, 0.0f);
  glGetFloatv(GL_MODELVIEW_MATRIX, temp_mat);
  Matrix4 my_mat(temp_mat);
  cam_eye = my_mat * cam_eye_origin;
  glPopMatrix();

}
void Renderer::compute_eye_pos()
{
  if(cam_mode_value == RC)
  {
    float y = cam_height_value*sin(delta_sin);
    float temp_mat[16];
    //std::cout << y << std::endl;
    cam_eye_origin = Vector4(0,0,0,1);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(cam_rot_angle, 0.0, 1.0, 0.0);
    glTranslatef(0, y, cam_distance_value);
    glGetFloatv(GL_MODELVIEW_MATRIX, temp_mat);
    Matrix4 my_mat(temp_mat);
    cam_eye = my_mat * cam_eye_origin;
    glPopMatrix();
    //Find cam_center
    if(look_center)
    {
      cam_center = Vector4(0,0,0,1);
    }
    else
    {
      cam_center = Vector4(0,0,0,1);
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(-cam_eye.x, -cam_eye.y, -cam_eye.z);
      float rot_val = cam_rot_angle;
      glRotatef(rot_val, 0.0, 1.0, 0.0);
      glTranslatef(cam_eye.x, cam_eye.y, cam_eye.z);
      float temp_mat[16];
      glGetFloatv(GL_MODELVIEW_MATRIX, temp_mat);
      Matrix4 my_mat(temp_mat);
      cam_center = my_mat * cam_center;
      glPopMatrix();
    }
  }
  else if(cam_mode_value == MANUAL)
  {
    cam_center = Vector4(0,0,0,1);
  }
	else
	{
    float y = cam_height_value*sin(delta_sin);
    float temp_mat[16];
    //std::cout << y << std::endl;
    cam_eye_origin = Vector4(0,0,0,1);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(cam_rot_angle, 0.0, 1.0, 0.0);
    glTranslatef(0, y, cam_distance_value);
    glGetFloatv(GL_MODELVIEW_MATRIX, temp_mat);
    Matrix4 my_mat(temp_mat);
    cam_eye = my_mat * cam_eye_origin;
    glPopMatrix();
		cam_center = Vector4(0,0,0,1);
	}
}

void Renderer::render_object(Object *object)
{
  glPushMatrix();
  //glScalef(mesh->trans.scale.x, mesh->trans.scale.y, mesh->trans.scale.z);
  glScalef(object->trans.scale.x, object->trans.scale.y, object->trans.scale.z);
  glTranslatef(object->trans.translation.x, object->trans.translation.y, object->trans.translation.z);

  // Bind vertex and element data.
  glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vertex_ID);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_ID);

  // enable vertex arrays
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableVertexAttribArray(tangent_loc);
  glEnableVertexAttribArray(binormal_loc);

  glVertexPointer(3, GL_FLOAT, 0, 0);
  glNormalPointer(GL_FLOAT, 0, (void*)(sizeof(float) * 3 * 3 * object->mesh->face_count));
  glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)(2 * sizeof(float) * 3 * 3 * object->mesh->face_count));
  glVertexAttribPointer(binormal_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * 3 * 3 * object->mesh->face_count));
  glTexCoordPointer(2, GL_FLOAT, 0, (void*)(4 * sizeof(float) * 3 * 3 * object->mesh->face_count));
  //glNormalPointer(GL_FLOAT, 0, 0);
  //glVertexPointer(3, GL_FLOAT, 0, 0);

  //glDrawElements(GL_TRIANGLES, mesh->face_count * 3, GL_UNSIGNED_INT, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3 * object->mesh->face_count);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  //Draw face normals
  if(face_norm_value != 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, object->mesh->face_normals_ID);

    glVertexPointer(3, GL_FLOAT, 0, 0);

    //Draw
    glLineWidth(1.5);
    glDrawArrays(GL_LINES, 0, object->mesh->face_count * 2);
  }

  //Draw vertex normals
  if(vertex_norm_value != 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vertex_normals_ID);

    glVertexPointer(3, GL_FLOAT, 0, 0);

    //Draw
    glLineWidth(1.5);
    glDrawArrays(GL_LINES, 0, object->mesh->vertex_count * 2);

    glDisableClientState(GL_VERTEX_ARRAY);
  } 
  
  glPopMatrix();
}

void Renderer::draw_axes()
{
  glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
  glVertex3f(-1000, 0, 0);
	glVertex3f(1000, 0, 0);
	glVertex3f(0, -1000, 0);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, 0, -1000);
	glVertex3f(0, 0, 1000);
	glEnd();
}
Mesh* Renderer::load_obj_file(const char *filename)
{
  //Findout of this file was already loaded
  MMIt it = meshes.find(filename);
  if(it != meshes.end())
  {
    //If it is, return, no need to load it again.
    std::cout << "Mesh already loaded in." << std::endl;
    return meshes[filename];
  }

  //Now load in the mesh
	Mesh *new_mesh;
  std::string message = filename;
  std::ifstream obj_file;
  //Try to open the file
  obj_file.open(filename);
  if(obj_file.is_open())
	{
		message = message + " opened.";
		std::cout << message << std::endl;

		//Read the file, parse it and stuff.
    obj_file.close();
    new_mesh = new Mesh(filename, true);
    meshes[filename] = new_mesh;
    return new_mesh;
	}
  else
  {
    message = message + " could not be opened.";
    std::cout << message << std::endl;
    return 0;
  }
}

void Renderer::modify_central_object(Mesh * mesh)
{
  central_object.mesh = mesh;
  central_object.trans.translation.x = -mesh->center.x;
  central_object.trans.translation.y = -mesh->center.y;
  central_object.trans.translation.z = -mesh->center.z;
	float values[3] = {mesh->x_extents.max, mesh->y_extents.max, mesh->z_extents.max};
	float scale_value = 1/get_min(values, 3);
	central_object.trans.scale = Vector4(scale_value, scale_value, scale_value, 1.0f);
}


void Renderer::FloatToString(std::stringstream &ss, std::string &temp, float value)
{
  ss << value;
  temp = ss.str();
  ss.str("");
}

void Renderer::light_choice_change()
{
  light_type_value = lights[light_choice_value]->type;
  light_amb_value = lights[light_choice_value]->ambient;
  light_diff_value = lights[light_choice_value]->diffuse;
  light_spec_value = lights[light_choice_value]->specular;
  glui_window->sync_live();
}

void Renderer::cd_change()
{
  cam_eye = Vector4(0,0, cam_distance_value);
  cam_center = Vector4(0,0,0,1);
}

void Renderer::scale_change()
{
  normal_map->Regenerate(normal_map, normal_map_scale_value);
  glBindTexture(GL_TEXTURE_2D, normal_map_texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               normal_map->SizeX(),
               normal_map->SizeY(),
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               normal_map->NormalData());
               //normal_map->ImageData());
               //height_map_image->Data());
}

void CBFunction(int ID)
{
  Renderer *renderer = Renderer::GetSingleton();
	const char *directory = renderer->file_directory->get_text();
  switch(ID)
  {
  case Renderer::LOAD_FILE_BUTON:
    //std::cout << "Load a file" << std::endl;
    //renderer->LoadAdditionalFiles();
    break;
  case Renderer::RECOMPILE_BUTTON:
    //renderer->recompile_shaders();
    break;
  case Renderer::LIGHT_CHANGE:
    renderer->light_choice_change();
    break;
  case Renderer::CD_CHANGE:
    renderer->cd_change();
    break;
  case Renderer::CAM_MODE_CHANGE:
    renderer->cd_change();
    break;
  case Renderer::NM_SCALE_CHANGE:
    //renderer->scale_change();
    break;
  }
}