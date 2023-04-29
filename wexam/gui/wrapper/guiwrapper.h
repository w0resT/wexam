#pragma once
#include "../../interfaces/gui/iguiwrapper.h"

#include "../imgui/imgui.h"

enum class GUIPages {
	Tests, Users, Settings, Info
};

class GUIWrapper : public IGUIWrapper {
public:
	GUIWrapper();
	~GUIWrapper();

	void SetWindowSize(const ImVec2& size) override;
	ImVec2 GetWindowSize() const override;

	void SetWindowFlags(const ImGuiWindowFlags& flags) override;
	ImGuiWindowFlags GetWindowFlags() const override;

	void SetCurrentDrawPage(const GUIPages& page) override;
	GUIPages GetCurrentDrawPage() const override;

private:
	ImVec2 m_WindowSize;
	ImGuiWindowFlags m_WindowFlags;

	GUIPages m_CurrentDrawPage;
};