#include "simple_triangle.h"

#undef GL_VERSION_1_1

#include <gl/gl3w.h>
#include <GLFW/glfw3.h>

void simple_triangle::start()
{

	static const char * vs_source[] =
	{
		"#version 460 core                                                 \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    const vec4 vertices[] = vec4[](vec4( 0.4, -0.4, 0.5, 1.0),    \n"
		"                                   vec4(-0.4, -0.4, 0.5, 1.0),    \n"
		"                                   vec4( 0.4,  0.4, 0.5, 1.0));   \n"
		"                                                                  \n"
		"    gl_Position = vertices[gl_VertexID];                          \n"
		"}                                                                 \n"
	};

	static const char * fs_source[] =
	{
		"#version 460 core                                                 \n"
		"                                                                  \n"
		"out vec4 color;                                                   \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(1.0, 0.0, 1.0, 1.0);                             \n"
		"}                                                                 \n"
	};

	program_ = glCreateProgram();

	const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fs_source, nullptr);
	glCompileShader(fragment_shader);

	const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, nullptr);
	glCompileShader(vs);

	glAttachShader(program_, vs);
	glAttachShader(program_, fragment_shader);

	glLinkProgram(program_);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}

void simple_triangle::render()
{

	static const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, white);

	glUseProgram(program_);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void simple_triangle::finish()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
}