#pragma once
#include <vector>
#include "../interfaces/ipagemanager.h"

class PageManager : public IPageManager {
public:
	PageManager() {}
	~PageManager() {}

	void AddTab(std::unique_ptr<IPageView> tab) override;
	void Draw() override;

private:
	std::vector<std::unique_ptr<IPageView>> m_tabs;
};