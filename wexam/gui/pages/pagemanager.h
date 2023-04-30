#pragma once
#include <unordered_map>

#include "../../interfaces/gui/ipagemanager.h"
#include "../../interfaces/gui/ipageview.h"

class PageManager : public IPageManager {
public:
	PageManager() {}
	~PageManager() {}

	void AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) override;
	void Draw() override;
	void Draw(const GUIPages& page) override;

private:
	std::unordered_map<GUIPages, std::unique_ptr<IPageView>> m_tabs;
};