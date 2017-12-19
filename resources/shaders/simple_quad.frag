#version 150 core

in vec2 pass_TexCoord;

uniform sampler2D ColorTex;
uniform float GrayScale;
uniform float Blur;
uniform float Sobel;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;

out vec4 out_Color;

void main()
{

    vec4 Color = texture(ColorTex, pass_TexCoord);


    // gaussian blur
    if (Blur == 1.0f)
    {
    	vec4 sum = vec4(0.0);
	    for (int x = -4; x <= 4; x++)
	        for (int y = -4; y <= 4; y++)
	            sum += texture(ColorTex, vec2(pass_TexCoord.x + x * blurSizeH, pass_TexCoord.y + y * blurSizeV)) / 81.0;
	    Color = sum;
    }

    // sobel stuff
    if (Sobel == 1.0f)
    {
    	vec4 top         = texture(ColorTex, vec2(pass_TexCoord.x, pass_TexCoord.y + 1.0 / 200.0));
		vec4 bottom      = texture(ColorTex, vec2(pass_TexCoord.x, pass_TexCoord.y - 1.0 / 200.0));
		vec4 left        = texture(ColorTex, vec2(pass_TexCoord.x - 1.0 / 300.0, pass_TexCoord.y));
		vec4 right       = texture(ColorTex, vec2(pass_TexCoord.x + 1.0 / 300.0, pass_TexCoord.y));
		vec4 topLeft     = texture(ColorTex, vec2(pass_TexCoord.x - 1.0 / 300.0, pass_TexCoord.y + 1.0 / 200.0));
		vec4 topRight    = texture(ColorTex, vec2(pass_TexCoord.x + 1.0 / 300.0, pass_TexCoord.y + 1.0 / 200.0));
		vec4 bottomLeft  = texture(ColorTex, vec2(pass_TexCoord.x - 1.0 / 300.0, pass_TexCoord.y - 1.0 / 200.0));
		vec4 bottomRight = texture(ColorTex, vec2(pass_TexCoord.x + 1.0 / 300.0, pass_TexCoord.y - 1.0 / 200.0));
		vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
		vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
		vec4 sobel = sqrt(sx * sx + sy * sy);
		Color = (sobel + Color)/2;
    }

    //checks, if grayscale is activated & computes average Color
    if (GrayScale == 1.0f)
    {
    	float avg_clr = (Color.x + Color.y + Color.z)/3;
    	Color = vec4(avg_clr, avg_clr, avg_clr, 1);
    }
    out_Color = Color;
}
