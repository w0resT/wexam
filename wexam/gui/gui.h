#pragma once
#include <memory>

#include "GLFW/glfw3.h"

#include "interfaces/igui.h"
#include "interfaces/iimguimanager.h"

#include "../model/interfaces/iappmodel.h"

#include "interfaces/ipagemanager.h"

struct ImFont;

class Gui : public IGui{
public:
	Gui(std::shared_ptr<IAppModel> _model) : m_GLFWWindow(nullptr), m_Model(std::move(_model)),
		m_Segoeui18(nullptr), m_SegoeuiBold18(nullptr), m_SegoeuiBold32(nullptr), m_bShouldClose(false) {}

	~Gui() {}

	void Init() override;
	void Run() override;
	void Shutdown() override;

private:
	void Draw();
	void CreateTabs();

	void CreateFonts();
	void LoadStyle();

private:
	GLFWwindow* m_GLFWWindow;
	std::shared_ptr<IAppModel> m_Model;
	std::unique_ptr<IImGuiManager> m_ImguiManager;
	std::unique_ptr<IPageManager> m_PageManager;

	ImFont* m_Segoeui18;
	ImFont* m_SegoeuiBold18;
	ImFont* m_SegoeuiBold32;

	bool m_bShouldClose;
};