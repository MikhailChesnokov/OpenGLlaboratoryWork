#pragma once
#include <GLFW/glfw3.h>

class application
{
public:
	application(int width, int height, const char* title);

	virtual ~application();

	virtual void run() const;


	virtual void start()
	{
	}

	virtual void render(double time)
	{
	}

	virtual void finish()
	{
	}


	static GLuint load_shader(const char* file, const GLenum type);

	void application::load_model(const char* file_name, float * verices, GLushort * faces, int *vs, int *fs) const;

	static GLboolean checkout_shader_compilation(const GLuint shader);

	static GLboolean checkout_shader_link(const GLuint program);


	struct application_params
	{
		const char* window_title;
		int window_width;
		int window_height;
		int major_version;
		int minor_version;
	};

	static application* app;
protected:

	application_params appinfo_;
	GLFWwindow* window_;


	virtual void on_key(int key, int action)
	{
	}

	virtual void on_resize(int width, int height)
	{
	}

	virtual void on_mouse_button(int button, int action)
	{
	}

	virtual void on_mouse_move(int coord_x, int coords_y)
	{
	}

private:

	void init_app_params(const int width, const int height, const char* title);

	void init_callbacks() const;

	void create_window();


	static void window_on_key_handler(GLFWwindow* window, int key, int action, int scancode, int mods);

	static void window_on_resize_handler(GLFWwindow* window, int width, int height);

	static void window_on_mouse_button_handler(GLFWwindow* window, int button, int action, int mode);

	static void window_on_mouse_move_handler(GLFWwindow* window, double coord_x, double coord_y);
};
