#version 460 core

out vec4 color;
uniform vec4 my_color;

void main(void)
{
	color = my_color;
}