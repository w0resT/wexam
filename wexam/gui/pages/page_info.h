#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/localization/ilocalizationmanager.h"

class PageInfo : public IPageView {
public:
	PageInfo(std::shared_ptr<ILocalizationManager> _localize) : 
		m_LocalizationManager(std::move(_localize)) {}

	void Draw() override;

private:
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

};