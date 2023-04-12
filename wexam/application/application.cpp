#include "application.h"
#include "../gui/gui.h"

void Application::Run() {
	// Running GUI
	Gui gui(nullptr);
	gui.Init();
	gui.Run();
	gui.Shutdown();
}