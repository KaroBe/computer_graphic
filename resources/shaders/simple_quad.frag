#version 150 core

in vec2 pass_TexCoord;

out vec4 outColor;
uniform sampler2D ColorTex;

void main()
{
    outColor = texture(ColorTex, pass_TexCoord);
}
