#pragma once
#include "../../interfaces/gui/iimguiwrapper.h"

#include "../imgui/imgui.h"

class ImGuiWrapper : public IImGuiWrapper {
public:
	ImGuiWrapper();
	~ImGuiWrapper();

	void SetWindowSize(const ImVec2& size) override;
	ImVec2 GetWindowSize() const override;

	void SetWindowFlags(const ImGuiWindowFlags& flags) override;
	ImGuiWindowFlags GetWindowFlags() const override;

private:
	ImVec2 m_WindowSize;
	ImGuiWindowFlags m_WindowFlags;
};