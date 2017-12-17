#include <GL/gl3w.h>
#include "cornell_box.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>
using namespace glm;

#define MAX_BUFFER_SIZE 1024

mat4 static get_transform_matrix(const vec3 translate_v, const vec3 rotate_v, const vec3 scale_v)
{
	mat4 transform_matrix = mat4(1.0f);

	transform_matrix = translate(transform_matrix, translate_v);
	transform_matrix = rotate(transform_matrix, rotate_v.x, vec3(1.0f, 0.0f, 0.0f));
	transform_matrix = rotate(transform_matrix, rotate_v.y, vec3(0.0f, 1.0f, 0.0f));
	transform_matrix = rotate(transform_matrix, rotate_v.z, vec3(0.0f, 0.0f, 1.0f));
	transform_matrix = scale(transform_matrix, scale_v);

	return transform_matrix;
}

void static attach_shaders(GLuint& program)
{
	const GLuint vs = application::load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/vs.glsl", GL_VERTEX_SHADER);
	const GLuint tcs = application::load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tcs.glsl", GL_TESS_CONTROL_SHADER);
	const GLuint tes = application::load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tes.glsl", GL_TESS_EVALUATION_SHADER);
	const GLuint fs = application::load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);

	glLinkProgram(program);

	application::checkout_shader_link(program);
}

void static attach_model(const char* file_path, GLuint& extern_vertices_buffer, GLuint& extern_faces_buffer)
{
	GLfloat* vertices_buffer = new GLfloat[MAX_BUFFER_SIZE];
	GLsizeiptr vertices_buffer_size = 0;
	GLushort* faces_buffer = new GLushort[MAX_BUFFER_SIZE];
	GLsizeiptr faces_buffer_size = 0;

	application::load_model(file_path, vertices_buffer, faces_buffer, &vertices_buffer_size, &faces_buffer_size);

	glGenBuffers(1, &extern_vertices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, extern_vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices_buffer * vertices_buffer_size, vertices_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &extern_faces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, extern_faces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof faces_buffer * faces_buffer_size, faces_buffer, GL_STATIC_DRAW);

	delete[] vertices_buffer;
	delete[] faces_buffer;
}

void static attach_perspective_matrix(GLint& proj_uniform_location, application::application_params& appinfo)
{
	mat4 proj_matrix = perspective(
		50.0f,
		static_cast<float>(appinfo.window_width) / static_cast<float>(appinfo.window_height),
		0.1f,
		1000.0f);

	glUniformMatrix4fv(proj_uniform_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj_matrix));
}

void cornell_box::start()
{
	program_ = glCreateProgram();

	attach_shaders(program_);

	move_uniform_location_ = glGetUniformLocation(program_, "mv_matrix");
	proj_uniform_location_ = glGetUniformLocation(program_, "proj_matrix");
	color_uniform_location_ = glGetUniformLocation(program_, "my_color");

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	attach_model("../OpenGLlaboratoryWork/models/cube.obj", room_verts_buffer_, room_faces_buffer_);
}

void cornell_box::render(const double time)
{
	const auto current_time = static_cast<float>(time);

	static const GLfloat background_color[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, background_color);

	glUseProgram(program_);

	attach_perspective_matrix(proj_uniform_location_, appinfo_);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_SMOOTH);


	mat4 mv_matrix = get_transform_matrix(vec3(0.0f, 0.0f, -1.0f), vec3(current_time * 0.08f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f));

	glUniform4f(color_uniform_location_, 1.0f, 0.1f, 0.2f, 0.5f);
	glUniformMatrix4fv(move_uniform_location_, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mv_matrix));

	glDrawElements(GL_PATCHES, 36, GL_UNSIGNED_SHORT, 0);


	mv_matrix = get_transform_matrix(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(move_uniform_location_, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mv_matrix));
	glUniform4f(color_uniform_location_, 0.1f, 0.8f, 0.1f, 0.5f);

	glDrawElements(GL_PATCHES, 36, GL_UNSIGNED_SHORT, 0);
}

void cornell_box::finish()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
	glDeleteBuffers(1, &room_verts_buffer_);
	glDeleteBuffers(1, &room_faces_buffer_);
}
