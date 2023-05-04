#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/model/iappmodel.h"
#include "../../interfaces/localization/ilocalizationmanager.h"

class PageTests : public IPageView {
public:
	PageTests(std::shared_ptr<ILocalizationManager> _localize, std::shared_ptr<IAppModel> _model) : 
		m_LocalizationManager(std::move(_localize)), m_Model(std::move(_model)) {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

};