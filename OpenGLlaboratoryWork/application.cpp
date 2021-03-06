﻿#pragma once
#include <GL/gl3w.h>
#pragma once
#include <GLFW/glfw3.h>

#pragma once
#include "Application.h"

#include <cstdlib>
#include <istream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

application* application::app = nullptr;

application::application(const int width, const int height, const char* title)
{
	app = this;

	if (!glfwInit())
	{
		std::cout << "Cannot initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	init_app_params(width, height, title);

	create_window();

	glfwMakeContextCurrent(window_);

	init_callbacks();

	if (gl3wInit())
	{
		std::cout << "Cannot initialize gl3w" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	if (!gl3wIsSupported(appinfo_.major_version, appinfo_.minor_version))
	{
		std::cout << "Initialize fail: OpenGL " << appinfo_.major_version << "." << appinfo_.minor_version << "unsupported" <<
			std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	std::cout << "Initialize successful: OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(
		GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

application::~application()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void application::run() const
{
	app->start();

	do
	{
		app->render(glfwGetTime());

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	while (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_RELEASE &&
		glfwWindowShouldClose(window_) != GL_TRUE);

	app->finish();
}

GLuint application::load_shader(const char* file_name, const GLenum type)
{
	char* shader_code = new char[8192];
	memset(shader_code, 0, 8192);

	std::ifstream shader_file(file_name);

	if (shader_file.is_open())
	{
		const std::streamoff size = shader_file.seekg(0, std::ios::end).tellg();
		shader_file.seekg(0);
		shader_file.read(shader_code, size);
		shader_file.close();
	}

	const GLuint shader = glCreateShader(type);

	if (!shader)
	{
		std::cout << "Fail shader allocation" << std::endl;
		return 0;
	}

	glShaderSource(shader, 1, &shader_code, nullptr);

	delete[] shader_code;

	glCompileShader(shader);

	if (!app->checkout_shader_compilation(shader))
		return 0;

	return shader;
}

void application::load_model(const char* file_name, float* verices, GLushort* faces, int* vs, int* fs)
{
	int vertices_count = 0, faces_count = 0;
	std::ifstream infile;
	infile.open(file_name);
	std::string line;
	char c;
	while (getline(infile, line))
	{
		std::istringstream iss(line);
		switch (line[0])
		{
		case 'v':
			iss
				>> c
				>> verices[vertices_count++]
				>> verices[vertices_count++]
				>> verices[vertices_count++];
			break;
		case 'f':
			iss
				>> c
				>> faces[faces_count++]
				>> faces[faces_count++]
				>> faces[faces_count++];
			faces[faces_count - 3]--;
			faces[faces_count - 2]--;
			faces[faces_count - 1]--;
			break;
		default: break;
		}
	}
	infile.close();
	*vs = vertices_count;
	*fs = faces_count;
}

GLboolean application::checkout_shader_compilation(const GLuint shader)
{
	GLint is_compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

	if (is_compiled == GL_FALSE)
	{
		std::cout << "Shader compilation error" << std::endl;

		GLint max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> buffer(max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, &buffer[0]);

		std::cout << buffer.data() << std::endl;

		glDeleteShader(shader);

		return GL_FALSE;
	}

	return GL_TRUE;
}

GLboolean application::checkout_shader_link(const GLuint program)
{
	GLint is_linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int *>(&is_linked));
	if (is_linked == GL_FALSE)
	{
		GLint max_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> info_log(max_length);
		glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

		std::cout << info_log.data() << std::endl;

		glDeleteProgram(program);

		return GL_FALSE;
	}
	return GL_TRUE;
}

void application::init_app_params(const int width, const int height, const char* title)
{
	appinfo_.window_width = width;
	appinfo_.window_height = height;

	appinfo_.major_version = 4;
	appinfo_.minor_version = 6;

	appinfo_.window_title = title;
}

void application::init_callbacks() const
{
	glfwSetWindowSizeCallback(window_, window_on_resize_handler);
	glfwSetKeyCallback(window_, window_on_key_handler);
	glfwSetMouseButtonCallback(window_, window_on_mouse_button_handler);
	glfwSetCursorPosCallback(window_, window_on_mouse_move_handler);
}

void application::create_window()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, appinfo_.major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, appinfo_.minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_STEREO, 0);

	window_ = glfwCreateWindow(appinfo_.window_width, appinfo_.window_height, appinfo_.window_title, nullptr, nullptr);
	if (!window_)
	{
		std::cout << "Cannot initialize GLFW Window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

void application::window_on_key_handler(GLFWwindow* window, const int key, const int action, int scancode, int mods)
{
	app->on_key(key, action);
}

void application::window_on_resize_handler(GLFWwindow* window, const int width, const int height)
{
	app->on_resize(width, height);
}

void application::window_on_mouse_button_handler(GLFWwindow* window, const int button, const int action, int mode)
{
	app->on_mouse_button(button, action);
}

void application::window_on_mouse_move_handler(GLFWwindow* window, const double coord_x, const double coord_y)
{
	app->on_mouse_move(static_cast<int>(coord_x), static_cast<int>(coord_y));
}
