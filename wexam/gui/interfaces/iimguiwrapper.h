#pragma once

struct ImVec2;
typedef int ImGuiWindowFlags;

class IImGuiWrapper {
public:
	virtual void SetWindowSize(const ImVec2& size) = 0;
	virtual ImVec2 GetWindowSize() const = 0;

	virtual void SetWindowFlags(const ImGuiWindowFlags& flags) = 0;
	virtual ImGuiWindowFlags GetWindowFlags() const = 0;
};