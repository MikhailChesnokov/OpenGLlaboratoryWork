#pragma once
#include "Application.h"

class simple_triangle : public application
{
public:
	simple_triangle(const int width, const int height, const char* title)
		: application(width, height, title), program_(0), vao_(0)
	{
	}

	void start() override;

	void render(const double current_time) override;

	void finish() override;

private:
	GLuint program_;
	GLuint vao_;
};
