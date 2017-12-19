#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

out vec2 pass_TexCoord;

void main()
{
    pass_TexCoord = in_TexCoord;
    gl_Position = vec4(in_Position, 1.0);
}
