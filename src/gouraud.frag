#version 330 core
in vec4 colourV;
out vec4 fragColour;
in vec4 lightDir;
in vec4 normal;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main(void)
{
	float diff = max(dot(normal, lightDir), 0.0);
	fragColour = /*texture(ourTexture, TexCoord)*/vec4(1.0, 1.0, 1.0, 1.0) * diff;
}