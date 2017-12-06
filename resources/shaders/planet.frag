#version 150

in vec3 pass_WorldNormal;
in vec3 pass_WorldNormal_view;
in vec3 pass_vertPos;
in vec3 pass_vertPos_view;
in float pass_ShadingMethod;
in vec2 pass_TexCoord;
in vec3 pass_Tangent;


uniform sampler2D ColorTex;
uniform sampler2D NormalTex;

out vec4 out_Color;

//Light source
const vec3 sunPosition = vec3(0.0, 0.0, 0.0);

//indirect light incoming 
vec3 ambientColor = vec3(texture(ColorTex, pass_TexCoord))*0.1;

//diffusely reflected light
vec3 diffuseColor = vec3(texture(ColorTex, pass_TexCoord));

//Reflection of Light to viewer
const vec3 specColor = vec3(1.0, 1.0, 1.0);

//
vec3 norm_tex_color = vec3(texture(NormalTex, pass_TexCoord))* 2.0f - 1.0f;

const float difint = 0.95;
const float specint = 0.5;

//Shininess, woooohoooo
const float shininess = 50.0;

void main()
{
    vec3 bi_Tan = cross(pass_WorldNormal, pass_Tangent);
    mat3 Tan = mat3(pass_Tangent, bi_Tan, pass_WorldNormal);
    vec3 normal = normalize(Tan*norm_tex_color);
    vec3 normal_view = normalize(pass_WorldNormal_view);
    vec3 lightDir = normalize(sunPosition - pass_vertPos);
    vec3 colorLinear;

    float lambertian = max(dot(lightDir, normal), 0.0);

    float specular = 0.0;

    if ( pass_ShadingMethod < 1.0)
    {
        //specular highlight decay
        //float specular = 0.0;

        if(lambertian > 0.0){
            vec3 viewDir = normalize(-pass_vertPos);
            vec3 halfDir = normalize(lightDir + viewDir);
            float specAngle = max(dot(halfDir, normal), 0.0);
            specular = pow(specAngle, shininess);
        }

        colorLinear = ambientColor + lambertian * diffuseColor * difint + specular * specColor * specint;
    }

    else
    {
        float view_norm_angle = 0.0;

        vec3 viewDir_to_Sun = normalize(-pass_vertPos);
        vec3 halfDir = normalize(lightDir + viewDir_to_Sun);

        view_norm_angle = dot(-normalize(pass_vertPos_view), normal_view);

        if(lambertian > 0.0)
        {
            if (lambertian > 0.8) {lambertian = 1.0;}

            else if(lambertian > 0.6){lambertian = 0.8;} 

            else if(lambertian > 0.4){lambertian = 0.6;}

            else if(lambertian > 0.2){lambertian = 0.4;}

            else if(lambertian > 0.0){lambertian = 0.2;}
            
            float specAngle = max(dot(halfDir, normal), 0.0);
            specular = pow(specAngle, shininess);
        }

        if(abs(view_norm_angle) < 0.3)
        {
            //To generate cel outlines
            colorLinear = vec3(1.0,1.0,0.0);
        }

        else
        {
            colorLinear = ambientColor + lambertian * diffuseColor * difint + specular * specColor * specint;
        }
    }
    out_Color = vec4(colorLinear, 1.0);
   
}
