#pragma once
#include <vector>
#include "../interfaces/itabmanager.h"

class TabManager : public ITabManager {
public:
	TabManager() {}
	~TabManager() {}

	void AddTab(std::unique_ptr<ITabView> tab) override;
	void Draw() override;

private:
	std::vector<std::unique_ptr<ITabView>> m_tabs;
};