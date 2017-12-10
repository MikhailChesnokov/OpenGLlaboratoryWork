#include <GL/gl3w.h>

#include "simple_triangle.h"

#include <math.h>

void simple_triangle::start()
{
	program_ = glCreateProgram();

	const GLuint vs = load_shader("../OpenGLlaboratoryWork/shaders/triangle.vs.glsl", GL_VERTEX_SHADER);
	const GLuint tcs = load_shader("../OpenGLlaboratoryWork/shaders/triangle.tcs.glsl", GL_TESS_CONTROL_SHADER);
	const GLuint tes = load_shader("../OpenGLlaboratoryWork/shaders/triangle.tes.glsl", GL_TESS_EVALUATION_SHADER);
	const GLuint fs = load_shader("../OpenGLlaboratoryWork/shaders/triangle.fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program_, vs);
	glAttachShader(program_, tcs);
	glAttachShader(program_, tes);
	glAttachShader(program_, fs);

	glLinkProgram(program_);

	if (!checkout_shader_link(program_))
		return;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void simple_triangle::render(const double current_time)
{
	static const GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, white);

	glUseProgram(program_);
	glDrawArrays(GL_PATCHES, 0, 3);

	GLfloat attributes[] =
	{
		static_cast<float>(cos(current_time)) * 0.5,
		static_cast<float>(sin(current_time)) * 0.1,
		0.0, 0.0
	};

	glVertexAttrib4fv(0, attributes);
}

void simple_triangle::finish()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
}
