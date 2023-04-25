#pragma once
#include <memory>

#include "GLFW/glfw3.h"

#include "interfaces/igui.h"
#include "interfaces/iimguimanager.h"

#include "../model/interfaces/iappmodel.h"

class Gui : public IGui{
public:
	Gui(std::shared_ptr<IAppModel> _model) : m_GLFWWindow(nullptr), m_Model(std::move(_model)), m_bShouldClose(false) {}

	void Init() override;
	void Run() override;
	void Shutdown() override;

private:
	GLFWwindow* m_GLFWWindow;
	std::shared_ptr<IAppModel> m_Model;
	std::unique_ptr<IImGuiManager> m_ptrImguiManager;

private:
	bool m_bShouldClose;
};