#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
out vec4 colourV;
out vec4 lightDir;
out vec4 normal;
out vec2 TexCoord;
uniform float xOffset;
uniform float yOffset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	TexCoord = aTex;
	normal = vec4(aNormal.x, aNormal.y, aNormal.z, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	colourV = vec4(1.0, 1.0, 1.0, 1.0);
	lightDir = normalize(vec4(10.0, 10.0, 10.0, 1.0) - gl_Position);
}