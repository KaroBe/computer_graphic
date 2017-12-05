#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec2 pass_TexCoord;

void main(void)
{
	gl_Position = (ProjectionMatrix * mat4(mat3(ViewMatrix))) * vec4(in_Position, 1.0);
	pass_TexCoord = in_TexCoord;
}
