#version 150

in vec3 pass_WorldNormal;
in vec3 pass_WorldNormal_view;
in vec3 pass_vertPos;
in vec3 pass_vertPos_view;

out vec4 out_Color;

//Blinn-Phong / Cel
int mode = 2;

//Light source
const vec3 sunPosition = vec3(0.0, 0.0, 0.0);

//indirect light incoming 
const vec3 ambientColor = vec3(0.0005, 0.0005, 0.0005);

//diffusely reflected light
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);

//Reflection of Light to viewer
const vec3 specColor = vec3(1.0, 1.0, 1.0);

const float difint = 0.5;
const float specint = 0.9;

//Shininess, woooohoooo
const float shininess = 100.0;

void main()
{
    int mode = 1;

    if ( mode == 1)
    {
        vec3 normal = normalize(pass_WorldNormal);

        //Vector between FragCoord and SunPosition
        vec3 lightDir = normalize(sunPosition - pass_vertPos);

        //lambertian reflectance
        float lambertian = max(dot(lightDir, normal), 0.0);

        //specular highlight decay
        float specular = 0.0;

        if(lambertian > 0.0)
        {
            vec3 viewDir = normalize(-pass_vertPos);
            vec3 halfDir = normalize(lightDir + viewDir);
            float specAngle = max(dot(halfDir, normal), 0.0);
            specular = pow(specAngle, shininess);
        }

        vec3 colorLinear = ambientColor + lambertian * diffuseColor * difint + specular * specColor * specint;

        out_Color = vec4(colorLinear, 1.0);
    }

    else
    {
        vec3 normal = normalize(pass_WorldNormal);
        vec3 normal_view = normalize(pass_WorldNormal_view);

        //Vector between Vertex Position and SunPosition
        vec3 lightDir = normalize(sunPosition - pass_vertPos);

        //lambertian reflectance
        float lambertian = max(dot(lightDir, normal), 0.0);

        //specular highlight decay
        float specular = 0.0;

        float view_norm_angle = 0.0;

        vec3 colorLinear;

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
            colorLinear= vec3(1.0,1.0,0.0);
        }

        else
        {
            colorLinear = ambientColor + lambertian * diffuseColor * difint + specular * specColor * specint;
        }

        out_Color = vec4(colorLinear, 1.0);
    }
}
