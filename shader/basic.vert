#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 overrideColor;

void main()
{
	TexCoords = texCoords;
	Normal = mat3(transpose(inverse(model))) * normal;
	Position = vec3(model * vec4(position, 1.0f));
	gl_Position = proj * view * model * vec4(position, 1.0);
}