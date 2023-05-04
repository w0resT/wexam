#pragma once
#include "../../interfaces/gui/iguiwrapper.h"

#include "../imgui/imgui.h"

class GUIWrapper : public IGUIWrapper {
public:
	GUIWrapper();
	~GUIWrapper();

	void SetWindowSize(const ImVec2& size) override;
	ImVec2 GetWindowSize() const override;

	void SetWindowFlags(const ImGuiWindowFlags& flags) override;
	ImGuiWindowFlags GetWindowFlags() const override;

private:
	ImVec2 m_WindowSize;
	ImGuiWindowFlags m_WindowFlags;
};