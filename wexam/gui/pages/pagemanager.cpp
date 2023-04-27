#include "pagemanager.h"

void PageManager::AddTab(std::unique_ptr<IPageView> tab) {
	m_tabs.push_back(std::move(tab));
}

void PageManager::Draw() {
	for (const auto& tab : m_tabs) {
		tab->Draw();
	}
}
