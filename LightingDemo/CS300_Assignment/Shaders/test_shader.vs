varying vec3 N;
varying vec4 v;
varying vec3 n;
uniform mat4 view_matrix;
varying vec4 eyePos;
attribute vec3 tangent;
attribute vec3 binormal;
varying vec3 f_tangent;
varying vec3 f_binormal;

void main(void)
{
    v = (gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	vec4 temp_tan = vec4(tangent.x, tangent.y, tangent.z, 0.0);
	vec4 temp_bin = vec4(binormal.x, binormal.y, binormal.z, 0.0);
	f_tangent = (gl_ModelViewMatrix * temp_tan);
	f_binormal = (gl_ModelViewMatrix * temp_bin);
	//f_tangent = (gl_NormalMatrix * tangent);
	//f_binormal = (gl_NormalMatrix * binormal);
    
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;	
}