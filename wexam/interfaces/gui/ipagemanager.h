#pragma once
#include <memory>

class IPageView;
enum class GUIPages;

class IPageManager {
public:
	virtual void AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) = 0;
	virtual void Draw() = 0;
	virtual void Draw(const GUIPages& page) = 0;
};