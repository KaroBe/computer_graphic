#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 pass_WorldNormal;
out vec3 pass_WorldNormal_view;
out vec3 pass_vertPos;
out vec3 pass_vertPos_view;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	vec4 vertPos4 = ModelMatrix * vec4(in_Position, 1.0);
	pass_vertPos = vec3(vertPos4).xyz / vertPos4.w;

	vec4 vertPos4_view = ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);
	pass_vertPos_view = vec3(vertPos4_view).xyz / vertPos4_view.w;
	//In Worlspace
	pass_WorldNormal = (ModelMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_WorldNormal_view = (ViewMatrix * ModelMatrix * vec4(in_Normal, 0.0)).xyz;
}
