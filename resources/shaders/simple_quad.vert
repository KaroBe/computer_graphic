#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

uniform float H_Mirrored;
uniform float V_Mirrored;

out vec2 pass_TexCoord;

void main()
{
	vec2 TexCoord = in_TexCoord;

	if(H_Mirrored == 1.0f){
		if(in_TexCoord.y == 1) 	{TexCoord.y = 0.0;}
		else 					{TexCoord.y = 1.0;}
	}
	if(V_Mirrored == 1.0f){
		if(in_TexCoord.x == 1) 	{TexCoord.x = 0.0;}
		else 					{TexCoord.x = 1.0;}
	}

	gl_Position = vec4(in_Position, 1.0);
	pass_TexCoord = TexCoord;
}
