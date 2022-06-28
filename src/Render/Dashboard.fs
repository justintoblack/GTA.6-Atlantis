#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D gDashboardTexture;

void main()
{
	vec4 texColor= texture(gDashboardTexture, TexCoord);
	if(texColor.a<0.5)
		discard;
	FragColor=texColor;
}