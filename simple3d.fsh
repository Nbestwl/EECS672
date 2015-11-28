#version 410 core

in vec3 colorToFS;

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(colorToFS, 1.0);
}
