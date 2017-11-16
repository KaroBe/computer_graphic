#version 150

in vec3 pass_Normal;
//in vec4 gl_Position
out vec4 out_Color;

void main() {
  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
}
