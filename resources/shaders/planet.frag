#version 150/*#

in vec3 pass_Normal;
in vec4 pass_sunPosition;

out vec4 out_Color;

void main() {
  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
}*/

in vec3 pass_sunPosition;
in vec3 vert_Position;
in vec4 gl_FragCoord;
in vec3 pass_Normal;

const vec3 ambientColor = vec3(0.1, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space
const vec3 sunPosition= vec3(0.0,0.0,0.0);
void main() {

  vec3 normal = normalize(pass_Normal);
  vec3 lightDir = normalize(pass_sunPosition - vert_Position);

  float lambertian = max(dot(lightDir,normal), 0.0);
  float specular = 0.1;

  if(lambertian > 0.0) {

    vec3 viewDir = normalize(vert_Position);

    // this is blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specAngle, shininess);

    }
  
  vec3 colorLinear = ambientColor +
                     lambertian * diffuseColor +
                     specular * specColor;
  // apply gamma correction (assume ambientColor, diffuseColor and specColor
  // have been linearized, i.e. have no gamma correction in them)
  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
  // use the gamma corrected color in the fragment
  gl_FragColor = vec4(colorGammaCorrected, 1.0);
}