#include "simple_triangle.h"

#undef GL_VERSION_1_1

#include <gl/gl3w.h>
#include <GLFW/glfw3.h>

void simple_triangle::start()
{
	const GLuint vs = load_shader("../OpenGLlaboratoryWork/shaders/triangle.vs.glsl", GL_VERTEX_SHADER);
	const GLuint fs = load_shader("../OpenGLlaboratoryWork/shaders/triangle.fs.glsl", GL_FRAGMENT_SHADER);

	program_ = glCreateProgram();

	glAttachShader(program_, vs);
	glAttachShader(program_, fs);

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