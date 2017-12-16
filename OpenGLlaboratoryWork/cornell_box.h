#pragma once
#include "Application.h"

class cornell_box : public application
{
public:
	cornell_box(const int width, const int height, const char* title)
		: application(width, height, title), program_(0), vao_(0)
	{
	}

	void start() override;

	void render(const double current_time) override;

	void finish() override;

private:
	GLuint program_;
	GLuint vao_;
	GLuint position_buffer;
	GLuint index_buffer;
	GLint mv_location;
	GLint proj_location;
	GLint color;
};
