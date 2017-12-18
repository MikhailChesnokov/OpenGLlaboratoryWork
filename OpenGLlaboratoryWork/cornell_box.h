#pragma once
#include <gl/gl3w.h>
#pragma once
#include "Application.h"

#include <map>
#include <glm.hpp>


class cornell_box : public application
{
public:
	cornell_box(const int width, const int height, const char* title)
		: application(width, height, title), program_(0), move_uniform_location_(0), proj_uniform_location_(0), color_uniform_location_(0) { }

	glm::mat4 static cornell_box::get_transform_matrix(const glm::vec3 translate_v, const glm::vec3 rotate_v, const glm::vec3 scale_v);

	void static cornell_box::attach_shaders(GLuint& program);

	void static cornell_box::attach_model(const char* file_path, GLuint& extern_vertices_buffer, GLuint& extern_faces_buffer, const GLint location);

	void static cornell_box::attach_perspective_matrix(GLint& proj_uniform_location, application_params& appinfo);

	void cornell_box::set_objects_attributes(const float current_time);

	void cornell_box::draw_objects();


	void start() override;

	void render(const double current_time) override;

	void finish() override;


	struct scene_object
	{
		GLuint vao;

		GLuint vertices_buffer;
		GLuint faces_buffer;

		glm::mat4 transform_matrix;

		glm::vec4 diffuse_color;

		scene_object(): vao(0), vertices_buffer(0), faces_buffer(0) { }

		explicit scene_object(char* file_path)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			attach_model(file_path, vertices_buffer, faces_buffer, 0);

			transform_matrix = glm::mat4(1.0f);

			diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
		}
	};

private:
	GLuint program_;

	GLint move_uniform_location_;
	GLint proj_uniform_location_;
	GLint color_uniform_location_;

	std::map<std::string, scene_object> objects_;
};
