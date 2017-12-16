#include <cstdlib>

#include "cornell_box.h"

int main()
{
	const auto app = new cornell_box(850, 850, "Box");
	app->run();
	delete app;

	exit(EXIT_SUCCESS);
}
