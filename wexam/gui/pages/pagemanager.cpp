#include "pagemanager.h"

PageManager::PageManager() {
	// TODO: 
	// - add and use AddTabName instead hardcode
	m_tabNames[GUIPages::Tests] = "tabNameTests";
	m_tabNames[GUIPages::Users] = "tabNameUsers";
	m_tabNames[GUIPages::Settings] = "tabNameSettings";
	m_tabNames[GUIPages::Info] = "tabNameInfo";
}

void PageManager::AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) {
	m_tabs[page] = std::move(tab);
}

void PageManager::Draw() {
	for (const auto& tab : m_tabs) {
		tab.second->Draw();
	}
}

void PageManager::Draw(const GUIPages& page) {
	const auto& it = m_tabs.find(page);
	if (it != m_tabs.end()) {
		it->second->Draw();
	}
	else {
		// TODO: use assert
	}
}

void PageManager::SetCurrentPage(const GUIPages& page) {
	m_curPage = page;
}

GUIPages PageManager::GetCurrentPage() const {
	return m_curPage;
}

std::string PageManager::GetPageName(const GUIPages& page) const {
	const auto& it = m_tabNames.find(page);
	if (it != m_tabNames.end()) {
		return it->second;
	}
	else {
		// TODO: use assert
		return "ERROR";
	}
}

std::string PageManager::GetCurrentPageName() const {
	const auto& it = m_tabNames.find(GetCurrentPage());
	if (it != m_tabNames.end()) {
		return it->second;
	}
	else {
		return "ERROR";
	}
}
