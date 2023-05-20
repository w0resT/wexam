#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
//#include "../../interfaces/model/iappmodel.h"
#include "../../interfaces/localization/ilocalizationmanager.h"
#include "../../interfaces/tsystem/itestrepository.h"
#include "../../interfaces/tsystem/itestmanager.h"

class PageTests : public IPageView {
public:
	PageTests( std::shared_ptr<ILocalizationManager> _localize, std::shared_ptr<ITestRepository> _repos, std::shared_ptr<ITestManager> _manager ) :
		m_LocalizationManager( std::move( _localize ) ), m_Repository( std::move( _repos ) ), m_Manager( std::move( _manager ) ) {}

	void Draw() override;

private:
	std::shared_ptr<ITestRepository> m_Repository;
	std::shared_ptr<ITestManager> m_Manager;
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

};