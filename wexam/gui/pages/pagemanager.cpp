#include "pagemanager.h"

void PageManager::AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) {
	m_tabs[page] = std::move(tab);
}

void PageManager::Draw() {
	// TODO: add here GetCurrentDrawPage
	for (const auto& tab : m_tabs) {
		tab.second->Draw();
	}
}

void PageManager::Draw(const GUIPages& page) {
	m_tabs[page]->Draw();
}
