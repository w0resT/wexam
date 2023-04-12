#pragma once
#include "GLFW/glfw3.h"

#include "interfaces/igui.h"
#include "interfaces/iimguimanager.h"

#include <memory>

class Gui : public IGui {
public:
	Gui(GLFWwindow *_window) : m_GLFWWindow(_window) {}

	void Init() override;
	void Run() override;
	void Shutdown() override;

private:
	GLFWwindow* m_GLFWWindow;
	std::unique_ptr<IImGuiManager> m_ptrImguiManager;
};