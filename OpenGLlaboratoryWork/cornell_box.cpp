#include <GL/gl3w.h>
#include "cornell_box.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <xutility>
using namespace glm;

void cornell_box::start()
{
	program_ = glCreateProgram();

	const GLuint vs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/vs.glsl", GL_VERTEX_SHADER);
	const GLuint tcs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tcs.glsl", GL_TESS_CONTROL_SHADER);
	const GLuint tes = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tes.glsl", GL_TESS_EVALUATION_SHADER);
	const GLuint fs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program_, vs);
	glAttachShader(program_, tcs);
	glAttachShader(program_, tes);
	glAttachShader(program_, fs);

	glLinkProgram(program_);

	if (!checkout_shader_link(program_))
		return;

	mv_location = glGetUniformLocation(program_, "mv_matrix");
	proj_location = glGetUniformLocation(program_, "proj_matrix");
	color = glGetUniformLocation(program_, "my_color");

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	GLfloat * verts = new GLfloat[1024]; GLsizeiptr verts_size = 0;
	GLushort *faces = new GLushort[1024]; GLsizeiptr faces_size = 0;
	load_model("../OpenGLlaboratoryWork/models/cube.obj", verts, faces, &verts_size, &faces_size);

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof verts * verts_size, verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof faces * faces_size, faces, GL_STATIC_DRAW);
}

void cornell_box::render(const double time)
{
	const float current_time = static_cast<float>(time);
	static const GLfloat black[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	glViewport(0, 0, appinfo_.window_width, appinfo_.window_height);
	glClearBufferfv(GL_COLOR, 0, black);

	glUseProgram(program_);
	
	mat4 proj_matrix = perspective(50.0f,
		static_cast<float>(appinfo_.window_width) / static_cast<float>(appinfo_.window_height),
		0.1f,
		1000.0f);
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj_matrix));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_SMOOTH);

	mat4 mv_matrix = mat4(1.0f);
	
	mv_matrix = translate(mv_matrix, vec3(0.0f, 0.0f, -1.0f));
	mv_matrix = scale(mv_matrix, vec3(0.10f, 0.10f, 0.10f));
	mv_matrix = rotate(mv_matrix, current_time * 0.08f, vec3(1.0f, 0.0f, 0.0f));

	glUniform4f(color, 1.0f, 0.1f, 0.2f, 0.5f);
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mv_matrix));
	glDrawElements(GL_PATCHES, 36, GL_UNSIGNED_SHORT, 0);

	mv_matrix = scale(mv_matrix, vec3(0.1f, 0.1f, 0.1f));
	mv_matrix = rotate(mv_matrix, current_time * 0.08f, vec3(1.0f, 0.0f, 0.0f));
	mv_matrix = rotate(mv_matrix, 3.14f, vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mv_matrix));

	glUniform4f(color, 0.5f, 0.5f, 0.5f, 0.5f);
	glDrawElements(GL_PATCHES, 36, GL_UNSIGNED_SHORT, 0);
}

void cornell_box::finish()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
	glDeleteBuffers(1, &position_buffer);
}
