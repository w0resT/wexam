#include "tabmanager.h"

void TabManager::AddTab(std::unique_ptr<ITabView> tab) {
	m_tabs.push_back(std::move(tab));
}

void TabManager::Draw() {
	for (const auto& tab : m_tabs) {
		tab->Draw();
	}
}
