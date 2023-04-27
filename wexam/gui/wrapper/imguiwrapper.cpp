#include "imguiwrapper.h"
#include "../../defines.h"

ImGuiWrapper::ImGuiWrapper() {
	m_WindowSize = ImVec2(defines::WINDOW_W, defines::WINDOW_H);
	m_WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
}

ImGuiWrapper::~ImGuiWrapper() {
}

void ImGuiWrapper::SetWindowSize(const ImVec2& size) {
	m_WindowSize = size;
}

ImVec2 ImGuiWrapper::GetWindowSize() const {
	return m_WindowSize;
}

void ImGuiWrapper::SetWindowFlags(const ImGuiWindowFlags& flags) {
	m_WindowFlags = flags;
}

ImGuiWindowFlags ImGuiWrapper::GetWindowFlags() const {
	return m_WindowFlags;
}
