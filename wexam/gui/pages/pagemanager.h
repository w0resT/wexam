#pragma once
#include <unordered_map>

#include "../../interfaces/gui/ipagemanager.h"
#include "../../interfaces/gui/ipageview.h"

enum class GUIPages {
	Tests, Users, Settings, Info, MAX
};

class PageManager : public IPageManager {
public:
	PageManager();

	void AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) override;
	void Draw() override;
	void Draw(const GUIPages& page) override;

	void SetCurrentPage(const GUIPages& page) override;
	GUIPages GetCurrentPage() const override;

	std::string GetPageName(const GUIPages& page) const override;
	std::string GetCurrentPageName() const override;

private:
	std::unordered_map<GUIPages, std::unique_ptr<IPageView>> m_tabs;
	std::unordered_map<GUIPages, std::string> m_tabNames;

	GUIPages m_curPage;
};