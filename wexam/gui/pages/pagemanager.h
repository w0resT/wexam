#pragma once
#include <map>

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
	std::map<GUIPages, std::unique_ptr<IPageView>> m_tabs;
};