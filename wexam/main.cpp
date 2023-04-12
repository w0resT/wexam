#include <iostream>
#include "application/application.h"	

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	try {
		Application app;
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << "[error] " << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
