#pragma once
#include "Application.h"

class cornell_box : public application
{
public:
	cornell_box(const int width, const int height, const char* title)
		: application(width, height, title), program_(0), vao_(0), room_verts_buffer_(0), room_faces_buffer_(0),
		  move_uniform_location_(0), proj_uniform_location_(0), color_uniform_location_(0)
	{
	}

	void start() override;

	void render(const double current_time) override;

	void finish() override;

private:
	GLuint program_;
	GLuint vao_;

	GLuint room_verts_buffer_;
	GLuint room_faces_buffer_;

	GLint move_uniform_location_;
	GLint proj_uniform_location_;
	GLint color_uniform_location_;
};
