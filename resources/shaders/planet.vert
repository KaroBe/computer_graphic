#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec3 SunPosition;

out vec3 pass_Normal;
out vec3 pass_SunPosition;
out vec3 vert_Position;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	
	vec4 vert_Position4 = ViewMatrix * vec4(in_Position, 1.0);
	
	vec3 vert_Position = vec3(vert_Position4)/vert_Position4.w;

	pass_SunPosition = (ViewMatrix * vec4(SunPosition, 1)).xyz; // richtig rum so?

	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
}
