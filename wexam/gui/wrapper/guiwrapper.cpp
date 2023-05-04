#include "guiwrapper.h"
#include "../../defines.h"

GUIWrapper::GUIWrapper() {
	m_WindowSize = ImVec2(defines::WINDOW_W, defines::WINDOW_H);
	m_WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
}

GUIWrapper::~GUIWrapper() {
}

void GUIWrapper::SetWindowSize(const ImVec2& size) {
	m_WindowSize = size;
}

ImVec2 GUIWrapper::GetWindowSize() const {
	return m_WindowSize;
}

void GUIWrapper::SetWindowFlags(const ImGuiWindowFlags& flags) {
	m_WindowFlags = flags;
}

ImGuiWindowFlags GUIWrapper::GetWindowFlags() const {
	return m_WindowFlags;
}