#pragma once
#include "imgui/imgui.h"
#include "imgui/backend/imgui_impl_glfw.h"
#include "imgui/backend/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include "GLFW/glfw3.h"

#include "../interfaces/gui/iimguimanager.h"

class ImGuiManager : public IImGuiManager {
public:
	ImGuiManager(GLFWwindow* _window) : m_GLFWWindow(_window) {}

	void Init() override;
	void Shutdown() override;
	void NewFrame() override;
	void Render() override;

private:
	GLFWwindow* m_GLFWWindow;
};