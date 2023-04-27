#pragma once
#include <memory>
#include "ipageview.h"

class IPageManager {
public:
	virtual void AddTab(std::unique_ptr<IPageView> tab) = 0;
	virtual void Draw() = 0;
};