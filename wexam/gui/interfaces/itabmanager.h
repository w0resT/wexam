#pragma once
#include <memory>
#include "itabview.h"

class ITabManager {
public:
	virtual void AddTab(std::unique_ptr<ITabView> tab) = 0;
	virtual void Draw() = 0;
};