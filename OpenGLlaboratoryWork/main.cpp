#include <cstdlib>

#include "simple_triangle.h"

int main()
{
	const auto app = new simple_triangle(1024, 768, "MyApp");
	app->run();
	delete app;

	exit(EXIT_SUCCESS);
}
