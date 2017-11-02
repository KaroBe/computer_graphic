layout(location = 0) in vec 3 in_Position;
layout(location = 0) in vec 3 in_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec3 pass_Normal;

void main()
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
}