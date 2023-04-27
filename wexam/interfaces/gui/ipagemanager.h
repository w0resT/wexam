#pragma once
#include <memory>

class IPageView;

class IPageManager {
public:
	virtual void AddTab(std::unique_ptr<IPageView> tab) = 0;
	virtual void Draw() = 0;
};