#include "Application.h"

int main()
{
	try {
		Application app;
		app.Init(1920, 1080);

		app.Update();

		app.Terminate();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}