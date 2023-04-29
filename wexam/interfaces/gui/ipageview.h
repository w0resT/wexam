#pragma once

class IPageView {
public:
	virtual void Draw() = 0;

	virtual ~IPageView() {}
};