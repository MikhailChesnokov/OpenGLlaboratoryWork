#include <cstdlib>
#include <GLFW/glfw3.h>

#include "Application.h"

int main()
{
	const auto app = new application(800, 600, "MyApp");
	app->run();
	delete app;

	exit(EXIT_SUCCESS);
}