#pragma once
#include <memory>
#include <string>

class IPageView;
enum class GUIPages;

class IPageManager {
public:
	virtual void AddTab(std::unique_ptr<IPageView> tab, const GUIPages& page) = 0;
	virtual void Draw() = 0;
	virtual void Draw(const GUIPages& page) = 0;

	virtual void SetCurrentPage(const GUIPages& page) = 0;
	virtual GUIPages GetCurrentPage() const = 0;

	virtual std::string GetPageName(const GUIPages& page) const = 0;
	virtual std::string GetCurrentPageName() const = 0;

	virtual ~IPageManager() {}
};