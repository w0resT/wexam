#include <thread>

#include "application.h"
#include "../gui/gui.h"
#include "../controller/appcontroller.h"
#include "../model/appmodel.h"

void Application::Run() {
	std::shared_ptr<IAppModel> model = std::make_shared<AppModel>();
	std::shared_ptr<IGui> gui = std::make_shared<Gui>(model);
	std::shared_ptr<IAppController> controller = std::make_shared<AppController>(model, gui);

	// Initialization GUI
	gui->Init();

	// Register controller as observer for gui events
	gui->RegisterObserver(controller);

	// Start controller in another thread
	std::thread controllerThread(&IAppController::Start, controller);

	// Run GUI (main thread)
	gui->Run();

	// Waiting for controller shutdown
	controllerThread.join();

	// Shutdown GUI
	gui->Shutdown();
}