#define POSITIONAL  0
#define DIRECTIONAL 1
#define SPOTLIGHT   2
#define OFF         3
#define PI          3.14159265358979323846264 

struct Light
{ 
  int type;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  vec4 direction;
};



//texture uniforms
uniform sampler2D tex_diff;
uniform sampler2D tex_spec;
uniform int use_textures;
//Vertex shader values

//Fragment normal
varying vec3 N;
//Fragment position in eye space
varying vec4 v;

//light values
uniform vec4 global_ambient;
uniform vec4 fog_color;
uniform vec4 att_coef;
uniform vec4 eye_position;
uniform float inner_angle;
uniform float outer_angle;
uniform float spot_falloff;
uniform float fog_near;
uniform float fog_far;
uniform mat4 cam_matrix;

//Lights 8 MAX
uniform struct Light lights[8];

void main (void)  
{
  vec4 I_emissive = vec4(0,0,0,0);
  vec4 k_ambient =  vec4(1,1,1,1);
  vec4 k_diffuse =  vec4(1,1,1,1);
  vec4 k_specular = vec4(1,1,1,1);
  vec4 debug_color = vec4(0,0,0,0);
  
  //eye vector
  vec3 V = -normalize(v.xyz);
  //Light vector
  vec3 L;  //calculated depending on type of light
  //Reflection vector
  vec3 R; //Calculted after L
  vec3 H;
  if(use_textures == 1)
  {
    k_ambient = texture2D(tex_diff, gl_TexCoord[0].st);
    k_diffuse = texture2D(tex_diff, gl_TexCoord[0].st);
    k_specular = texture2D(tex_spec, gl_TexCoord[0].st);
  }
  vec4 final_color = vec4(0,0,0,0);
  vec4 lightSum = vec4(0,0,0,0);
  //For now k_a, k_d, and k_s, and n_s are 1.0
  final_color = I_emissive + (global_ambient * k_ambient);
  for(int i = 0; i < 8; i++)
  {
    if(lights[i].type == OFF)
	  continue;
	//Position of light in the modelview space
	vec3 light_position = (lights[i].position).xyz;
	//vec3 light_position = vec3(0,0,0);
	//Direction of the light in modelview space
	vec3 light_direction = ((lights[i].direction)).xyz; 
	//Deafault Attenuation value
	float Att = 1.0;
	//Deafult spotlight value
	float spotEffect = 1.0;
	
	//Calculating Attenuation value for this light
	//Point Light or Spotligght source
	if(lights[i].type != DIRECTIONAL)
	{
	  debug_color = vec4(1,0,0,0);
	  L = normalize(light_position - v.xyz);
	  float dL = distance(light_position, v.xyz);
	  debug_color = vec4(dL,dL,dL,1);
	  debug_color = normalize(debug_color);
	  Att = min(1.0/(att_coef.x + att_coef.y * dL + att_coef.z * pow(dL,2.0)), 1.0);
	}
	//Directional light source
	else
	{
	  debug_color = vec4(0,1,0,0);
	  L = -normalize(light_direction);
	}
	H = normalize((light_position).xyz + (v).xyz)/(length((light_position).xyz + (v).xyz));
	
	if(lights[i].type == SPOTLIGHT)
	{
	  debug_color = vec4(0,0,1,0);
      vec3 sL = -normalize(light_direction);
	  vec3 sD = normalize(light_position - v.xyz);
      float cosPhi   = cos(outer_angle * (PI/180.0));
   	  float cosTheta = cos(inner_angle * (PI/180.0));
	  float LD = dot(sL, sD);
      if(LD < cosPhi)
	  {
	    spotEffect = 0.0;
	  }
	  else if(LD > cosTheta)
	  {
	    spotEffect = 1.0;
	  }
	  else
	  {
	    float cosAlpha = LD;// / (length(sL) * length(sD));
		spotEffect = pow((cosAlpha - cosPhi)/(cosTheta - cosPhi), spot_falloff);
	  }	
	}
	vec4 amb  = k_ambient * lights[i].ambient;		
    vec4 diff = k_diffuse * lights[i].diffuse  * max(dot(N,L), 0.0);
	vec4 spec = lights[i].specular * pow(max(dot(N,H), 0.0),((k_specular.r + 1.0) * 255.0));
			
	lightSum += (Att * amb) + (Att * spotEffect)*(diff + spec);
  }
  final_color = final_color + global_ambient * k_ambient + lightSum;
  //final_color = lights[0].diffuse;
  //final_color = vec4(.5,0,0,0);
  //final_color = vec4(1,1,1,1);
  //final_color = abs(N.x) * final_color;

  //Compute fog
  float z = gl_FragCoord.z / gl_FragCoord.w;
  //float z = length(v);
	
  float S;
  if(z < fog_near) S = 0.0;
  else if(z > fog_far) S = 1.0;
  else S = (z - fog_near)/(fog_far - fog_near);
	
  gl_FragColor = S*fog_color + (1.0 - S)*final_color;
  //gl_FragColor = v;
  //gl_FragColor = I_emissive;
  //gl_FragColor = debug_color;
  //gl_FragColor = v;
  //gl_FragColor = lights[0].position;
  //vec4 diff = lights[0].position.xyzw - v.xyzw;
  //gl_FragColor = vec4(diff.z, diff.z, diff.z, 0);
  //gl_FragColor = diff;
  //gl_FragColor = vec4(v.z,v.z,v.z,v.z);
  //vec4 debug_L = vec4(L.x,L.y,L.z,0);
  //vec4 debug_R = vec4(R.x,R.y,R.z,0);
  //gl_FragColor = debug_L;
  //gl_FragColor = final_color;
  //gl_FragColor = global_ambient;
  //gl_FragColor = vec4(0,1,0,0);
  //if(use_textures == 1)
  //gl_FragColor = vec4(0,1,0,0);
} 