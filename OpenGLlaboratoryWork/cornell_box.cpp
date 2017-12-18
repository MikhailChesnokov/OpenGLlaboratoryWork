#include <GL/gl3w.h>
#include "cornell_box.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>
using namespace glm;

#define MAX_BUFFER_SIZE 4096
#define DRAW_ELEMENTS 4096

mat4 cornell_box::get_transform_matrix(const vec3 translate_v, const vec3 rotate_v, const vec3 scale_v)
{
	mat4 transform_matrix = mat4(1.0f);

	transform_matrix = translate(transform_matrix, translate_v);
	transform_matrix = rotate(transform_matrix, rotate_v.x, vec3(1.0f, 0.0f, 0.0f));
	transform_matrix = rotate(transform_matrix, rotate_v.y, vec3(0.0f, 1.0f, 0.0f));
	transform_matrix = rotate(transform_matrix, rotate_v.z, vec3(0.0f, 0.0f, 1.0f));
	transform_matrix = scale(transform_matrix, scale_v);

	return transform_matrix;
}

void cornell_box::attach_shaders(GLuint& program)
{
	const GLuint vs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/vs.glsl", GL_VERTEX_SHADER);
	const GLuint tcs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tcs.glsl", GL_TESS_CONTROL_SHADER);
	const GLuint tes = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/tes.glsl", GL_TESS_EVALUATION_SHADER);
	const GLuint fs = load_shader("../OpenGLlaboratoryWork/shaders/cornell_box/fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);

	glLinkProgram(program);

	checkout_shader_link(program);
}

void cornell_box::attach_model(const char* file_path, GLuint& extern_vertices_buffer, GLuint& extern_faces_buffer, const GLint location)
{
	GLfloat* vertices_buffer = new GLfloat[MAX_BUFFER_SIZE];
	GLsizeiptr vertices_buffer_size = 0;
	GLushort* faces_buffer = new GLushort[MAX_BUFFER_SIZE];
	GLsizeiptr faces_buffer_size = 0;

	load_model(file_path, vertices_buffer, faces_buffer, &vertices_buffer_size, &faces_buffer_size);

	glGenBuffers(1, &extern_vertices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, extern_vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices_buffer * vertices_buffer_size, vertices_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(location);

	glGenBuffers(1, &extern_faces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, extern_faces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof faces_buffer * faces_buffer_size, faces_buffer, GL_STATIC_DRAW);

	delete[] vertices_buffer;
	delete[] faces_buffer;
}

void cornell_box::attach_perspective_matrix(GLint& proj_uniform_location, application_params& appinfo)
{
	mat4 proj_matrix = perspective(
		50.0f,
		static_cast<float>(appinfo.window_width) / static_cast<float>(appinfo.window_height),
		0.1f,
		1000.0f);

	glUniformMatrix4fv(proj_uniform_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj_matrix));
}

void cornell_box::set_objects_attributes(const float current_time)
{
	objects_["room"].transform_matrix = get_transform_matrix(vec3(0.0f, 0.0f, -1.0f), vec3( 0.01f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f));
	objects_["room"].diffuse_color = vec4(0.0f, 0.1f, 0.8f, 1.0f);

	objects_["sphere"].transform_matrix = get_transform_matrix(vec3(-0.07f, -0.07f, -1.0f), vec3(current_time * 0.05f, current_time * 0.0f, 0.0f), vec3(0.03f, 0.03f, 0.03f));
	objects_["sphere"].diffuse_color = vec4(0.5f, 0.8f, 0.8f, 1.0f);

	objects_["rocks"].transform_matrix = get_transform_matrix(vec3(0.0f, 0.046f, -1.05f), vec3(3.14f, -0.05f, -0.1f), vec3(0.1f, 0.1f, 0.1f));
	objects_["rocks"].diffuse_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	objects_["trees"].transform_matrix = get_transform_matrix(vec3(0.0f, 0.046f, -1.05f), vec3(3.14f, 0.0f, -0.00 + sin(current_time) * 0.01f), vec3(0.1f, 0.1f, 0.1f));
	objects_["trees"].diffuse_color = vec4(0.0f, 0.8f, 0.1f, 1.0f);


	objects_["snow1"].transform_matrix = get_transform_matrix(vec3(-0.01 + sin(current_time) * 0.0025f, -0.00 + current_time * 0.0023f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow1"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	objects_["snow2"].transform_matrix = get_transform_matrix(vec3(+0.03 + sin(current_time) * 0.001f, -0.07 + current_time * 0.0021f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow2"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	objects_["snow3"].transform_matrix = get_transform_matrix(vec3(-0.05 + sin(current_time) * 0.003f, -0.03 + current_time * 0.0025f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow3"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	objects_["snow4"].transform_matrix = get_transform_matrix(vec3(+0.07 + sin(current_time) * 0.002f, -0.09 + current_time * 0.0027f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow4"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	objects_["snow5"].transform_matrix = get_transform_matrix(vec3(-0.0 + sin(current_time) * 0.001f, -0.08 + current_time * 0.0024f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow5"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	objects_["snow6"].transform_matrix = get_transform_matrix(vec3(0.08 + sin(current_time) * 0.002f, -0.02 + current_time * 0.0025f, -1.0f), vec3(current_time * 0.05f, 0.0f, current_time * 0.0f), vec3(0.001f, 0.001f, 0.001f));
	objects_["snow6"].diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void cornell_box::draw_objects()
{
	for (const std::map<std::basic_string<char>, scene_object>::value_type object : objects_)
	{
		scene_object obj = object.second;

		glUniform4f(color_uniform_location_, obj.diffuse_color.x, obj.diffuse_color.y, obj.diffuse_color.z, obj.diffuse_color.w);
		glUniformMatrix4fv(move_uniform_location_, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&obj.transform_matrix));

		glBindVertexArray(object.second.vao);
		glDrawElements(GL_PATCHES, DRAW_ELEMENTS, GL_UNSIGNED_SHORT, nullptr);
	}
}


void cornell_box::start()
{
	program_ = glCreateProgram();

	attach_shaders(program_);

	move_uniform_location_ = glGetUniformLocation(program_, "mv_matrix");
	proj_uniform_location_ = glGetUniformLocation(program_, "proj_matrix");
	color_uniform_location_ = glGetUniformLocation(program_, "my_color");

	objects_["room"] = scene_object("../OpenGLlaboratoryWork/models/cube.obj");
	objects_["sphere"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["rocks"] = scene_object("../OpenGLlaboratoryWork/models/rocks.obj");
	objects_["trees"] = scene_object("../OpenGLlaboratoryWork/models/trees.obj");

	objects_["snow1"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["snow2"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["snow3"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["snow4"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["snow5"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
	objects_["snow6"] = scene_object("../OpenGLlaboratoryWork/models/sphere.obj");
}

void cornell_box::render(const double curr_time)
{
	static const GLfloat background_color[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, background_color);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(program_);

	attach_perspective_matrix(proj_uniform_location_, appinfo_);

	set_objects_attributes(static_cast<float>(curr_time));

	draw_objects();
}

void cornell_box::finish()
{
	for (const auto object : objects_)
	{
		scene_object obj = object.second;
		glDeleteVertexArrays(1, &obj.vao);
		glDeleteBuffers(1, &obj.vertices_buffer);
		glDeleteBuffers(1, &obj.faces_buffer);
	}
	glDeleteProgram(program_);
}
